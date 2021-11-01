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
#ifndef Mics6814RT_H
#define Mics6814RT_H
#include <Common/Daq/rtobject.h>
#include <MrlLib/stats.hpp>
#include <MrlLib/rollingbuffer.hpp>
#include <MrlLib/i2cinterface.h>
#include "../Common/ads1015.h"

namespace MRL {
    // runtime Mics6814 object
/*!
     * \brief The Mics6814RT class
     */
    class Mics6814RT : public RTObject
    {
        std::string _i2cAddress = I2C_DEF_DEV;
        int _i2cType = 0;
        int _i2cPort = 5000;
        MRL::I2cInterfaceBase::Ptr _i2c;
        Adafruit_ADS1015 _adc;
        //
        int _state = 0;
        //
        double _ox  = 0.0;
        double _red = 0.0;
        double _nh3 = 0.0;
        //
        enum
        {
            STATE_IDLE = 0,
            STATE_START,
            STATE_MEASURE,
            STATE_FAULT
        };
        //
        PropertyPath _path; // where in the models the object data goes
        static StringVector _inputs;
        //
        Open62541::NodeId _oxNode;
        Open62541::NodeId _redNode;
        Open62541::NodeId _nh3Node;
        //
    public:
        Mics6814RT(int id) : RTObject(id)
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
#endif // Mics6814RT_H
