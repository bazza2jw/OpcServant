/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "sps30rt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>

MRL::StringVector MRL::SPS30RT::_inputs =
{   "PM1.0",
    "PM2.5",
    "PM4.0",
    "PM10.0",
    "0.5um",
    "1.0um",
    "2.5um",
    "4.0um",
    "10.0um",
    "TypicalParticleSize"
};

static MRL::I2cInterfaceBase *i2cIf =nullptr;

/*!
    \brief start
*/
void MRL::SPS30RT::start()
{
    RTObject::start();
    _i2cType = configuration().getInt("/I2CType");
    _i2cAddress = configuration().getString("/I2CAddress");
    _i2cPort = configuration().getInt("/I2CPort");
    MRL::I2cInterfaceBase::makeI2c(MRL::I2cInterfaceBase::Type(_i2cType),_i2c,_i2cAddress,_i2cPort);
    i2cIf =_i2c.get();
}

/*!
    \brief stop
*/
void MRL::SPS30RT::stop()
{
    RTObject::stop();
    _i2c.reset();
    i2cIf = nullptr;
}

/*!
    \brief publish
*/
void MRL::SPS30RT::publish()
{
    RTObject::publish();
    publishValue(_m.mc_1p0,"PM1.0");
    publishValue(_m.mc_2p5,"PM2.5");
    publishValue(_m.mc_4p0,"PM4.0");
    publishValue(_m.mc_10p0,"PM10.0");
    publishValue(_m.nc_0p5,"0.5um");
    publishValue(_m.nc_1p0,"1.0um");
    publishValue(_m.nc_2p5,"2.5um");
    publishValue(_m.nc_4p0,"4.0um");
    publishValue(_m.nc_10p0,"10.0um");
    publishValue(_m.typical_particle_size,"TypicalParticleSize");

}

/*!
    \brief measure
*/
void MRL::SPS30RT::measure()
{
    RTObject::measure();
    updateValue(_m.mc_1p0,"PM1.0");
    updateValue(_m.mc_2p5,"PM2.5");
    updateValue(_m.mc_4p0,"PM4.0");
    updateValue(_m.mc_10p0,"PM10.0");
    updateValue(_m.nc_0p5,"0.5um");
    updateValue(_m.nc_1p0,"1.0um");
    updateValue(_m.nc_2p5,"2.5um");
    updateValue(_m.nc_4p0,"4.0um");
    updateValue(_m.nc_10p0,"10.0um");
    updateValue(_m.typical_particle_size,"TypicalParticleSize");
    updateObject();
}

/*!
 * \brief MRL::SPS30RT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::SPS30RT::updateOpc(OpcServer &server, Open62541::NodeId &objectNode)
{
    MRL::RTObject::updateOpc(server,objectNode);
    Open62541::UANodeTree &t = tree();
    Open62541::UAPath p;
    Open62541::Variant mc_1p0(_m.mc_1p0);
    t.setNodeValue(p,"PM1.0",mc_1p0);
    Open62541::Variant mc_2p5(_m.mc_2p5);
    t.setNodeValue(p,"PM2.5",mc_2p5);
    Open62541::Variant  mc_4p0(_m.mc_4p0);
    t.setNodeValue(p,"PM4.0",mc_4p0);
    Open62541::Variant mc_10p0(_m.mc_10p0);
    t.setNodeValue(p,"PM10.0",mc_10p0);
    Open62541::Variant  nc_0p5(_m.nc_0p5);
    t.setNodeValue(p,"0.5um",nc_0p5);
    Open62541::Variant nc_1p0(_m.nc_1p0);
    t.setNodeValue(p,"1.0um",nc_1p0);
    Open62541::Variant nc_2p5(_m.nc_2p5);
    t.setNodeValue(p,"2.5um",nc_2p5);
    Open62541::Variant nc_4p0(_m.nc_4p0);
    t.setNodeValue(p,"4.0um",nc_4p0);
    Open62541::Variant nc_10p0(_m.nc_10p0);
    t.setNodeValue(p,"10.0um",nc_10p0);
    Open62541::Variant tps(_m.typical_particle_size);
    t.setNodeValue(p,"TypicalParticleSize",tps);
}


/*!
 * \brief MRL::SPS30RT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::SPS30RT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
    _tree = std::make_unique<Open62541::ServerNodeTree>(server,node,server.applicationNamespace());
    server.browseTree(node,tree()); // set up the tree
}


/*!
 * \brief publishHtmlPage
 */
