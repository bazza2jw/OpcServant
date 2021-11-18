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
#ifndef SPS30RT_H
#define SPS30RT_H
#include <Common/Daq/rtobject.h>
#include <MrlLib/stats.hpp>
#include <MrlLib/rollingbuffer.hpp>
#include "sps30/sps30.h"
#include <MrlLib/i2cinterface.h>
#include <open62541cpp/servernodetree.h>

namespace MRL {
/*!
     * \brief The SPS30RT class
     */
class SPS30RT : public RTObject
{
    PropertyPath _path; // where in the models the object data goes
    std::string _i2cAddress = I2C_DEF_DEV;
    int _i2cType = 0;
    int _i2cPort = 5001;
    MRL::I2cInterfaceBase::Ptr _i2c;
    std::unique_ptr<Open62541::ServerNodeTree> _tree;
    enum
    {
        STATE_IDLE = 0,
        STATE_RESET,
        STATE_WAIT,
        STATE_MEASURE,
        STATE_FAIL
    };
    int _state = 0;
    int _waitTime = 0;
    struct sps30_measurement _m;
public:
    static  StringVector _inputs;
    SPS30RT(int id) : RTObject(id)
    {

    }
    int slaveAddress() {
        return SPS30_I2C_ADDRESS;
    }

    Open62541::UANodeTree & tree() {
        if(!_tree)
        {
            throw std::runtime_error("UANodeTree Not Defined");
        }
        return *(_tree.get());
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
     * \brief measureHtmlPage
     */
    virtual void measureHtmlPage(CTML::Node &s);
    /*!
     * \brief onOneSecond
     * \param t
     */
    void onOneSecond(time_t t);
    /*!
     * \brief hasInputs
     * \return true if has inputs
     */
    virtual bool hasInputs() const {
        return true;
    }
    /*!
     * \brief inputs
     * \return
     */
    virtual StringVector & inputs() {
        return _inputs;
    }


};
}
#endif // SPS30RT_H
