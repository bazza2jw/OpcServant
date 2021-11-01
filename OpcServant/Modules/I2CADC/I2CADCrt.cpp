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
#include "I2CADCrt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>

MRL::StringVector MRL::I2CADCRT::_inputs = {
    "NO2", "CO", "NH3","Noise","Light"
};

/*!
    \brief start
*/
void MRL::I2CADCRT::start()
{
    //
    RTObject::start();
     _state = STATE_RESET;
     _i2cType = configuration().getInt("/I2CType");
     _i2cAddress = configuration().getString("/I2CAddress");
     _i2cPort = configuration().getInt("/I2CPort");
     MRL::I2cInterfaceBase::makeI2c(MRL::I2cInterfaceBase::Type(_i2cType),_i2c,_i2cAddress,_i2cPort);

}

/*!
    \brief stop
*/
void MRL::I2CADCRT::stop()
{
    RTObject::stop();
    _state = STATE_IDLE;
    _i2c.reset();
}

/*!
    \brief publish
*/
void MRL::I2CADCRT::publish()
{
    RTObject::publish();
}

/*!
    \brief measure
*/
void MRL::I2CADCRT::measure()
{
    RTObject::measure();
    updateObject();
}

/*!
 * \brief MRL::I2CADCRT::onOneSecond
 * \param t
 * fetch each input
 */
void MRL::I2CADCRT::onOneSecond(time_t t)
{
    RTObject::onOneSecond(t);
    if(_i2c->start(_bmeAddr))
    {
        switch(_state)
        {
        case STATE_IDLE:
            break;
        case STATE_FAULT:
            _state = STATE_IDLE;
            break;
        default:
            _state = STATE_IDLE;
            break;
        }
        _i2c->end();
    }

}


/*!
 * \brief MRL::I2CADCRT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::I2CADCRT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/)
{
}


/*!
 * \brief MRL::I2CADCRT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::I2CADCRT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
}


/*!
 * \brief publishHtmlPage
 */
void MRL::I2CADCRT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
    //
}

/*!
 * \brief measureHtmlPage
 */
void MRL::I2CADCRT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
}
