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
#ifndef I2CADCRT_H
#define I2CADCRT_H
#include <Common/Daq/rtobject.h>
#include <MrlLib/stats.hpp>
#include <MrlLib/rollingbuffer.hpp>
#include <MrlLib/i2cinterface.h>
namespace MRL {
    // runtime I2CADC object
/*!
     * \brief The I2CADCRT class
     */
    class I2CADCRT : public RTObject
    {
        std::string _i2cAddress = I2C_DEF_DEV;
        int _i2cType = 0;
        int _i2cPort = 5000;
        MRL::I2cInterfaceBase::Ptr _i2c;
        //


        int _bmeAddr = 0x76;
        int _state = 0;

        enum
        {
            STATE_IDLE = 0,
            STATE_FAULT

        };
        PropertyPath _path; // where in the models the object data goes
        static StringVector _inputs;

    public:
        I2CADCRT(int id) : RTObject(id)
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
#endif // I2CADCRT_H
