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
#ifndef ALARMRT_H
#define ALARMRT_H
#include <Common/Daq/rtobject.h>
#include <MrlLib/stats.hpp>
#include <MrlLib/rollingbuffer.hpp>
namespace MRL {
    // runtime simulator object
/*!
     * \brief The AlarmRT class
     */
    class AlarmRT : public RTObject
    {
        PropertyPath _path; // where in the models the object data goes
        Open62541::NodeId _stateNode;
        int _state = 0;
        static StringVector _outputs;
        std::list<unsigned> _objects; // list of objects in alarm group
        //
        // object references
        unsigned _alarmObject = 0;
        unsigned _alertObject = 0;
        unsigned _okObject = 0;
        //
        // Outputs to use
        std::string _alarmOutput;
        std::string _alertOutput;
        std::string _okOutput;
        //
        bool _publishFlag = false;

    public:
        AlarmRT(int id) : RTObject(id)
        {

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
            \brief publish
        */
        virtual void publish();
        /*!
            \brief measure
        */
        virtual void measure();
        /*!
         * \brief updateOpc
         * \param server
         * \param objectNode
         */
        virtual void updateOpc(OpcServer &server, Open62541::NodeId &objectNode);
        /*!
         * \brief initialiseOpc
         * \param server
         * \param objectNode
         */
        virtual void initialiseOpc(OpcServer &server, Open62541::NodeId &objectNode);

        /*!
         * \brief measureHtmlPage
         */
        virtual void measureHtmlPage(CTML::Node &s);

        /*!
         * \brief messageObject
         * \param id
         * \param f
         */
        void messageObject(unsigned id, const std::string &ip, bool f);

    };
}

#endif // ALARMRT_H
