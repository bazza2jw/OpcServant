/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided with open62541.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "EnviroPlusrt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#ifdef RASPBERRY_PI_BUILD
    #include pigpio.h"
#endif
MRL::StringVector MRL::EnviroPlusRT::_inputs = {
    "NO2", "CO", "NH3","Noise","Light"
};

/*!
    \brief start
*/
void MRL::EnviroPlusRT::start()
{
    //
    RTObject::start();
     _state = STATE_START;
     _i2cType = configuration().getInt("/I2CType");
     _i2cAddress = configuration().getString("/I2CAddress");
     _i2cPort = configuration().getInt("/I2CPort");
     MRL::I2cInterfaceBase::makeI2c(MRL::I2cInterfaceBase::Type(_i2cType),_i2c,_i2cAddress,_i2cPort);
#ifdef RASPBERRY_PI_BUILD
     // switch on the gas sensor heater
     gpioSetMode(24, PI_OUTPUT);
     gpioWrite(24,1);
#endif
}

/*!
    \brief stop
*/
void MRL::EnviroPlusRT::stop()
{
    RTObject::stop();
    _state = STATE_IDLE;
    _i2c.reset();
#ifdef RASPBERRY_PI_BUILD
     // switch off the gas sensor heater
    gpioWrite(24,0);
    gpioSetMode(24, PI_INPUT);
#endif
}

/*!
    \brief publish
*/
void MRL::EnviroPlusRT::publish()
{
    RTObject::publish();
}

/*!
    \brief measure
*/
void MRL::EnviroPlusRT::measure()
{
    RTObject::measure();
    updateObject();
}

/*!
 * \brief MRL::EnviroPlusRT::onOneSecond
 * \param t
 */
void MRL::EnviroPlusRT::onOneSecond(time_t t)
{
    RTObject::onOneSecond(t);

        switch(_state)
        {
        case STATE_IDLE:
            break;
        case STATE_START:
            _state = STATE_LTR;
            break;
        case STATE_LTR: // read the light level
            _state = STATE_GASES;
            break;
        case STATE_GASES: // read the gases
            // read the analogs
            _state = STATE_NOISE;
            break;
        case STATE_NOISE: // read the noise
            _state = STATE_START;
            break;
        default:
            _state = STATE_IDLE;
            break;
        }
}


/*!
 * \brief MRL::EnviroPlusRT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::EnviroPlusRT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/)
{
}


/*!
 * \brief MRL::EnviroPlusRT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::EnviroPlusRT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
}


/*!
 * \brief publishHtmlPage
 */
void MRL::EnviroPlusRT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
    //
}

/*!
 * \brief measureHtmlPage
 */
void MRL::EnviroPlusRT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
}
