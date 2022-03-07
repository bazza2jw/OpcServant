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
#ifndef MQTTRT_H
#define MQTTRT_H

#include <mosquittopp.h>
#include <Common/Daq/rtobject.h>
#include <Common/MQTT/mqttconnection.h>
namespace MRL
{

/*!
     * \brief The MQTTRT class
     */
    class MQTTRt : public RTObject {
        std::string _filter = "*"; // filter on source path
        int  _port = 1883; // server port
        std::string _ident = "OPCSERVANT"; // this bridges ident to aid NODE RED processing
        std::string _host = "localhost"; // MQTT broker hostaddress
        std::string _username = "user";
        std::string _password = "password";
        std::string _command = "";
        MqttConnectionRef _mqtt;

    public:
        /*!
         * \brief MQTTRT
         * \param id
         */
        MQTTRt(unsigned id);
        /*!
            \brief start
        */
        virtual void start();
        /*!
            \brief stop
        */
        virtual void stop();
        /*!
            \brief publishQueueItem
            \param msg
            \return
        */
        void  publishQueueItem(const Message &msg);

        virtual bool useProcessTimer() {
            return true;   // used to drive state machines
        }
        /*!
            \brief process
        */
        virtual void process();
        /*!
            \brief onOneSecond
            \param t
        */
        virtual void onOneSecond(time_t t);
    };

    /*!
     * \brief The MqttChan class
     */
    class MqttChan : public MqttConnection
    {
        MQTTRt &_parent;
        std::string _commandSubscription = ""; // the subscription to use to receive commands on
      public:
        MqttChan(MQTTRt &p, const char *id) : MqttConnection(id),_parent(p) {}
        void setCommandSubscription(const std::string &s) {_commandSubscription = s;}
        void on_connect(int rc)
        {
           MqttConnection::on_connect(rc) ;
           if(ok() && !_commandSubscription.empty())
           {
               subscribe(nullptr,_commandSubscription.c_str());
           }
        }
    };
}
#endif // MQTTRT_H
