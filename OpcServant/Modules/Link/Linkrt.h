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
#ifndef LinkRT_H
#define LinkRT_H
#include <Common/Daq/rtobject.h>
#include <wx/socket.h>
#include <Common/MQTT/mqttconnection.h>


namespace MRL {
    // runtime Link object
/*!
     * \brief The LinkRT class
     */
    class LinkRT : public RTObject
    {
        MqttConnection _dummy;
        std::string _filter = "*"; // filter on source path
        std::string _port = "10000"; // server port
        std::string _ident = "OPCSERVANT";
        std::string _host = "localhost";
        //
        PropertyPath _path; // where in the models the object data goes
        std::queue<std::string> _queue;
        //
        std::unique_ptr<wxDatagramSocket> _socket;
        wxIPV4address _addr;
        //
        virtual bool useProcessTimer() {
            return true;   // used to drive state machines
        }

    public:
        LinkRT(int id) : RTObject(id),_dummy("DUMMY")
        {
            Daq::instance()->publishMessage().connect(this, &LinkRT::publishQueueItem);
        }

        /*!
            \brief start
        */
        virtual void start();
        /*!
            \brief stop
        */
        virtual void stop();
        /*!
         * \brief publishQueueItem
         * \param msg
         */
        void publishQueueItem(const Message &msg);

        void process();
        /*!
         * \brief onOneSecond
         * \param t
         */
        void onOneSecond(time_t t);

        /*!
         * \brief hasInputs
         * \return true if has inputs
         */
        virtual bool hasInputs() const { return false;}
    private:

    };
}
#endif // LinkRT_H
