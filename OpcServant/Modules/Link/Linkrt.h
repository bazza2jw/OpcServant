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

namespace MRL {
    // runtime Link object
/*!
     * \brief The LinkRT class
     */
    class LinkRT : public RTObject
    {
        std::string _filter = "*"; // filter on source path
        std::string _port = "1883"; // server port
        std::string _ident = "OPCSERVANT";
        std::string _host = "localhost";
        //
        PropertyPath _path; // where in the models the object data goes
        std::queue<std::string> _queue;
        //

    public:
        LinkRT(int id) : RTObject(id)
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
