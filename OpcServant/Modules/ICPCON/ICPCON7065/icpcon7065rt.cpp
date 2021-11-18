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
#include "icpcon7065rt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#include <Common/Daq/daqcommon.h>
#include "icpcon7065nodecontext.h"

MRL::StringVector MRL::ICPCON7065RT::_channelNames = {"OUT0", "OUT1", "OUT2", "OUT3", "OUT4"};

/*!
    \brief start
*/
void MRL::ICPCON7065RT::start() {
    RTObject::start();
    // get the configuration
    // get port
    _port =  configuration().getValue<std::string>("SerialPort");
    // get node id
    _node = configuration().getValue<int>("NodeAddress");
    //Baud Rate
    _baudRate =  MRL::stringToNumber<int>(configuration().getValue<std::string>("BaudRate"));
    //
    makePackets();
    _state = STATE_ID;
}

/*!
    \brief stop
*/
void MRL::ICPCON7065RT::stop() {
    RTObject::stop();
    _state = STATE_IDLE;
}

/*!
    \brief publish
*/
void MRL::ICPCON7065RT::publish() {
    RTObject::publish();
    for (size_t i = 0; i < _channelNames.size(); i++) {
        publishValue(_outputs[i], _channelNames[i], _nodeOk ? STATES::States::StateOk : STATES::States::StateAction);
    }
}

/*!
    \brief measure
*/
void MRL::ICPCON7065RT::measure() {
    RTObject::measure();
    for (size_t i = 0; i < _channelNames.size(); i++) {
        updateValue(_outputs[i], _channelNames[i]);
    }
    updateObject(_nodeOk ? STATES::States::StateOk : STATES::States::StateAction);
}
/*!
 * \brief MRL::ICPCON7065RT::process
 */
void MRL::ICPCON7065RT::process() {
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
                    // is it 7065?
                    if ((m.length() > 6) && (m.substr(3, 4) == "7065")) {
                        _state = STATE_SEND;
                    }
                    else {
                        setFault("Not a 7065");
                    }
                    r->close();
                }
                else {
                    setFault("Serial 7065");
                }
                r->release();
            }
        }
        break;
        case STATE_SEND: { // send the channel read request
            SerialPacketRef &r = MRL::Common::daq().serial(_port);
            if (_secondFlag && r->claim()) {
                if (r->openSerial(_port, _baudRate, SC_8DATA, SC_1STOP, SC_NOPARITY)) {
                    //
                    unsigned v = 0;
                    int j = 1;
                    for (int i = 0; i < NUMBER_7065_CHAN; i++) {
                        if (_outputs[i]) v |= j;
                        j <<= 1;
                    }
                    //
                    char b[32];
                    sprintf(b, "@%%02X%02X", v); // build command
                    std::string s = ICPCON::makePacket(_node, b);
                    r->startPacket(s); // update
                    if (v != _lastOut) {
                        _lastOut = v;
                        publish(); // propogate changes to the
                        measure();
                    }
                    _state = STATE_RECEIVE;
                }
                else
                {
                    setFault("7065 Open Fail");
                    r->release();
                }
                _secondFlag = false;
            }
            // get lock on serial port
            // send request
        }
        break;
        case STATE_RECEIVE: {
            SerialPacketRef &r = MRL::Common::daq().serial(_port);
            if (r->packetDrive()) { // drive the packet
                // process packets
                r->close();
                r->release();
                _state = STATE_SEND;

            }
            else if (r->timedOut()) {
                setFault("7065 Serial Timed Out");
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
    \brief MRL::ICPCON7065RT::onOneSecond
    \param t
*/
void MRL::ICPCON7065RT::onOneSecond(time_t t) {
    RTObject::onOneSecond(t);
    _secondFlag = true;
}


/*!
    \brief MRL::ICPCON7065RT::updateOpc
    \param server
    \param objectNode
*/
void MRL::ICPCON7065RT::updateOpc(OpcServer &server, Open62541::NodeId &objectNode) {
    RTObject::updateOpc(server, objectNode);
    // update the value
    for (int i = 0; i < NUMBER_7065_CHAN; i++) {
        Open62541::Variant v0(_outputs[i]);
        server.writeValue(_valueNodes[i], v0);
    }
    Open62541::Variant st(_nodeOk);
    server.writeValue(_nodeStatus, st);
}


/*!
    \brief MRL::ICPCON7065RT::initialiseOpc
    \param server
    \param objectNode
*/
void MRL::ICPCON7065RT::initialiseOpc(OpcServer &server, Open62541::NodeId &node) {
    // set up the variables
    RTObject::initialiseOpc(server, node);
    for (int i = 0; i < NUMBER_7065_CHAN; i++) {
        server.getChild(node, _channelNames[i], _valueNodes[i]);
        ICPCON7065NodeContext *c = new ICPCON7065NodeContext(dbId(), i);
        server.setNodeContext(_valueNodes[i], c); // set the read/write driver
        c->setAsDataSource(server, _valueNodes[i]);
    }
    server.getChild(node, "Status", _nodeStatus);
}


/*!
    \brief publishHtmlPage
*/
void MRL::ICPCON7065RT::publishHtmlPage(CTML::Node &s) { // publish the current runtime status as HTML text
    RTObject::publishHtmlPage(s);
}

/*!
    \brief measureHtmlPage
*/
void MRL::ICPCON7065RT::measureHtmlPage(CTML::Node &s) {
    RTObject::measureHtmlPage(s);
    for (int i = 0; i < NUMBER_7065_CHAN; i++) {
        std::stringstream ss;
        CTML::Node v("p");
        ss << _channelNames[i] <<  " : " << (_outputs[i] ? "ON" : "OFF")  << std::endl;
        v.SetContent(ss.str());
        s.AppendChild(v);
    }
}


/*!
    \brief processQueueItem
*/
bool MRL::ICPCON7065RT::processQueueItem(const Message &msg) {
    Message &m = const_cast<Message &>(msg);
    if (!RTObject::processQueueItem(m)) {
        switch (m.id()) {
            case MESSAGEID::SetOutput: {
                bool v;
                std::string i;
                m.data().get(PARAMETERID::Index, i);
                m.data().get(PARAMETERID::Value, v);
                //
                if ((i.length() > 3) && !strncmp(i.c_str(), "OUT", 3)) { // validate
                    int j = int(i[3]) - '0';
                    //
                    if ((j >= 0) && (j < NUMBER_7065_CHAN)) {
                        _outputs[j] = v;
                        publish();
                    }
                }
            }
            break;
            default:
                break;
        }
        return false;
    }
    return true; // return true if processed
}


