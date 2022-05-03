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
#include "daq.h"
#include "rtobject.h"
#include "daqcommon.h"
#include <MrlLib/states.h>
#include <Common/bobject.h>
#include <Common/Opc/opccommon.h>
#include <Common/messageids.h>
#include <MrlLib/costate.h>

MRL::StringVector MRL::RTObject::_emptyList;


/*!
 * \brief MRL::RTObject::toUpdateValue
 * \param v
 */
void MRL::RTObject::updateValue(const std::string &tag, Open62541::Variant& v)
{
    std::string ret;
    const UA_DataType *t = v.dataType();

    switch (t->typeKind) {
    /**
        Boolean
        ^^^^^^^
    */
    case UA_DATATYPEKIND_BOOLEAN: {
        updateValue(v.value<bool>(), tag);
    }
    break;

    /**
        Int16
        ^^^^^
    */
    case UA_DATATYPEKIND_INT16: {
        updateValue(int(v.value<int16_t>()), tag);
    }
    break;

    /**
        UInt16
        ^^^^^^
    */
    case UA_DATATYPEKIND_UINT16: {
        updateValue(unsigned(v.value<uint16_t>()), tag);

    }
    break;

    /**
        Int32
        ^^^^^
    */
    case UA_DATATYPEKIND_INT32: {
        updateValue(int(v.value<int32_t>()), tag);
    }
    break;

    /**
        UInt32
        ^^^^^^
    */
    case UA_DATATYPEKIND_UINT32: {
        updateValue(unsigned(v.value<uint32_t>()), tag);

    }
    break;

    /**
        Int64
        ^^^^^
    */
    case UA_DATATYPEKIND_INT64: {
        updateValue(v.value<int64_t>(), tag);

    }
    break;

    /**
        UInt64
        ^^^^^^
    */
    case UA_DATATYPEKIND_UINT64: {
        updateValue(v.value<uint64_t>(), tag);
    }
    break;

    /**
        Float
        ^^^^^
    */
    case UA_DATATYPEKIND_FLOAT: {
        updateValue(double(v.value<float>()), tag);

    }
    break;

    /**
        Double
        ^^^^^^
    */
    case UA_DATATYPEKIND_DOUBLE: {
        updateValue(v.value<double>(), tag);
    }
    break;

    /**
        String
        ^^^^^^
    */
    case UA_DATATYPEKIND_STRING: {
        UA_String* p = (UA_String*)(v.get().data);
        ret          = std::string((const char*)p->data, p->length);
        updateValue(ret, tag);

    }
    break;


    /**
        ByteString
        ^^^^^^^^^^
    */
    case UA_DATATYPEKIND_BYTESTRING: {
        UA_ByteString* p = (UA_ByteString*)(v.get().data);
        ret              = std::string((const char*)p->data, p->length);
        updateValue(ret, tag);
    }
    break;

    default:
        break;
    }
}

/*!
 * \brief MRL::RTObject::toUpdateValue
 * \param v
 */
