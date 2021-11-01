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
#include "mqttconnection.h"
#include <MrlLib/json/json.h>

MRL::MqttCommandMap MRL::MqttConnection::_map; // command map

/*!
 * \brief MRL::MqttCommand::MqttCommand
 */
MRL::MqttCommand::MqttCommand(const std::string &s)
{
    // self register
    MRL::MqttConnection::addCommand(s,this);
}

/*!
 * \brief MRL::MqttConnection::MqttConnection
 */
MRL::MqttConnection::MqttConnection( const char *id)
    : mosqpp::mosquittopp(id)
{

}
/*!
 * \brief MRL::MqttConnection::~MqttConnection
 */
MRL::MqttConnection::~MqttConnection()
{

}
/*!
 * \brief MRL::MqttConnection::on_connect
 * \param rc
 */
void MRL::MqttConnection::on_connect(int rc)
{
    //wxLogDebug("Connected with code %d.\n", rc);
    _ok = (rc== 0);
}
/*!
 * \brief MRL::MqttConnection::on_message
 * \param message
 */
void MRL::MqttConnection::on_message(const struct mosquitto_message *message)
{
    // received a message
    Json::Value v;
    std::string payload( static_cast<const char *>(message->payload),message->payloadlen);
    std::string topic(message->topic);
    if(MRL::stringToJson(payload,v))
    {
        Json::Value &op = v["action"];
        if(!op.isNull())
        {
            // find the command an execute it
            std::string o = op.asString();
            MqttCommand *c = find(o);
            if(c)
            {
                c->action(*this,topic,v);
            }
        }
        else
        {
            // no op defined
            defaultParse(v);
        }
    }
    else
    {
        // not JSON so something else
        defaultParse(payload);
    }
}
/*!
 * \brief MRL::MqttConnection::on_subscribe
 * \param mid
 * \param qos_count
 * \param granted_qos
 */
void MRL::MqttConnection::on_subscribe(int /*mid*/, int /*qos_count*/, const int */*granted_qos*/)
{
   // wxLogDebug("Subscription succeeded.\n");
}