void MRL::SPS30RT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
}

/*!
 * \brief measureHtmlPage
 */
void MRL::SPS30RT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
}

void MRL::SPS30RT::onOneSecond(time_t t)
{
    RTObject::onOneSecond(t);
    if(i2cIf)
    {
        sensirion_i2c_init(); // open I2C channel
        switch(_state)
        {
        case STATE_IDLE:
            break;
        case STATE_RESET:
            if(!sps30_probe())
            {
                if(sps30_start_measurement() == 0)
                {
                    _state = STATE_WAIT;
                    _waitTime = 0;
                }
            }
            else
            {
                MRL::Common::instance()->logMessage(__FUNCTION__," Failed to initialise SPS30",STATES::STATE_FAULT);
            }
            break;
        case STATE_WAIT:
        {
            _waitTime++;
            if(_waitTime > 2) _state = STATE_MEASURE;
        }
        break;
        case STATE_MEASURE:
        {
            int ret = sps30_read_measurement(&_m);
            if (ret < 0) {
                MRL::Common::instance()->logMessage(__FUNCTION__,"error reading measurement",STATES::STATE_FAULT);
                _state = STATE_FAIL;

            } else {
                _state = STATE_WAIT;
                _waitTime = 0;
            }
        }
        break;
        case STATE_FAIL:
            _state = STATE_RESET;
            break;

        default:
            _state = STATE_FAIL;
            break;
        }

        sensirion_i2c_release();
    }
}


extern "C"
{

#define I2C_WRITE_FAILED -1
#define I2C_READ_FAILED -1

    /**
     * Initialize all hard- and software components that are needed for the I2C
     * communication.
     */
    void sensirion_i2c_init() {
        i2cIf->start(0x69);
        return; /* no error handling */
    }

    /**
     * Release all resources initialized by sensirion_i2c_init().
     */
    void sensirion_i2c_release(void) {
        if(i2cIf)
        {
            i2cIf->end();
        }
        i2cIf = nullptr;
    }

    /**
     * Execute one read transaction on the I2C bus, reading a given number of bytes.
     * If the device does not acknowledge the read command, an error shall be
     * returned.
     *
     * @param address 7-bit I2C address to read from
     * @param data    pointer to the buffer where the data is to be stored
     * @param count   number of bytes to read from I2C and store in the buffer
     * @returns 0 on success, error code otherwise
     */
    int8_t sensirion_i2c_read(uint8_t /*address*/, uint8_t *data, uint16_t count) {
        if(i2cIf)
        {
            if(!i2cIf->blockRead(data, count))
            {
                return I2C_READ_FAILED;
            }
        }
        return 0;
    }

    /**
     * Execute one write transaction on the I2C bus, sending a given number of
     * bytes. The bytes in the supplied buffer must be sent to the given address. If
     * the slave device does not acknowledge any of the bytes, an error shall be
     * returned.
     *
     * @param address 7-bit I2C address to write to
     * @param data    pointer to the buffer containing the data to write
     * @param count   number of bytes to read from the buffer and send over I2C
     * @returns 0 on success, error code otherwise
     */
    int8_t sensirion_i2c_write(uint8_t /*address*/, const uint8_t *data, uint16_t count) {
        if(i2cIf)
        {
            if(!i2cIf->blockWrite(data,count))
            {
                return I2C_WRITE_FAILED;
            }
        }
        return 0;
    }


    /**
     * Sleep for a given number of microseconds. The function should delay the
     * execution for at least the given time, but may also sleep longer.
     *
     * @param useconds the sleep time in microseconds
     */
    void sensirion_sleep_usec(uint32_t useconds) {
        usleep(useconds);
    }
}
