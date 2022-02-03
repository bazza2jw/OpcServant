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
#ifndef VisualCalcRT_H
#define VisualCalcRT_H
#include <Common/Daq/rtobject.h>
#include <open62541cpp/open62541server.h>
#include <open62541cpp/serverbrowser.h>
#include <open62541cpp/servernodetree.h>

namespace MRL {
    // runtime VisualCalc object
/*!
     * \brief The VisualCalcRT class
     */
    class VisualCalcRT : public RTObject
    {
        //
        int _state = 0;
        //
        //
        enum
        {
            STATE_IDLE = 0,
            STATE_START,
            STATE_FAULT
        };
        //
        std::unique_ptr<Open62541::ServerNodeTree> _tree; // opc node tree for child processes to use
        PropertyPath _path; // where in the models the object data goes
        StringVector _inputs;
        //
        //
    public:
        VisualCalcRT(int id) : RTObject(id)
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
         * \brief process
         */
        void process();
        /*!
         * \brief useProcessTimer
         * \return
         */
        virtual bool useProcessTimer() {
            return true;   // used to drive state machines
        }
        /*!
          * \brief processQueueItem
          * \return true if processed
          */
        virtual bool processQueueItem(const Message &);
        /*!
         * \brief onOneSecond
         * \param t
         */
        void onOneSecond(time_t t);
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
         * \brief measureHtmlPage
         */
        virtual void measureHtmlPage(CTML::Node &s);

        /*!
         * \brief hasInputs
         * \return true if has inputs
         */
        virtual bool hasInputs() const { return true;}
        /*!
         * \brief inputs
         * \return
         */
        virtual StringVector & inputs() { return _inputs;}
    private:

    };
}
#endif // VisualCalcRT_H
