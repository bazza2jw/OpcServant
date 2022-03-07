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
#include "mqttrt.h"
#include <Common/Daq/daqcommon.h>
#include <Common/messageids.h>
#include <Common/stockobjectids.h>
#include <MrlLib/variantpropertytree.h>
#include <Common/objectmanager.h>

/*!
    \brief MQTTRt
    \param id
*/
MRL::MQTTRt::MQTTRt(unsigned id) : RTObject(id) {
    Daq::instance()->publishMessage().connect(this, &MQTTRt::publishQueueItem);
}
/*!
    \brief start
*/
void MRL::MQTTRt::start() {
    RTObject::start();
    // get the configuration
    //
    _filter = configuration().getAsString("Filter");
    _ident = configuration().getAsString("Ident");
    _port = configuration().getValue<int>("Port");
    _host = configuration().getAsString("Host");
    _username = configuration().getAsString("Username");
    _password = configuration().getAsString("Password");
    _command = configuration().getAsString("Command");
    //
    bool ei = configuration().getValue<bool>("Enabled");
    setEnabled(ei);
    //
    if (enabled()) {
        // start the server
        auto mp = new MqttChan(*this, _ident.c_str());
        mp->setCommandSubscription(_command);
        _mqtt.reset(mp);
        _mqtt->username_pw_set(_username.c_str(), _password.c_str()); // set access
        int rc = _mqtt->connect(_host.c_str(), _port); // connect
        wxLogDebug("MQ Connect %d",rc);

    }
}
/*!
    \brief stop
*/
void MRL::MQTTRt::stop() {
    RTObject::stop();
    if (_mqtt) _mqtt->disconnect();
    _mqtt.reset(); // delete
}
/*!
    \brief MRL::MQTTRt::onOneSecond
    \param t
*/
void  MRL::MQTTRt::onOneSecond(time_t t) {
    RTObject::onOneSecond(t);
    if (started() && enabled()) {

    }

}


/*!
    \brief MRL::MQTTRt::process
*/
void MRL::MQTTRt::process() {
    RTObject::process();
    if (started() && enabled() && _mqtt) {
        // process any waiting input commands
        int res = _mqtt->loop(0);     // Keep MQTT connection
        if (res) _mqtt->reconnect(); // reconnect if necessary
    }
}

/*!
    \brief publishQueueItem
    \param msg
    \return
*/
void  MRL::MQTTRt::publishQueueItem(const Message &msg) {
    // publish the message
    Message &m = const_cast<Message &>(msg);
    switch (m.id()) {
        case MESSAGEID::Publish_Record: {
            if (enabled() && _mqtt) {
                // unpack the message
                unsigned id = 0;
                std::string v; // the record to publish
                //
                m.data().get(PARAMETERID::ObjectId, id); // set the source of the message
                m.data().asString(PARAMETERID::Record, v);
                //
                // get source as string
                //
                // get object type as string
                DataNodeIdMap &mm = Common::configuration().idMap();
                DataNode *d  = mm[id];
                if (d) {
                    auto o =  ObjectManager::find(d->data().type());
                    if (o) {
                        std::string src;
                        //
                        RtObjectRef &j = MRL::Common::daq().objects()[id];
                        j->path().toString(src);
                        // filter - either empty string or look for string within source name
                        if ((_filter.length() < 1) || (_filter == "*") || (src.find(_filter) != std::string::npos)) {
                            //
                            //Build key string
                            std::string item = "/" + _ident + "/" + src;
                            int rc = _mqtt->publish(NULL, item.c_str(), v.length(), v.c_str());
                            if(rc)
                            {
                                wxLogDebug("Publish Fails");
                            }
                        }
                    }
                }
            }
        }
        break;

        case MESSAGEID::Publish_Object: {
            if (enabled() && _mqtt) {
                // unpack the message
                unsigned id = 0;
                int state = 0;
                time_t ts = 0;
                std::string v;
                std::string tag;
                //
                m.data().get(PARAMETERID::ObjectId, id); // set the source of the message
                m.data().asString(PARAMETERID::Value, v);
                m.data().get(PARAMETERID::Tag, tag);
                m.data().get(PARAMETERID::Status, state);
                m.data().get(PARAMETERID::Timestamp, ts);
                //
                // get source as string
                //
                // get object type as string
                DataNodeIdMap &mm = Common::configuration().idMap();
                DataNode *d  = mm[id];
                if (d) {
                    auto o =  ObjectManager::find(d->data().type());
                    if (o) {
                        std::string src;
                        //
                        RtObjectRef &j = MRL::Common::daq().objects()[id];
                        if(j)
                        {
                            j->path().toString(src);
                            // add the tag
                            if (!tag.empty()) {
                                src += TAG_SEPERATOR;
                                src += tag;
                            }
                            // filter - either empty string or look for string within source name
                            if ((_filter.length() < 1) || (_filter == "*") || (src.find(_filter) != std::string::npos)) {
                                //
                                //Build key string
                                Json::Value vs;
                                vs["Value"] = v;
                                vs["Tag"] = tag;
                                vs["objectId"] = id;
                                //
                                std::string alias = MRL::Common::findReverseAlias(src);
                                //const std::string &alias = am[src];
                                // does this map onto an alias? - if so translate
                                if(!alias.empty())
                                {
                                    src = alias;
                                }
                                //
                                std::string js;
                                MRL::jsonToString(vs, js);
                            }
                        }
                    }
                }
            }
        }
        break;
        default:
            break;
    }
}


