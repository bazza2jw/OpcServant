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
#include "alarmrt.h"
#include <MrlLib/mrllib.h>
#include <Common/Opc/opcserver.h>
#include <Common/messageids.h>
#include <Common/Daq/daqcommon.h>
MRL::StringVector MRL::AlarmRT::_outputs = {"State"};

/*!
    \brief start
*/
void MRL::AlarmRT::start() {
    RTObject::start();
    //
    // Get the settings
    // get the object ids
    _alarmObject = Common::configuration().find(configuration().getValue<std::string>("AlarmObject"));
    _alertObject =  Common::configuration().find(configuration().getValue<std::string>("AlertObject"));
    _okObject = Common::configuration().find(configuration().getValue<std::string>("OkObject"));
    //
    // Outputs to use
    _alarmOutput = configuration().getValue<std::string>("AlarmOutput");
    _alertOutput = configuration().getValue<std::string>("AlertOutput");
    _okOutput  = configuration().getValue<std::string>("OkOutput");
    //
    // get eh list of value objects in the group
    std::string sa = configuration().getValue<std::string>("Selection");
    wxArrayString sl =  wxStringTokenize(sa, ",");
    //
    for (size_t i = 0; i < sl.Count(); i++) {
        unsigned id = 0;
        if (sl[i].ToULong((unsigned long *)&id)) {
            _objects.push_back(id);
        }
    }
}

/*!
    \brief stop
*/
void MRL::AlarmRT::stop() {
    RTObject::stop();
}

void MRL::AlarmRT::messageObject(unsigned id, const std::string &ip,  bool f) {
    RtObjectRef &r = Common::daq().objects()[id];
    if (r) {
        Message m(MESSAGEID::SetOutput);
        m.data().add(PARAMETERID::Index, ip);
        m.data().add(PARAMETERID::Value, f);
        r->queueItem(m);
    }
}

/*!
    \brief measure
*/
void MRL::AlarmRT::measure() {
    //
    int nextState = 0;
    for (auto i = _objects.begin(); i != _objects.end(); i++) {
        RtObjectRef &r = Common::daq().objects()[*i];
        if (r) {
            int l = r->runtime().getValue<int>("State");
            if (l > nextState) {
                nextState = l; // look for highest state
            }
        }
    }

    RTObject::measure();
    if (nextState != _state) {
        _state = nextState;
        updateValue(_state,"Status",_state);
        _publishFlag = true;
    }
    //
    // now trigger outputs
    bool action = false;
    bool alert = false;
    bool ok = false;
    //
    switch (_state) {
        case STATES::States::StateAction:
            action = true;
            break;
        case STATES::States::StateAlert:
            alert = true;
            break;
        case STATES::States::StateOk:
            ok = true;
            break;
        default:
            break;

    }
    //
    messageObject(_alarmObject,_alarmOutput, action);
    messageObject(_alertObject,_alertOutput, alert);
    messageObject(_okObject,_okOutput, ok);
    //
    updateObject();
}

/*!
    \brief publish
*/
void MRL::AlarmRT::publish() {
    RTObject::publish();
    // emit a publish message
    if (_publishFlag) {
        //
        Message m(MESSAGEID::Publish_Object,dbId());
        // say we are posting an update
        toUpdateValue(m, (double)_state, "Status", _state);
        // now post the value
        message().emit(m);
        //
    }
    _publishFlag = false;
}


/*!
    \brief MRL::AlarmRT::updateOpc
    \param server
    \param objectNode
*/
void MRL::AlarmRT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/) {
    // update the value
    Open62541::Variant v(_state);
    // find the child node called Value
    server.writeValue(_stateNode, v);
}


/*!
    \brief MRL::AlarmRT::initialiseOpc
    \param server
    \param objectNode
*/
void MRL::AlarmRT::initialiseOpc(OpcServer &server, Open62541::NodeId &node) {
    server.getChild(node, "State", _stateNode);
}


/*!
    \brief measureHtmlPage
*/
void MRL::AlarmRT::measureHtmlPage(CTML::Node &s) {
    RTObject::measureHtmlPage(s);
    std::stringstream ss;
    CTML::Node v("p");
    ss << "State " << _state;
    v.SetContent(ss.str());
    s.AppendChild(v);
}
