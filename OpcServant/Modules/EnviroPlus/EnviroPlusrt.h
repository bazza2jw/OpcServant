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
#ifndef EnviroPlusRT_H
#define EnviroPlusRT_H
#include <Common/Daq/rtobject.h>
#include <MrlLib/stats.hpp>
#include <MrlLib/rollingbuffer.hpp>
#include <MrlLib/i2cinterface.h>
#include "../Common/ads1015.h"
namespace MRL {
    // runtime EnviroPlus object
/*!
     * \brief The EnviroPlusRT class
     */
    class EnviroPlusRT : public RTObject
    {
        std::string _i2cAddress = I2C_DEF_DEV;
        int _i2cType = 0;
        int _i2cPort = 5000;
        MRL::I2cInterfaceBase::Ptr _i2c;
        //
        double _no = 0.0;
        double _nh3 = 0.0;
        double _co = 0.0;
        double _light = 0.0;
        double _noise = 0.0;
        //
        // there is a BME 280
        //            LTR 559
        //            ADS 1015 (or 115)
        // these are all I2C
        //
        static constexpr int _bmeAddr = 0x76;
        static constexpr int _adcAddr = 0x44;
        static constexpr int _ltrAddr = 0x46;
        //
        int _state = 0;
        std::unique_ptr<Adafruit_ADS1015> _adc;
        enum
        {
            STATE_IDLE = 0,
            STATE_FAULT,
            STATE_START,
            STATE_BME280, // measure BME 280 - NB not used - another module deals with the BME280
            STATE_LTR, // read the light level
            STATE_GASES, // read the gases
            STATE_NOISE // read the noise
        };
        PropertyPath _path; // where in the models the object data goes
        static StringVector _inputs;

    public:
        EnviroPlusRT(int id) : RTObject(id)
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
#endif // EnviroPlusRT_H
