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
#ifndef ICPCON7017_H
#define ICPCON7017_H
#include "../icpcon_global.h"
#include "../icpcon.h"
#include <Common/Daq/rtobject.h>
#include <MrlLib/stats.hpp>
#include <MrlLib/rollingbuffer.hpp>
#include <Common/Daq/serialpacket.h>
#include <MrlLib/scaleoffset.h>

namespace MRL {
    // runtime ICPCON7017 object
    constexpr int NUMBER_7017_CHAN= 7;
    constexpr int TIMEOUT_7017 = 10;
    //
/*!
     * \brief The ICPCON7017RT class
     */
    class ICPCON7017RT : public RTObject
    {

        PropertyPath _path; // where in the models the object data goes
        int _state = 0;
        SerialPacket _packet;
        //
        std::string _port = "/dev/ttyUSB0";
        int _baudRate = 56700;
        int _node = 1;
        int _errorWait = 10;
        wxStopWatch _timer;
        //
        // The pre-built packets
        std::string getConfig;
        std::string moduleList;
        std::string resetCheck;
        std::string getData;
        std::string clearReset;
        std::string enableAll;
        //
        ScaleOffsetMap _map;
        //
        StatisticsBuffer _stats[NUMBER_7017_CHAN];
        Open62541::NodeId _valueNodes[NUMBER_7017_CHAN];
        Open62541::NodeId _nodeStatus;
        bool _nodeOk = true;
        //
        bool _secondFlag = false;
        bool _publishRecord = false;
        enum
        {
          STATE_IDLE = 0,
          STATE_ID, // identify the module
          STATE_INIT, // send the initialisation string
          STATE_SEND, // send the channel read request
          STATE_RECEIVE, // wait for response
          STATE_ERROR,
        };


    public:
        /*!
         * \brief ICPCON7017RT
         * \param id
         */
        ICPCON7017RT(int id) : RTObject(id)
        {

        }

        /*!
         * \brief useProcessTimer
         * \return true
         */
        virtual bool useProcessTimer() {
            return true;   // used to drive state machines
        }
        /*!
         * \brief makePackets
         */
        void makePackets()
        {
            moduleList = ICPCON::makePacket(_node,"$%02XM"); // get the module ID
            getData = ICPCON::makePacket(_node,"#%02X");
            resetCheck = ICPCON::makePacket(_node,"~%02X0");
            clearReset = ICPCON::makePacket(_node,"~%02X1"); // reset the watchdog
            enableAll = ICPCON::makePacket(_node,"$%02X5FF"); // enable all channels
        }
        /*!;
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
        virtual void process();
        /*!
         * \brief onOneSecond
         * \param t
         */
        virtual void onOneSecond(time_t t);

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
        static StringVector _channelNames;
        virtual StringVector & inputs() { return _channelNames;}
        /*!
         * \brief setFault
         * \param s
         */
        void setFault(const std::string &s)
        {
            runtime().setValue("FaultReason",s);
            runtime().setValue("Fault",true);
            _errorWait = TIMEOUT_7017 * 1000;
            _state = STATE_ERROR;
             _nodeOk = false;
             _secondFlag = false;
            _timer.Start();
        }

        /*!
         * \brief clearFault
         * \param s
         */
        void clearFault()
        {
            runtime().setValue("FaultReason",std::string("OK"));
            runtime().setValue("Fault",false);
            _nodeOk = true;
            _secondFlag = false;
        }
    };
}
#endif // ICPCON7017_H