void MRL::RTObject::toUpdateValue(Message &m, const std::string &tag, Open62541::Variant& v)
{
    std::string ret;
    const UA_DataType *t = v.dataType();

    switch (t->typeKind) {
    /**
        Boolean
        ^^^^^^^
    */
    case UA_DATATYPEKIND_BOOLEAN: {
        toUpdateValue(m,v.value<bool>(), tag);
    }
    break;

    /**
        Int16
        ^^^^^
    */
    case UA_DATATYPEKIND_INT16: {
        toUpdateValue(m,int(v.value<int16_t>()), tag);
    }
    break;

    /**
        UInt16
        ^^^^^^
    */
    case UA_DATATYPEKIND_UINT16: {
        toUpdateValue(m,unsigned(v.value<int16_t>()), tag);

    }
    break;

    /**
        Int32
        ^^^^^
    */
    case UA_DATATYPEKIND_INT32: {
        toUpdateValue(m,int(v.value<int32_t>()), tag);
    }
    break;

    /**
        UInt32
        ^^^^^^
    */
    case UA_DATATYPEKIND_UINT32: {
        toUpdateValue(m,unsigned(v.value<int32_t>()), tag);

    }
    break;

    /**
        Int64
        ^^^^^
    */
    case UA_DATATYPEKIND_INT64: {
        toUpdateValue(m,(v.value<int64_t>()), tag);

    }
    break;

    /**
        UInt64
        ^^^^^^
    */
    case UA_DATATYPEKIND_UINT64: {
        toUpdateValue(m,(v.value<uint64_t>()), tag);
    }
    break;

    /**
        Float
        ^^^^^
    */
    case UA_DATATYPEKIND_FLOAT: {
        toUpdateValue(m,(v.value<float>()), tag);

    }
    break;

    /**
        Double
        ^^^^^^
    */
    case UA_DATATYPEKIND_DOUBLE: {
        toUpdateValue(m,(v.value<double>()), tag);
    }
    break;

    /**
        String
        ^^^^^^
    */
    case UA_DATATYPEKIND_STRING: {
        UA_String* p = (UA_String*)(v.get().data);
        ret          = std::string((const char*)p->data, p->length);
        toUpdateValue(m,ret, tag);

    }
    break;


    /**
        ByteString
        ^^^^^^^^^^
    */
    case UA_DATATYPEKIND_BYTESTRING: {
        UA_ByteString* p = (UA_ByteString*)(v.get().data);
        ret              = std::string((const char*)p->data, p->length);
        toUpdateValue(m,ret, tag);
    }
    break;

    default:
        break;
    }
}




/*!
 * \brief MRL::RTObject::logEvent
 * \param type
 * \param s
 */
void MRL::RTObject::logEvent(const std::string &type, const std::string &s) {
    std::stringstream is;
    is << "RTObject:" << dbId();
    Common::instance()->logMessage(pathAsString(),s,type);
}


/*!
    \brief MRL::RTObject::subscribe
    \param id
*/
void MRL::RTObject::subscribe(unsigned id) {
    RtObjectRef &r = Common::daq().objects()[id];
    if (r) {
        join(r.get());
    }
}


/*!
    \brief MRL::RTObject::processQueueItem
    \param o
*/
bool MRL::RTObject::processQueueItem(const Message &o) {
    if (!BObject::processQueueItem(o)) {
        // process base messages
        return false;
    }
    return true;
}

/*!
    \brief MRL::RTObject::loadConfiguration
*/
void MRL::RTObject::loadConfiguration() {
    //
    // Get the display path
    Common::configuration().getPath(dbId(), _path);
    _path.toString(_pathAsString);
    //
    // get the configuration
    Json::Value v;
    Common::configuration().getData(_path, v);
    configuration().fromJson(v); // load configuration
    //configuration().dump();
    setEnabled(configuration().getValue<bool>("Enabled"));
}

/*!
    \brief start
*/
void MRL::RTObject::start() {
    // Set up the basic runtime
    runtime().setValue("State", int(enabled() ? STATES::States::StateWaiting : STATES::States::StateDisabled)); // activity state
    runtime().setValue("Quality", int(STATES::States::QualityBad)); // nothing collected so unknown
    //
    runtime().setValue("LastMeasure", time_t(0)); // set the time stamps  to 0 hence not set
    runtime().setValue("LastPublish", time_t(0)); //
    runtime().setValue("LastFault", time_t(0));  //
    //
    loadConfiguration();
    //
    //
    if (configuration().exists("MeasureInterval")) {
        _measureInterval = configuration().getValue<int>("MeasureInterval");
    }
    else {
        _measureInterval = 0;
    }
    //
    if (configuration().exists("PublishInterval")) {
        _publishInterval = configuration().getValue<int>("PublishInterval");
    }
    else {
        _publishInterval = 0;
    }
    //
    //
    // Connect to master DAQ object to get timing
    //
    if (!_signalsConnected) {
        _signalsConnected = true;
        Daq *p = Daq::instance();
        wxASSERT(p != nullptr);
        if (useProcessTimer())   p->processTimer().connect(this, &RTObject::handleProcess);
        if (useOneSecondTimer()) p->oneSecondTimer().connect(this, &RTObject::handleOneSecond);
        if (useOneMinuteTimer()) p->oneMinuteTimer().connect(this, &RTObject::handleOneMinute);
        if (useOneHourTimer()) p->oneHourTimer().connect(this, &RTObject::handleOneHour);
        if (useOneDayTimer()) p->oneDayTimer().connect(this, &RTObject::handleOneDay);

    }
    //
    //
    {
        Message msg(MESSAGEID::Started,dbId());
        msg.data().add(PARAMETERID::ObjectId, dbId());
        message().emit(msg);
    }
    //
    //
    // Now do we create windows for this object or not -
    // the GUI and Web have to do the create
    //
    if(createTabView())
    {
        Message msg(MESSAGEID::CreateTabView,dbId());
        msg.data().add(PARAMETERID::ObjectId, dbId());
        message().emit(msg);
    }
    //
    if(createTopWindow())
    {
        Message msg(MESSAGEID::CreateTopView,dbId());
        msg.data().add(PARAMETERID::ObjectId, dbId());
        message().emit(msg);
    }
    //
    BObject::start();
    logEvent(STATES::STATE_STARTED, pathAsString());
}

