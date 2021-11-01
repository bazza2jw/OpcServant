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
#ifndef OPCN3RT_H
#define OPCN3RT_H
#include <Common/Daq/rtobject.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#include "opcn3.h"

namespace MRL {
/*!
 * \brief The OPCN3RT class
 */
class OPCN3RT : public MRL::RTObject
{
    OPCN3 _opc;
    static StringVector _inputs;
    PropertyPath _path; // where in the models the object data goes
    //
    static constexpr int FAIL_WAIT_TIME = 10000;
    static constexpr int LASER_OK = 10;
    static constexpr int FAN_OK = 10;

    Open62541::NodeId _pm1Node;
    Open62541::NodeId _pm2_5Node;
    Open62541::NodeId _pm10Node;
    Open62541::NodeId _statusNode;
    Open62541::NodeId _laserNode;
    Open62541::NodeId _fanNode;
    Open62541::NodeId _temperatureNode;
    Open62541::NodeId _humidityNode;


    //
    unsigned _sampleInterval = 60;
    unsigned _waitInterval = 0;
    std::string _address;
    //
    bool _status = false;

    OPCN3::histogramData _results;

    wxStopWatch _timer;
    //
    int _state = 0;
    uint16_t _lastCount = 0;
    enum {
        STATE_IDLE = 0,
        STATE_INIT,
        STATE_WAIT,
        STATE_FAIL,
        STATE_SAMPLE,
        STATE_WAIT_SAMPLE,
        STATE_START,
        START_STOP
    };
    //
    bool _ok = false;
    //
    static std::map<unsigned,OPCN3RT *> _map;
public:
    OPCN3RT(unsigned id) : RTObject(id),_opc(0)
    {
        _map[id] = this; // auto register
    }

    ~OPCN3RT()
    {
        _map.erase(dbId()); // remove from map
    }

    /*!
     * \brief find
     * \param id
     * \return counter RT object
     */
    static  OPCN3RT * find(unsigned id)
    {
        if(_map.find(id) != _map.end()) return _map[id];
        return nullptr;
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
     * \brief onOneSecond
     * \param t
     */
    void onOneSecond(time_t t);
    /*!
     * \brief onOneMinute
     * \param t
     */
    void onOneMinute(time_t t);

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

    /*!
     * \brief pm1
     * \return
     */
    double pm1() const { return _results.pm1;}
    /*!
     * \brief pm2_5
     * \return
     */
    double pm2_5() const { return _results.pm2_5;}
    /*!
     * \brief pm10
     * \return
     */
    double pm10() const { return _results.pm10;}
};
}
#endif // OPCN3RT_H
