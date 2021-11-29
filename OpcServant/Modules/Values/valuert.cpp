/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "valuert.h"
#include <Common/Daq/daqcommon.h>

std::map<unsigned, MRL::ValueRT *> MRL::ValueRT::_map;

/*!
 * \brief MRL::ValueRT::ValueItem::mapToInput
 */
void MRL::ValueRT::ValueItem::mapToInput()
{
    // translate the alias to an object id and an input path
    std::string o = Common::aliasMap()[_alias];
    // split on the tag separator
    PathTag pt;
    if(pt.parse(o))
    {
        PropertyPath p;
        p.toList(pt._path);
        _id = MRL::Common::configuration().find(p);
        _input = pt._tag;
    }
}

/*!
    \brief start
*/
void MRL::ValueRT::start() {
    //
    RTObject::start();
    //
    _values.clear();
    _inputs.clear();
    _inputs.push_back("GroupState"); // teh group state is useful for alarm enunciation
    //
    MRL::StringList l ;
    PropertyPath p;
    //
    p.push_back("Values");
    configuration().listChildren(p,l);
    //
    for(auto i = l.begin(); i != l.end(); i++)
    {
        _inputs.push_back(*i);
        p.push_back(*i);
        std::string a =  configuration().getValue<std::string>(p,"Object");
        ValueItem v(a);
        v.scale().setScaleOffset(configuration().getValue<double>(p,"Scale"), configuration().getValue<double>(p,"Offset"));
        v.stats().statistics().setThresholds(
            configuration().getValue<double>(p,"LoLo"),
            configuration().getValue<double>(p,"LoHi"),
            configuration().getValue<double>(p,"HiLo"),
            configuration().getValue<double>(p,"HiHi"),
            configuration().getValue<bool>(p,"LoLoEnabled"),
            configuration().getValue<bool>(p,"LoHiEnabled"),
            configuration().getValue<bool>(p,"HiLoEnabled"),
            configuration().getValue<bool>(p,"HiHiEnabled")
        );
        p.pop_back();
        v.mapToInput();
        _values[*i] = v; // add to dictionary
    }
    //
    runtime().setValue("State", STATES::States::StateOk);
    //
    //
    // Get the output ids as path and output name
    _okOutput.parse(configuration().getString("/OkOutput"));
    _alertOutput.parse(configuration().getString("/AlertOutput"));
    _actionOutput.parse(configuration().getString("/ActionOutput"));
    _failureOutput.parse(configuration().getString("/FailureOutput"));
}

/*!
    \brief stop
*/
void MRL::ValueRT::stop() {
    RTObject::stop();
}

/*!
    \brief publish
*/
void MRL::ValueRT::publish() {
    RTObject::publish();

    for(auto i =  _values.begin(); i !=  _values.end(); i++)
    {
        ValueItem &v = i->second;
        // emit a publish message
        if (v.stats().statistics().getNumberSamples() > 0) {
            Json::Value jv;
            publishValue(v.stats().statistics().getLastValue(), i->first, v.state());
            // publish record with all stats - TBD
            v.stats().statistics().toJson(jv);
            publishRecord(jv,v.state());
        }
    }
    publishValue(_group_st, "GroupState", _group_st);
}

/*!
    \brief measure
*/
void MRL::ValueRT::measure() {
    RTObject::measure();
    // get the current values - the set is updated at the same time, not when the source value is updated
    // this means the set has the same timestamp. Source values can update at any time
    //
    _group_st = STATES::States::StateOk;
    for(auto i =  _values.begin(); i !=  _values.end(); i++)
    {
        try
        {
            ValueItem &v = i->second;
            //
            // get the value
            MRL::RtObjectRef &r = MRL::Common::daq().objects()[v.id()];
            if(r)
            {
                // scale it
                if(r->runtime().exists(v.input()))
                {
                    MRL::PropertyPath p;
                    p.push_back(v.input());
                    p.push_back(MRL::VALUE_TAG);
                    //
                    double d;
                    d = r->runtime().getValue<double>(p);
                    d = v.scale().scale(d);
                    // update stats
                    v.stats().addValue(d);
                    // set in the runtime
                    v.stats().evaluate();
                    //
                    int st = STATES::States::StateOk;
                    if (v.stats().statistics().triggeredHiHiLoLo()) {
                        st = STATES::States::StateAction;
                    }
                    else if (v.stats().statistics().triggeredHiLoLoHi()) {
                        st = STATES::States::StateAlert;
                    }
                    else {

                    }
                    v.setState(st);
                    if(st > _group_st) _group_st = st;
                    //
                    updateValue(d, i->first, st);
                }
            }
        }
        catch(...)
        {

        }
    }
    runtime().setInt("/GroupState",_group_st);
    updateObject(_group_st);
    //
    // Now manage the outputs based on the group state
    //
    switch(_group_st)
    {
    case STATES::States::StateOk:
        setOutputs(true);
        break;
    case STATES::States::StateAction:
        setOutputs(false,true);
        break;
    case STATES::States::StateAlert:
        setOutputs(false,false,true);
        break;
    case  STATES::States::StateFault:
        setOutputs(false,false,false,true);
        break;
        default:
        setOutputs();
        break;
    }
}

