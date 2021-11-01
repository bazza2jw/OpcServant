/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "icpcon7017rt.h"

MRL::StringVector MRL::ICPCON7017RT::_channelNames = {"IN0", "IN1", "IN2", "IN3", "IN4", "IN5", "IN6"};

/*!
    \brief start
*/
void MRL::ICPCON7017RT::start() {
    RTObject::start();

    _map.clear();
    for(int i = 0; i < NUMBER_7017_CHAN; i++)
    {
        _map[_channelNames[i]] = ScaleOffset();
    }
    //
    PropertyPath so;
    so.push_back("ScaleOffset");
    _map.read(configuration(),so); // load from configuration
    //
    // get the configuration
    // get port
    _port =  configuration().getValue<std::string>("SerialPort");
    // get node id
    _node = configuration().getValue<int>("NodeAddress");
    //Baud Rate
    _baudRate =  MRL::stringToNumber<int>(configuration().getValue<std::string>("BaudRate"));
    _publishRecord = configuration().getValue<bool>("PublishRecord");
    //
    makePackets();
    _state = STATE_ID;
}

/*!
    \brief stop
*/
void MRL::ICPCON7017RT::stop() {
    RTObject::stop();
    _state = STATE_IDLE;
}

/*!
    \brief publish
*/
void MRL::ICPCON7017RT::publish() {
    RTObject::publish();
    // publish each of the inputs
    for (int i = 0; i < NUMBER_7017_CHAN; i++) {
        _stats[i].evaluate();
        publishValue(_stats[i].statistics().getMean(), _channelNames[i],
                     _nodeOk ? STATES::States::StateOk : STATES::States::StateAction);
    }
    //
    // publish the record
    //
    if(_publishRecord)
    {
        Json::Value v;
        v["time"] = wxDateTime::Now().FormatISOCombined().ToStdString();
        for(int i = 0; i < NUMBER_7017_CHAN; i++)
        {
            v[_channelNames[i]] = _stats[i].statistics().getMean();
        }
        //
        publishRecord(v);
        //
    }
}

/*!
    \brief measure
*/
void MRL::ICPCON7017RT::measure() {
    RTObject::measure();
    for (int i = 0; i < NUMBER_7017_CHAN; i++) {
        updateValue(_map[ _channelNames[i]].current(), _channelNames[i]);
    }
    updateObject(_nodeOk ? STATES::States::StateOk : STATES::States::StateAction);
}
/*!
    \brief MRL::ICPCON7017RT::process
*/
void MRL::ICPCON7017RT::process() {
    RTObject::process();
    switch (_state) {
        case STATE_IDLE:
            break;
        case STATE_ERROR: {
            if (_timer.Time() > _errorWait) {
                _state = STATE_ID;
            }
        }
        break;
        case STATE_ID: { // identify the module
            SerialPacketRef &r = MRL::Common::daq().serial(_port);
            if (r->claim()) {
                // open port
                if (r->openSerial(_port, _baudRate, SC_8DATA, SC_1STOP, SC_NOPARITY)) {
                    // send the module request
                    _nodeOk = true;
                    std::string m = r->doPacket(moduleList);
                    // is it 7017?
                    if ((m.length() > 6) && (m.substr(3, 4) == "7017")) {
                        _state = STATE_SEND;
                        r->doPacket(enableAll); // enable all channels
                    }
                    else {
                        setFault("Not a 7017");
                    }
                    r->close();
                }
                else {
                    setFault("7017 Serial");
                }
                r->release();
            }
        }
        break;
        case STATE_SEND: { // send the channel read request
            SerialPacketRef &r = MRL::Common::daq().serial(_port);
            if (_secondFlag && r->claim()) {
                if (r->openSerial(_port, _baudRate, SC_8DATA, SC_1STOP, SC_NOPARITY)) {
                    _nodeOk = true;
                    r->startPacket(getData);
                    _state = STATE_RECEIVE;
                }
                else {
                    setFault("7017 Serial");
                    r->release();
                }
            }
            // get lock on serial port
            // send request
        }
        break;
        case STATE_RECEIVE: {
            SerialPacketRef &r = MRL::Common::daq().serial(_port);
            if(r->packetDrive()) // drive the packet
            {
                // get data
                std::string &s = r->buffer();
                //
                TRC("Rxd:" << s);
                if ( s.length() > (49)) {
                    for (int i = 0; i < NUMBER_7017_CHAN; i++) {
                        std::string v = s.substr(1 + (i * 7));
                        if (v[0] == '+') v[0] = ' ';
                        ScaleOffset &o = _map[ _channelNames[i]];
                        o.scale(MRL::stringToNumber<double>(v));
                        _stats[i].addValue(o.current());
                    }
                }
                // process packets
                r->close();
                r->release();
                _secondFlag = false;
                _state = STATE_SEND;
            }
            else if(r->timedOut())
            {
                 setFault("7017 Serial Timed Out");
                 r->release();
                 r->close();
            }

        }
        break;
        default:
            break;
    }
}
/*!
    \brief MRL::ICPCON7017RT::onOneSecond
    \param t
*/
void MRL::ICPCON7017RT::onOneSecond(time_t t) {
    RTObject::onOneSecond(t);
    _secondFlag = true;
}


/*!
    \brief MRL::ICPCON7017RT::updateOpc
    \param server
    \param objectNode
*/
void MRL::ICPCON7017RT::updateOpc(OpcServer &server, Open62541::NodeId &objectNode) {
    RTObject::updateOpc(server, objectNode);
    // update the value
    for (int i = 0; i < NUMBER_7017_CHAN; i++) {
        ScaleOffset &o = _map[_channelNames[i]];
        Open62541::Variant v0(o.current());
        server.writeValue(_valueNodes[i], v0);
    }
    Open62541::Variant st(_nodeOk);
    server.writeValue(_nodeStatus, st);
}


/*!
    \brief MRL::ICPCON7017RT::initialiseOpc
    \param server
    \param objectNode
*/
void MRL::ICPCON7017RT::initialiseOpc(OpcServer &server, Open62541::NodeId &node) {
    // set up the variables
    RTObject::initialiseOpc(server, node);
    for (int i = 0; i < NUMBER_7017_CHAN; i++) {
        server.getChild(node, _channelNames[i], _valueNodes[i]);
    }
    server.getChild(node, "Status", _nodeStatus);
}


/*!
    \brief publishHtmlPage
*/
void MRL::ICPCON7017RT::publishHtmlPage(CTML::Node &s) { // publish the current runtime status as HTML text
    RTObject::publishHtmlPage(s);
    //
    for (int i = 0; i < NUMBER_7017_CHAN; i++) {
        CTML::Node v("p");
        std::stringstream ss;
        ss <<  _channelNames[i] <<  ":"
           << " : "
           << _stats[i].statistics().getMean()
           << " : "
           << _stats[i].statistics().getMaximum()
           << " : "
           << _stats[i].statistics().getMinimum()
           << " : "
           << _stats[i].statistics().getStdDev()
           << std::endl;
        v.SetContent(ss.str());
        s.AppendChild(v);
    }
    //
}

/*!
    \brief measureHtmlPage
*/
void MRL::ICPCON7017RT::measureHtmlPage(CTML::Node &s) {
    RTObject::measureHtmlPage(s);
    for (int i = 0; i < NUMBER_7017_CHAN; i++) {
        std::stringstream ss;
        CTML::Node v("p");
        ss << _channelNames[i] <<  " : " << _stats[i].statistics().getLastValue()  << std::endl;
        v.SetContent(ss.str());
        s.AppendChild(v);
    }
}
