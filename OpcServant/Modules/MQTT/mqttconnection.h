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
#ifndef MQTTCONNECTION_H
#define MQTTCONNECTION_H
#include <mosquittopp.h>
#include <Common/Daq/rtobject.h>

namespace MRL
{
/*!
     * \brief The MqttConnection class
     */
    class MqttConnection : public mosqpp::mosquittopp
    {


            bool _ok = false;
        public:
        /*!
             * \brief MqttConnection
             * \param id
             * \param host
             * \param port
             */
            MqttConnection(const char *id);
            /*!
              */
            ~MqttConnection();
            /*!
             * \brief on_connect
             * \param rc
             */
            void on_connect(int rc);
            /*!
             * \brief on_message
             * \param message
             */
            void on_message(const struct mosquitto_message *message);
            /*!
             * \brief on_subscribe
             * \param mid
             * \param qos_count
             * \param granted_qos
             */
            void on_subscribe(int mid, int qos_count, const int *granted_qos);

            bool ok() const {return _ok;}
    };

    typedef std::unique_ptr<MqttConnection> MqttConnectionRef;
}

#endif // MQTTCONNECTION_H
