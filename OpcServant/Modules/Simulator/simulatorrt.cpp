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
#include "simulatorrt.h"
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>

MRL::StringVector MRL::SimulatorRT::_inputs = {"Value"};
MRL::StringVector MRL::SimulatorRT::_outputs = {"Output"};


/*!
    \brief start
*/
void MRL::SimulatorRT::start()
{

    // initialise from configuration
    // get the range
    _range = configuration().getValue<int>("Range");
    if(_range < 5) _range = 100;
    std::string s;
    s = configuration().getValue<std::string>("Type");
    if(s == "Ramp")
    {
        _type = Ramp;
    }
    else
    {
        _type = Random;
    }
    //
    //
    RTObject::start();
}

/*!
    \brief stop
*/
void MRL::SimulatorRT::stop()
{
    RTObject::stop();
}

/*!
    \brief publish
*/
void MRL::SimulatorRT::publish()
{
    RTObject::publish();
    // emit a publish message
    //TRC(_lastValue);
    //
    // This is an example of publishing
    // not all values are published
    Message m(MESSAGEID::Publish_Object,0,dbId());
    // say we are posting an update
    toUpdateValue(m,(double)_lastValue,"Value");
    // now post the value
    message().emit(m);
    //
}

/*!
    \brief measure
*/
void MRL::SimulatorRT::measure()
{
    RTObject::measure();
    if(_type == Random)
    {
        _lastValue = std::rand() % _range;
    }
    else
    {
        // ramp generation
        if(_dirDown)
        {
            _lastValue--;
            if(_lastValue < 1) _dirDown = false;
        }
        else
        {
            _lastValue++;
            if(_lastValue  > _range) _dirDown = true;
        }
    }
    //
    // Now set the value
    //TRC(_lastValue)
    //
    updateValue(double(_lastValue),"Value");
    updateObject();
}

/*!
 * \brief MRL::SimulatorRT::updateOpc
 * \param server
 * \param objectNode
 */
void MRL::SimulatorRT::updateOpc(OpcServer &server, Open62541::NodeId &/*objectNode*/)
{
    // update the value
    Open62541::Variant v(_lastValue);
    // find the child node called Value
    server.writeValue(_valueNode,v);
    //
    Open62541::Variant o(_output);
    server.writeValue(_outputNode,o);
}


/*!
 * \brief MRL::SimulatorRT::initialiseOpc
 * \param server
 * \param objectNode
 */
void MRL::SimulatorRT::initialiseOpc(OpcServer &server, Open62541::NodeId &node)
{
        //
        // set up the variables
        //
        server.getChild(node,"Range",_rangeNode);
        TRC_LINE("Range Node " << Open62541::toString(_rangeNode));
        server.getChild(node,"Type",_typeNode);
        TRC_LINE("Type Node " << Open62541::toString(_typeNode));
        server.getChild(node,"Value",_valueNode);
        TRC_LINE("Value Node " << Open62541::toString(_valueNode));
        server.getChild(node,"Output",_outputNode);
        //
        Open62541::Variant v(configuration().getValue<int>("Range"));
        server.writeValue(_rangeNode,v);
        Open62541::Variant vr(configuration().getValue<std::string>("Type"));
        server.writeValue(_typeNode,vr);
        //
        // Set up Historian
        // _historian.setUpdateNode(nodeNumber,*this);
        server.historian().setUpdateNode(_valueNode,server);

}


/*!
 * \brief publishHtmlPage
 */
void MRL::SimulatorRT::publishHtmlPage(CTML::Node &s) // publish the current runtime status as HTML text
{
    RTObject::publishHtmlPage(s);
//    std::stringstream ss;
//    ss << "Mean     " << st.getMean() << std::endl;
//    ss << "Maximum  " << st.getMaximum() << std::endl;
//    ss << "Minimum  " << st.getMinimum() << std::endl;
//    ss << "Std.Dev. " << st.getStdDev() << std::endl;
//    ss << "Count    " << st.getNumberSamples() << std::endl;
//    //
//    CTML::Node v("p");
//    v.SetContent(ss.str());
//    s.AppendChild(v);
//
}

/*!
 * \brief measureHtmlPage
 */
void MRL::SimulatorRT::measureHtmlPage(CTML::Node &s)
{
    RTObject::measureHtmlPage(s);
    std::stringstream ss;
    ss << "Value = " << _lastValue;
    CTML::Node v("p");
    v.SetContent(ss.str());
    s.AppendChild(v);
}

/*!
    \brief processQueueItem
*/
bool MRL::SimulatorRT::processQueueItem(const Message &msg) {
    Message &m = const_cast<Message &>(msg);
    if (!RTObject::processQueueItem(m)) {
        switch (m.id()) {
            case MESSAGEID::SetOutput: {
                bool v;
                std::string i;
                m.data().get(PARAMETERID::Index, i);
                m.data().get(PARAMETERID::Value, v);
                //
                if(i == "Output")
                {
                    _output = v;
                    wxLogDebug("Set Output to %s",v?"On":"Off");
                }
            }
            break;
            default:
                break;
        }
        return false;
    }
    return true; // return true if processed
}