/*!
    \brief MRL::ValueRT::updateOpc
    \param server
    \param objectNode
*/
void MRL::ValueRT::updateOpc(OpcServer &/*server*/, Open62541::NodeId &/*objectNode*/) {

    if(_tree)
    {
        for(auto i =  _values.begin(); i !=  _values.end(); i++)
        {
            try
            {
                ValueItem &v = i->second;
                int st = STATES::States::StateOk;
                if (v.stats().statistics().triggeredHiHiLoLo()) {
                    st = STATES::States::StateAction;
                }
                else if (v.stats().statistics().triggeredHiLoLoHi()) {
                    st = STATES::States::StateAlert;
                }

                // get the value
                Open62541::UAPath p;
                Open62541::Variant vn(v.stats().statistics().getLastValue());
                Open62541::Variant sn(st);
                Open62541::Variant min(v.stats().statistics().getMinimum());
                Open62541::Variant max(v.stats().statistics().getMaximum());
                Open62541::Variant mean(v.stats().statistics().getMean());

                //
                p.push_back(i->first);
                //
                _tree->setNodeValue(p,"Value",vn);
                _tree->setNodeValue(p,"Status",sn);
                _tree->setNodeValue(p,"Minimum",min);
                _tree->setNodeValue(p,"Maximum",max);
                _tree->setNodeValue(p,"Mean",mean);

                // TBD log all statistics
            }
            catch(...)
            {

            }
        }
    }
}


/*!
    \brief MRL::ValueRT::initialiseOpc
    \param server
    \param objectNode
*/
void MRL::ValueRT::initialiseOpc(OpcServer &server, Open62541::NodeId &node) {
    _tree = std::make_unique<Open62541::ServerNodeTree>(server,node,server.applicationNamespace());
    server.browseTree(node,*_tree); // set up the tree
    for(auto i =  _values.begin(); i !=  _values.end(); i++)
    {
        Open62541::UAPath p;
        Open62541::Variant vn(double(0));
        Open62541::Variant sn(int(0));
        p.push_back(i->first);
        _tree->setNodeValue(p,"Value",vn);
        _tree->setNodeValue(p,"Status",sn);
        _tree->setNodeValue(p,"Minimum",vn);
        _tree->setNodeValue(p,"Maximum",vn);
        _tree->setNodeValue(p,"Mean",vn);
    }
}


/*!
    \brief publishHtmlPage
*/
void MRL::ValueRT::publishHtmlPage(CTML::Node &s) { // publish the current runtime status as HTML text
    RTObject::publishHtmlPage(s);
    //
    // Construct an HTML table - colour coded
    //
//    StatisticsThresholdSet &st = _stats.evaluate();
//    std::stringstream ss;
//    ss << "Mean     " << st.getMean() << std::endl;
//    ss << "Maximum  " << st.getMaximum() << std::endl;
//    ss << "Minimum  " << st.getMinimum() << std::endl;
//    ss << "Std.Dev. " << st.getStdDev() << std::endl;
//    ss << "Count    " << st.getNumberSamples() << std::endl;
    //
//    CTML::Node v("p");
//    v.SetContent(ss.str());
//    s.AppendChild(v);
    //
}

/*!
    \brief measureHtmlPage
*/
void MRL::ValueRT::measureHtmlPage(CTML::Node &s) {
    RTObject::measureHtmlPage(s);
//    std::stringstream ss;
//    ss << "Value = " << _stats.statistics().getMean() << " Status " << _stats.statistics().stateToString();
//    CTML::Node v("p");
//    v.SetContent(ss.str());
//    s.AppendChild(v);
}
