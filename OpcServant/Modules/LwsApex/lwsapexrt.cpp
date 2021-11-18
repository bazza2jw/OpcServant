/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "lwsapexrt.h"

MRL::StringVector MRL::LwsApexRT::_inputs = {"0.5","5.0","Status"};
std::map<unsigned,MRL::LwsApexRT *> MRL::LwsApexRT::_map;

/*!
    \brief start
*/
void MRL::LwsApexRT::start()
{

    // initialise from configuration
    // get the range
    _sampleTime = configuration().getValue<int>("SampleTime");
    if(_sampleTime < 10) _sampleTime= 60;
    _purgeTime = configuration().getValue<int>("PurgeTime");
    _address = configuration().getValue<std::string>("Address");
    if(!_address.empty())
    {
        _modbus.setTcpConnection(_address, "502");
    }
    //
    _state = STATE_INIT;
    RTObject::start();
}

/*!
    \brief stop
*/
void MRL::LwsApexRT::stop()
{
    RTObject::stop();
    _state = STATE_IDLE;
}

/*!
 * \brief MRL::LwsApexRT::onOneSecond
 * \param t
 */
void MRL::LwsApexRT::onOneSecond(time_t t)
{
    MRL::RTObject::onOneSecond(t);
    switch(_state)
    {
    case STATE_IDLE:
        break;

    case STATE_INIT:
    {
        if(_modbus.connectTo())
        {
            // initialise
            // set the sample time
            // set the purge time
            // set the record index
            // clear the sample buffer
            if(_modbus.writeRegister(1,1,12)) // stop sampling
            {
                _modbus.writeRegister(1,29,0);
                _modbus.writeRegister(1,30,0);
                _modbus.writeRegister(1,31,_purgeTime);
                _modbus.writeRegister(1,32,0);
                _modbus.writeRegister(1,33,_sampleTime);
                _modbus.writeRegister(1,24,0xFFFF); // read last record
                _modbus.writeRegister(1,1,3); // clear buffer
                _lastCount = 0;
                //
                // Set the RTC
                _modbus.writeRegister(1,34,((unsigned)t) & 0xFFFF);
                _modbus.writeRegister(1,35,((unsigned)t) >> 16);
                _modbus.writeRegister(1,1,13); // set RTC
                //
                _modbus.writeRegister(1,1,11); // start counting
                _state = STATE_PROCESS;
            }
            _modbus.close();
            //
            _status = 0;
            _particles_0_5 = 0.0;
            _particles_5_0 = 0.0;
            //
        }
    };
    break;
    case STATE_PROCESS:
    {

        if(_modbus.connectTo())
        {
            uint16_t devStatus = 0;
            if(_modbus.readRegisters(1, 2, 1, &devStatus))
            {
                if(devStatus & 1) // running?
                {
                    uint16_t count = 0;
                    if(_modbus.readRegisters(1, 23, 1, &count))
                    {
                        if(count != _lastCount)
                        {
                            _lastCount = count;
                            //
                            // read the status and the particle counts - for now assume two channels
                            //
                            uint16_t res[6];
                            if(_modbus.readInputRegisters(1,6,6,res)) // 3 longs
                            {
                                RTObject::measure();
                                _status = MRL::toLong(res[1],0); // the status
                                _particles_0_5 = double(MRL::toLong(res[3],res[2])) * double(_sampleTime) / 60.0;
                                _particles_5_0 = double(MRL::toLong(res[5],res[4])) * double(_sampleTime) / 60.0;
                                //
                                //
                                updateValue(_status,"Status",_status?STATES::States::StateFault:STATES::States::StateOk);
                                updateValue(_particles_0_5,"0.5",_status?STATES::States::StateFault:STATES::States::StateOk);
                                updateValue(_particles_5_0,"5.0",_status?STATES::States::StateFault:STATES::States::StateOk);
                                updateObject();

                                //
                                RTObject::publish();
                                Message m(MESSAGEID::Publish_Object);
                                // say we are posting an update
                                toUpdateValue(m,_particles_0_5,"0.5",_status?STATES::States::StateFault:STATES::States::StateOk);
                                toUpdateValue(m,_particles_5_0,"5.0",_status?STATES::States::StateFault:STATES::States::StateOk);
                                toUpdateValue(m,_status,"Status",_status?STATES::States::StateFault:STATES::States::StateOk);
                                // now post the value
                                message().emit(m);



                                if(_status & 1)
                                {
                                    runtime().setString("StatusDesc","Laser Failure");
                                }
                                else if(_status & 2)
                                {
                                    runtime().setString("StatusDesc","Flow Failure");
                                }
                                else if(_status & 4)
                                {
                                    runtime().setString("StatusDesc","Overflow Failure");
                                }
                                else if(_status & 8)
                                {
                                    runtime().setString("StatusDesc","Instrument Failure");
                                }
                                else
                                {
                                    runtime().setString("StatusDesc","OK");
                                };

                                if(_lastCount > 200)
                                {
                                    _modbus.writeRegister(1,1,3); // clear buffer
                                    _lastCount = 0;
                                }
                            }
                        }
                    }
                }
                else
                {
                    _state = STATE_INIT; // stopped for some reason - restart
                }
            }
            _modbus.close();
        } // cannot connect  - always possible there is more than one master
    }
    break;
    default:
        _state = STATE_INIT;
        break;
    }
}


/*!
 * \brief MRL::LwsApexRT::onOneMinute
 * \param t
 */
void MRL::LwsApexRT::onOneMinute(time_t t)
{
    // sync the RTC from time to time
    MRL::RTObject::onOneMinute(t);
}




/*!
 * \brief MRL::LwsApexRT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::LwsApexRT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/)
{
    // update the value
    Open62541::Variant p0(_particles_0_5);
    Open62541::Variant p1(_particles_5_0);
    Open62541::Variant s(_status);
    //
    // find the child node called Value
    server.writeValue(_particles_0_5Node,p0);
    server.writeValue(_particles_5_0Node,p1);
    server.writeValue(_statusNode,s);
    //
}


/*!
 * \brief MRL::LwsApexRT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::LwsApexRT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
    //
    // set up the variables
    //
    server.getChild(node,"0.5",_particles_0_5Node);
    server.getChild(node,"5.0",_particles_5_0Node);
    server.getChild(node,"Status",_statusNode);
    //
}


/*!
 * \brief publishHtmlPage
 */
void MRL::LwsApexRT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
    std::stringstream ss;
    ss << "0.5     " << _particles_0_5 << std::endl;
    ss << "5.0  " << _particles_5_0 << std::endl;
    ss << "Status  " << std::hex << _status << std::endl;
    //
    CTML::Node v("p");
    v.SetContent(ss.str());
    s.AppendChild(v);
    //
}

/*!
 * \brief measureHtmlPage
 */
void MRL::LwsApexRT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
    std::stringstream ss;
    ss << "0.5     " << _particles_0_5 << std::endl;
    ss << "5.0  " << _particles_5_0 << std::endl;
    ss << "Status  " << std::hex << _status << std::endl;
    CTML::Node v("p");
    v.SetContent(ss.str());
    s.AppendChild(v);
}