/*!
    \brief stop
*/
void MRL::RTObject::stop() {
    BObject::stop();
    logEvent(STATES::STATE_STOPPED, pathAsString());
}

/*!
    \brief MRL::RTObject::publish
*/
void MRL::RTObject::publish() {
    // action the publish - post to publish interfaces
    // do the measure - post current values to subscribers
    runtime().setValue("LastPublish", wxDateTime::Now().GetTicks());
}

/*!
    \brief MRL::RTObject::measure
*/
void MRL::RTObject::measure() {
    // do the measure - post current values to subscribers
    runtime().setValue("LastMeasure", wxDateTime::Now().GetTicks());
    runtime().setValue("Quality", int(STATES::States::QualityGood));
}

/*!
    \brief MRL::RTObject::setFault
    \param f
    \param msg
*/
void MRL::RTObject::setFault(bool f, const std::string &msg) {
    if (_fault != f) {
        _fault = f;
        if (_fault) {
            runtime().setValue("LastFault", wxDateTime::Now().GetTicks());
            runtime().setValue("State", int(STATES::States::StateFault));
            runtime().setValue("Quality", int(STATES::States::QualityBad));
            runtime().setValue("Comment", msg);

        }
        else {
            runtime().setValue("State", int(STATES::States::StateNone)); // not in a fault condition but not yet resolved
            runtime().setValue("Comment", std::string("Working")); // say working
        }
    }

}


/*!
    \brief publishHtmlPage
    Published status which may not update as often as measures
*/
void MRL::RTObject::publishHtmlPage(std::string &s) { // publish the current runtime status as HTML text
    CTML::Node d("html");
    publishHtmlPage(d);
    s = d.ToString();
}

/*!
    \brief measureHtmlPage
    this page is used in displays and can update often
*/
void MRL::RTObject::measureHtmlPage(std::string &s) {
    CTML::Node d("html");
    measureHtmlPage(d);
    s = d.ToString();
}


/*!
    \brief publishHtmlPage
*/
void MRL::RTObject::publishHtmlPage(CTML::Node &s) { // publish the current runtime status as HTML text
    // Get the path
    std::string str;
    PropertyPath path;
    Common::configuration().getPath(dbId(), path);
    path.toString(str);
    str += " ID:" + std::to_string(dbId());
    CTML::Node n("b");
    n.SetContent(str);
    s.AppendChild(n);
    CTML::Node d("p");
    d.SetContent(wxDateTime::Now().FormatISOCombined().ToStdString());
    s.AppendChild(d);
}

/*!
    \brief measureHtmlPage
*/
void MRL::RTObject::measureHtmlPage(CTML::Node &s) {
    std::string str;
    PropertyPath path;
    Common::configuration().getPath(dbId(), path);
    path.toString(str);
    str += " ID:" + std::to_string(dbId());
    CTML::Node n("b");
    n.SetContent(str);
    s.AppendChild(n);
    CTML::Node d("p");
    d.SetContent(wxDateTime::Now().FormatISOCombined().ToStdString());
    s.AppendChild(d);
}




