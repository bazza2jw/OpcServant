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
#include "messages.h"
#include "messageids.h"
#include "parameterids.h"
#include <MrlLib/trace.hpp>
int MRL::Message::_messageId = MESSAGEID::Last;
int MRL::Message::_parameterId = PARAMETERID::UserFirst;

MRL::mapping_bimap MRL::Message::_messages;
MRL::mapping_bimap MRL::Message::_parameters; // map of paramter names to parameter ids
bool MRL::Message::_initialised = false;

#define REG_MSG(s) _messages.insert(MRL::mapping_position(STRINGIFY(s), MESSAGEID::s));
#define REG_PARAM(s) _parameters.insert(MRL::mapping_position(STRINGIFY(s), PARAMETERID::s));



/*!
    \brief MRL::Message::initialise
*/
void MRL::Message::initialise() {
    // set up the stock message and parameter maps
    if (!_initialised) {
        _initialised = true;
        REG_MSG(Update_Object) // Object has been updated (measured) in DAQ space - OPC and GUI/Web need to sync
        REG_MSG(Publish_Object) // Object requests publication
        REG_MSG(Log_Event) // Object has notified of event - typically in and out of fault
        REG_MSG(Diagnostic_Event) // an object has emitted a diagnostic (trace)
        REG_MSG(Audit_Event) // some security related event has happend - login/out or config update
        REG_MSG(Add_Object) // object added to configuration
        REG_MSG(Remove_Object) // object removed from configuration
        REG_MSG(Edit_Object) // object configuration has been editted (persitent changes) - not the same as an OPC write
        REG_MSG(Started) // object has started
        REG_MSG(Stopped) // object has stopped
        //
        // Define the stock DAQ ids - these are mesages to the DAQ thread
        REG_MSG(Daq_Write_Value) // set a value in an object - OPC write value
        REG_MSG(Daq_Exec_Method) // get object to exec method
        REG_MSG(Daq_Start) // request to start
        REG_MSG(Daq_Stop) // request to stop
        REG_MSG(Daq_Restart) // restart the DAQ process
        //
        REG_MSG(ObjectStart)
        REG_MSG(ObjectStop)
        //
        // Define the stock OPC messages - messages to the OPC thread
        //
        REG_MSG(Opc_Clear)
        REG_MSG(Opc_Update_Object)
        REG_MSG(Opc_Restart)
        //
        // Define the stock GUI / Web messages - should be the same
        //
        REG_MSG(Gui_Update) // sync with data view
        REG_MSG(SetOutput)
        REG_MSG(IdleTimeout)
        REG_MSG(PowerOff)

        // Stock Parameters

        REG_PARAM(ObjectId)
        REG_PARAM(Status) // status of object
        REG_PARAM(ActionAlertLevel) // alarm / warning status
        REG_PARAM(Timestamp) // timestamp
        REG_PARAM(Value) // current value for display
        REG_PARAM(Minimum) // value statisitics parameters
        REG_PARAM(Maximum)
        REG_PARAM(Average)
        REG_PARAM(StdDev)
        REG_PARAM(Count)
        // For event messages
        REG_PARAM(MessageType)
        REG_PARAM(Message)
        REG_PARAM(Source)
        // some sources / sinks have multiple inputs and outputs
        // Allows for rather big spectra
        REG_PARAM(Input0) // input values are based on Input0
        REG_PARAM(Ouput0) // output values are based on Output0
    }
}

/*!
 * \brief toJson
 * \param v
 * \return
 */
bool MRL::Message::toJson(Json::Value &v) // convert a message to JSON
{
    bool ret = false;
    try
    {
        v["MessageId"]  = message(_id); // convert to text
        // Now the payload
        Json::Value d;
        for(auto i = data().begin(); i != data().end(); i++)
        {
            Json::Value p;
            setJson(p,i->second);  // convert variant to JSON
            d[parameter(i->first)] = p; // assign as object to data set
        }
        v["Data"] = d;
        ret = true;
    }
    catch(...)
    {
        EXCEPT_DEF;
    }

    return ret;
}

/*!
 * \brief fromJson
 * \param v
 * \return
 */
bool MRL::Message::fromJson(Json::Value &v) // convert from JSON to a message
{
    bool ret = false;
    data().clear();
    try
    {
        _id = v["MessageId"].asInt();
        Json::Value &d = v["Data"];
        Json::Value::Members m = d.getMemberNames();
        for (int i = 0; i < int(m.size()); i++) {
            Json::Value &cv =  d[m[i]];
            if (!cv.isNull()) { // can only recurse objects
                Variant vv;
                getJson(cv,vv);
                data()[parameter(m[i])] = vv; //
            }
        }
        ret = true;
    }
    catch(...)
    {
        EXCEPT_DEF;
    }
    return ret;
}
