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
#ifndef BME280RT_H
#define BME280RT_H
#include <Common/Daq/rtobject.h>
#include <MrlLib/stats.hpp>
#include <MrlLib/rollingbuffer.hpp>
#include <MrlLib/i2cinterface.h>
namespace MRL {
    // runtime BME280 object
/*!
     * \brief The BME280RT class
     */
    class BME280RT : public RTObject
    {
        std::string _i2cAddress = I2C_DEF_DEV;
        int _i2cType = 0;
        int _i2cPort = 5000;
        MRL::I2cInterfaceBase::Ptr _i2c;
        //

        struct bme280_calib_data {
            /**
             * @ Trim Variables
             */

            /**@{*/
            uint16_t dig_T1;
            int16_t dig_T2;
            int16_t dig_T3;
            uint16_t dig_P1;
            int16_t dig_P2;
            int16_t dig_P3;
            int16_t dig_P4;
            int16_t dig_P5;
            int16_t dig_P6;
            int16_t dig_P7;
            int16_t dig_P8;
            int16_t dig_P9;
            uint8_t dig_H1;
            int16_t dig_H2;
            uint8_t dig_H3;
            int16_t dig_H4;
            int16_t dig_H5;
            int8_t dig_H6;
            int32_t t_fine;

            /**@}*/
        };

        bme280_calib_data calib_data;
        double _temperature = 20;
        double _pressure = 100000;
        double _humidity = 100;
        bool triggered = false;
        int _bmeAddr = 0x76;
        int _state = 0;

        enum
        {
            STATE_IDLE = 0,
            STATE_FAULT,
            STATE_RESET,
            STATE_READ_CALIB,
            STATE_TRIGGER,
            STATE_READ

        };
        PropertyPath _path; // where in the models the object data goes
        static StringVector _inputs;
        Open62541::NodeId _temperatureNode;
        Open62541::NodeId _pressureNode;
        Open62541::NodeId _humidityNode;

        bool _ok = false;

    public:
        BME280RT(int id) : RTObject(id)
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


        double temperature() {
            return _temperature;
        }
        double pressure() {
           return  _pressure;
        }
        double humidity() {
            return _humidity;
        }

    private:
        double compensate_humidity(double H);
        double compensate_temperature(double T);
        double compensate_pressure(double P);
        void readBmeCal();
        void readBme280(double &t, double &p, double &h);
        void initialiseBme280();
        void trigger();

    };
}
#endif // BME280RT_H
