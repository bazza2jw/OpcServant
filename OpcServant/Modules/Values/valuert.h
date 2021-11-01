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
#ifndef VALUERT_H
#define VALUERT_H
#include <MrlLib/mrllib.h>
#include <MrlLib/states.h>
#include <Common/Daq/rtobject.h>
#include <MrlLib/rollingbuffer.hpp>
#include <MrlLib/scaleoffset.h>
#include <open62541cpp/open62541server.h>
#include <open62541cpp/serverbrowser.h>
#include <open62541cpp/servernodetree.h>

namespace MRL {

// runtime Value object
/*!
 * \brief The ValueRT class
 */
class ValueRT : public RTObject
{
public:
    class ValueItem
    {
        std::string _alias; // source alias name
        StatisticsBuffer _stats;
        ScaleOffset _scale;
        unsigned _id = 0; // the object id
        std::string _input; // the input name / path in runtime
        int _state = 0;
    public:
        ValueItem() = default;
        ValueItem(const std::string &a) : _alias(a), _stats(3600, StatisticsBuffer::TimeWindow) {}
        ValueItem(const ValueItem &) = default;
        const std::string & alias() const {
            return _alias;
        }
        StatisticsBuffer & stats() {
            return  _stats;
        }
        ScaleOffset & scale() {
            return _scale;
        }
        unsigned id() const {
            return _id;   // the object id
        }
        const std::string & input() const {
            return _input;
        }
        void mapToInput();
        int state() { return _state;}
        void setState(int s) {_state= s;}

    };

private:
    std::unique_ptr<Open62541::ServerNodeTree> _tree; // opc node tree for child processes to use
    PropertyPath _path; // where in the models the object data goes
    StringVector _inputs;
    int _group_st = 0;
    //
    static std::map<unsigned, ValueRT *> _map;
    //
    std::map<std::string,ValueItem> _values;
    //

public:
    /*!
     * \brief ValueRT
     * \param id
     */
    ValueRT(unsigned id) : RTObject(id)
    {
        _map[id] = this;
    }

    ~ValueRT()
    {
        _map.erase(dbId());
    }

    /*!
     * \brief map
     * \return
     */
    static std::map<unsigned, ValueRT *> & map() {
        return _map;
    }
    /*!
     * \brief find
     * \param id
     * \return
     */
    static ValueRT * find(unsigned id) {
        if(_map.find(id) == _map.end() ) return nullptr;
        return _map[id];
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
    /*!
     * \brief values
     * \return value set
     */
    std::map<std::string,ValueItem> & values() {
        return  _values;
    }


};
}

#endif // VALUERT_H
