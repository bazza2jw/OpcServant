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
#include "OPCN3rt.h"

MRL::StringVector MRL::OPCN3RT::_inputs = {"PM1","PM2.5","PM10","Status","Temperature","Humidity"};
std::map<unsigned,MRL::OPCN3RT *> MRL::OPCN3RT::_map;

/*!
    \brief start
*/
void MRL::OPCN3RT::start()
{

    // initialise from configuration
    // get the range
    _sampleInterval = configuration().getValue<int>("/SampleInterval");
    if(_sampleInterval < 10) _sampleInterval= 60;
    _sampleInterval *= 1000; // convert to ms
    _waitInterval = configuration().getValue<int>("/WaitInterval");
    _waitInterval *= 60000; // convert to ms
    _address = configuration().getValue<std::string>("/SPIDevice"); // this is really fixed to SPI 0
    //
    _state = STATE_START;
    RTObject::start();
}

/*!
    \brief stop
*/
void MRL::OPCN3RT::stop()
{
    RTObject::stop();
    _state = STATE_IDLE;
}

/*!
 * \brief MRL::OPCN3RT::onOneSecond
 * \param t
 */
void MRL::OPCN3RT::onOneSecond(time_t t)
{
    // talk to the OPCN3 slow and steady
    MRL::RTObject::onOneSecond(t);
    switch(_state)
    {
    case STATE_IDLE:
        break;
    case STATE_START:
        _timer.Start();
        if(_opc.start())
        {
            _state = STATE_WAIT_SAMPLE;
        }
        else
        {
            _state = STATE_FAIL;
        }
        break;
    case STATE_WAIT:
        if(_timer.Time() >= _waitInterval)
        {
            _timer.Start();
            _state = STATE_START;
        }
        break;
    case STATE_FAIL:
        if(_timer.Time() >= FAIL_WAIT_TIME)
        {
            _state = STATE_START;
        }
        break;
    case START_STOP:
        break;
    case STATE_WAIT_SAMPLE:
        if(_timer.Time() >= _sampleInterval)
        {
            _timer.Start();
            if(_opc.read(OPCN3::OP_READ_HISTOGRAM_DATA, _results)) // read the data
            {
                //
                RTObject::measure();
                _status = ((_results.laserStatus > LASER_OK)); // the status
                //
                _ok = true;
                //
                int state = _status?STATES::States::StateOk:STATES::States::StateFault;
                updateValue(_status,"Status",state);
                updateValue(double(_results.pm1),"PM1",state);
                updateValue(double(_results.pm2_5), "PM2.5",state);
                updateValue(double(_results.pm10), "PM10",state);
                updateValue(_results.laserStatus,"LaserStatus",(_results.laserStatus > LASER_OK)?STATES::States::StateOk:STATES::States::StateFault);
                //updateValue(_results.fanRevCount,"FanSpeed",(_results.fanRevCount > FAN_OK)?STATES::States::StateOk:STATES::States::StateFault);
                updateValue(_opc.convSTtoTemperature(_results.temperature),"Temperature");
                updateValue(_opc.convSRHtoRelativeHumidity(_results.humidity),"Humidity");
                runtime().setString("StatusDesc",state?"OK":"FAULT");

                updateObject();

                //
                RTObject::publish();
                publishValue(double(_results.pm1),   "PM1",state);
                publishValue(double(_results.pm2_5), "PM2.5",state);
                publishValue(double(_results.pm10),  "PM10",state);
                publishValue(_results.laserStatus,"LaserStatus",(_results.laserStatus > LASER_OK)?STATES::States::StateOk:STATES::States::StateFault);
                publishValue(_status,"Status",state);
                publishValue(_opc.convSTtoTemperature(_results.temperature),"Temperature");
                publishValue(_opc.convSRHtoRelativeHumidity(_results.humidity),"Humidity");
                //
                Message m(MESSAGEID::Publish_Object);
                // say we are posting an update
                toUpdateValue(m,double(_results.pm1),   "PM1",state);
                toUpdateValue(m,double(_results.pm2_5), "PM2.5",state);
                toUpdateValue(m,double(_results.pm10),  "PM10",state);
                toUpdateValue(m,_results.laserStatus,"LaserStatus",(_results.laserStatus > LASER_OK)?STATES::States::StateOk:STATES::States::StateFault);
                //toUpdateValue(m,_results.fanRevCount,"FanSpeed",(_results.fanRevCount > FAN_OK)?STATES::States::StateOk:STATES::States::StateFault); // CURRENTLY returns 0
                toUpdateValue(m,_status,"Status",state);
                toUpdateValue(m,_opc.convSTtoTemperature(_results.temperature),"Temperature");
                toUpdateValue(m,_opc.convSRHtoRelativeHumidity(_results.humidity),"Humidity");
                // now post the value
                message().emit(m);
                //
                if(_waitInterval > 0)
                {
                    _opc.stop(); // power off laser and fan
                    _state = STATE_WAIT;
                }
            }
            else
            {
                _state = STATE_FAIL;
                _ok = false;
            }
        }
        break;
    default:
        _state = STATE_IDLE;
        break;
    }
}


