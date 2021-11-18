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
#ifndef LWSAPEXRT_H
#define LWSAPEXRT_H
#include <Common/Daq/rtobject.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#include <MrlLib/modbusClass.h>


namespace MRL {
/*!
 * \brief The LwsApexRT class
 */
class LwsApexRT : public MRL::RTObject
{
    MrlModbus::Modbus _modbus;
    static StringVector _inputs;
    PropertyPath _path; // where in the models the object data goes
    //
    Open62541::NodeId _particles_0_5Node;
    Open62541::NodeId _particles_5_0Node;
    Open62541::NodeId _statusNode;
    //
    unsigned _sampleTime = 60;
    unsigned _purgeTime = 0;
    std::string _address;
    //
    unsigned _status = 0;
    double _particles_0_5 = 0.0;
    double _particles_5_0 = 0.0;

    //
    int _state = 0;
    uint16_t _lastCount = 0;
    enum {
        STATE_IDLE = 0,
        STATE_INIT,
        STATE_PROCESS
    };
    //
    static std::map<unsigned,LwsApexRT *> _map;
public:
    LwsApexRT(unsigned id) : RTObject(id)
    {
        _map[id] = this; // auto register
    }

    ~LwsApexRT()
    {
        _map.erase(dbId()); // remove from map
    }

    /*!
     * \brief find
     * \param id
     * \return counter RT object
     */
    static  LwsApexRT * find(unsigned id)
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
     * \brief particles_0_5
     * \return
     */
    double particles_0_5() const { return _particles_0_5;}
    /*!
     * \brief particles_5_0
     * \return
     */
    double particles_5_0() const { return _particles_5_0;}
};
}
#endif // LWSAPEXRT_H
