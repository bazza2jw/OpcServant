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
#include "Mics6814rt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#ifdef RASPBERRY_PI_BUILD
    #include "pigpio.h"
#endif


MRL::StringVector MRL::Mics6814RT::_inputs = {
    "Ox", "Red", "NH3"
};

/*!
    \brief start
*/
void MRL::Mics6814RT::start()
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
void MRL::Mics6814RT::stop()
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
void MRL::Mics6814RT::publish()
{
    RTObject::publish();
    publishValue(_ox,"Ox");
    publishValue(_red,"Red");
    publishValue(_nh3,"NH3");
}

/*!
    \brief measure
*/
void MRL::Mics6814RT::measure()
{
    RTObject::measure();
    updateValue(_ox, "Ox");
    updateValue(_red,"Red");
    updateValue(_nh3,"NH3");
    updateObject();
}

/*!
 * \brief MRL::Mics6814RT::onOneSecond
 * \param t
 * fetch each input
 */
void MRL::Mics6814RT::onOneSecond(time_t t)
{
    RTObject::onOneSecond(t);
    if(_i2c->start(ADS1015_ADDRESS))
    {
        switch(_state)
        {
        case STATE_IDLE:
            break;
        case STATE_START:
        {
            _adc = _i2c.get();
            _adc.setGain(GAIN_TWOTHIRDS);
            _state= STATE_MEASURE;
        }
            break;
        case STATE_MEASURE:
        {
            _ox  = double(_adc.readADC_SingleEnded(0)) * (3.3 / double(0x7FFF)) ;
            _red = double(_adc.readADC_SingleEnded(1)) * (3.3 / double(0x7FFF)) ;
            _nh3 = double(_adc.readADC_SingleEnded(2)) * (3.3 / double(0x7FFF)) ;
            _ox  = (_ox * 56000.0)  / (3.3  - _ox);
            _red = (_red * 56000.0) / (3.3 - _red);
            _nh3 = (_nh3 * 56000.0) / (3.3 - _nh3);
        }
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
 * \brief MRL::Mics6814RT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::Mics6814RT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/)
{
    Open62541::Variant t(_ox);
    // find the child node called Value
    server.writeValue(_oxNode,t);
    Open62541::Variant p(_red);
    // find the child node called Value
    server.writeValue(_redNode,p);
    Open62541::Variant h(_nh3);
    // find the child node called Value
    server.writeValue(_nh3Node,h);

}


/*!
 * \brief MRL::Mics6814RT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::Mics6814RT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
    server.getChild(node,"Ox",_oxNode);
    server.getChild(node,"Red",_redNode);
    server.getChild(node,"NH3",_nh3Node);

}


/*!
 * \brief publishHtmlPage
 */
void MRL::Mics6814RT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
    //
}

/*!
 * \brief measureHtmlPage
 */
void MRL::Mics6814RT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
}