/*!
 * \brief MRL::OPCN3RT::onOneMinute
 * \param t
 */
void MRL::OPCN3RT::onOneMinute(time_t t)
{
    // sync the RTC from time to time
    MRL::RTObject::onOneMinute(t);
}




/*!
 * \brief MRL::OPCN3RT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::OPCN3RT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/)
{
    // update the value
    Open62541::Variant p0(double(_results.pm1));
    Open62541::Variant p1(double(_results.pm2_5));
    Open62541::Variant p2(double(_results.pm10));
    Open62541::Variant s(_status);
    Open62541::Variant l(_results.laserStatus);
    //Open62541::Variant f(_results.fanRevCount);
    Open62541::Variant t(_opc.convSTtoTemperature(_results.temperature));
    Open62541::Variant h(_opc.convSRHtoRelativeHumidity(_results.humidity));
    //
    // find the child node called Value
    server.writeValue(_pm1Node,p0);
    server.writeValue(_pm2_5Node,p1);
    server.writeValue(_pm10Node,p2);
    server.writeValue(_statusNode,s);
    server.writeValue(_laserNode,l);
    //server.writeValue(_fanNode,f);
    server.writeValue(_temperatureNode,t);
    server.writeValue(_humidityNode,h);
    //
}


/*!
 * \brief MRL::OPCN3RT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::OPCN3RT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
    //
    // set up the variables
    //
    server.getChild(node,"PM1",_pm1Node);
    server.getChild(node,"PM2.5",_pm2_5Node);
    server.getChild(node,"PM10",_pm10Node);
    server.getChild(node,"Status",_statusNode);
    server.getChild(node,"LaserStatus",_laserNode);
    //server.getChild(node,"FanSpeed",_fanNode);
    server.getChild(node,"Temperature",_temperatureNode);
    server.getChild(node,"Humidity",_humidityNode);

    //
}


/*!
 * \brief publishHtmlPage
 */
void MRL::OPCN3RT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
    std::stringstream ss;
    ss << "PM1   " << _results.pm1 << std::endl;
    ss << "PM2.5 " << _results.pm2_5 << std::endl;
    ss << "PM10  " << _results.pm10 << std::endl;
    ss << "Laser Status  " << _results.laserStatus << std::endl;
    //ss << "Fan Speed " << _results.fanRevCount << std::endl;
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
void MRL::OPCN3RT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
    std::stringstream ss;
    ss << "PM1   " << _results.pm1 << std::endl;
    ss << "PM2.5 " << _results.pm2_5 << std::endl;
    ss << "PM10  " << _results.pm10 << std::endl;
    ss << "Laser Status  " << _results.laserStatus << std::endl;
    //ss << "Fan Speed " << _results.fanRevCount << std::endl;
    ss << "Status  " << std::hex << _status << std::endl;
    CTML::Node v("p");
    v.SetContent(ss.str());
    s.AppendChild(v);
}
