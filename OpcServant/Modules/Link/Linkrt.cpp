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
#include "Linkrt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#include <Common/objectmanager.h>
#include <sstream>

/*
 * This is a UDP based link to other applications. In particular Node Red.
 * Packets are sent and received as JSON
 * Uses the MQTT command handler to avoid dupilcation
 */

/*!
    \brief start
*/
void MRL::LinkRT::start()
{
    //
    RTObject::start();

    _filter = configuration().getAsString("Filter");
    _ident = configuration().getAsString("Ident");
    _port = configuration().getInt("Port");
    _host = configuration().getAsString("Host");
    _hostPort = configuration().getInt("HostPort");
    //
    _remoteAddress.Hostname(_host);
    _remoteAddress.Service(_port);
    _localAddress.AnyAddress();
    _localAddress.Service(_hostPort);
    //
    _datagram.reset();
    _datagram = std::make_unique<wxDatagramSocket>(_localAddress);
    _datagram->SetFlags(wxSOCKET_NOWAIT);
}

/*!
    \brief stop
*/
void MRL::LinkRT::stop()
{
    RTObject::stop();
}


/*!
 * \brief MRL::LinkRT::onOneSecond
 * \param t
 * fetch each input
 */
void MRL::LinkRT::onOneSecond(time_t t)
{
    RTObject::onOneSecond(t);
}

/*!
 * \brief MRL::LinkRT::process
 */
void MRL::LinkRT::process()
{
    RTObject::process();

    if(_datagram)
    {

        char buf[2000];
        int n = 0;
        while((n = _datagram->Read(buf, sizeof(buf)  - 1).LastReadCount()) > 0) // receive
        {
            buf[n] = 0;
            //
            // process the received message
            // should be a JSON packet
            // TBD use the MQTT command handler
            Json::Value v;
            if(stringToJson(buf,v))
            {
                Json::Value &op = v["action"];
                std::string o = op.asString();
                MqttCommand *c = MqttConnection::find(o);
                if(c)
                {
                    c->action(_dummy,_ident,v);
                }
            }

        }

        if(_queue.size() > 0) // send
        {
            while(_queue.size())
            {
                std::string &s = _queue.front();
                _datagram->SendTo(_remoteAddress,s.c_str(),s.size());
                _queue.pop();
            }
        }
    }

}


/*!
 * \brief MRL::LinkRT::publishQueueItem
 * \param msg
 */
void MRL::LinkRT::publishQueueItem(const MRL::Message &msg) {
    // publish the message
    Message &m = const_cast<Message &>(msg);
    switch (m.id()) {

    case MESSAGEID::Publish_Object: {
        if (enabled()) {
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
                            std::string alias = MRL::Common::findReverseAlias(src);
                            // does this map onto an alias? - if so translate
                            if(!alias.empty())
                            {
                                src = alias;
                            }
                            //
                            Json::Value jv;
                            std::string item =  "/" + _ident + "/" + src;
                            jv["Ident"]  = item;
                            jv["Value"] = v;
                            jv["Id"] = id;
                            //
                            std::string ss;
                            jsonToString(jv,ss);
                            //
                            _queue.push(ss);
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


