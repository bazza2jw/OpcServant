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
#ifndef SIMULATORRT_H
#define SIMULATORRT_H
//
#include "simulator.h"
#include <Common/Daq/rtobject.h>
//
namespace MRL {
    // runtime simulator object
/*!
     * \brief The SimulatorRT class
     */
    class SimulatorRT : public RTObject
    {
        enum
        {
            Random, Ramp
        };
        PropertyPath _path; // where in the models the object data goes
        int _range = 100;
        int _type = 0;
        int _lastValue = 0;
        bool _dirDown = false; // direction for ramp
        bool _output = false;
        //
        // Cache the NodeIds
        Open62541::NodeId _rangeNode;
        Open62541::NodeId _typeNode;
        Open62541::NodeId _valueNode;
        Open62541::NodeId _outputNode;

        static StringVector _inputs;
        static StringVector _outputs;
    public:
        SimulatorRT(int id) : RTObject(id)
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
         * \brief publishHtmlPage
         */
        virtual void publishHtmlPage(CTML::Node &s); // publish the current runtime status as HTML text

        /*!
            \brief processQueueItem
        */
        virtual bool processQueueItem(const Message &);

        /*!
         * \brief measureHtmlPage
         */
        virtual void measureHtmlPage(CTML::Node &s);

        /*!
         * \brief hasInputs
         * \return true if has inputs
         */
        virtual bool hasInputs() const { return true;}
        /*!
         * \brief hasOutputs
         * \return
         */
        virtual bool hasOutputs() const { return true;}

        /*!
         * \brief inputs
         * \return
         */
        virtual StringVector & inputs() { return _inputs;}
        virtual StringVector & outputs() { return _outputs;}
        /*!
         * \brief useProcessTimer
         * \return
         */
        virtual bool useProcessTimer() {
            return true;   // used to drive state machines
        }


    };
}
#endif // SIMULATORRT_H
