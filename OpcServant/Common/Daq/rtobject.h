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
#ifndef RTOBJECT_H
#define RTOBJECT_H
#include <MrlLib/mrllib.h>
#include <MrlLib/mrlsignals.h>
#include <MrlLib/states.h>
#include <Common/bobject.h>
#include <MrlLib/variantpropertytree.h>
#include <Common/Daq/daq.h>
#include <open62541cpp/open62541objects.h>
#include <MrlLib/CTML.h>
#include <Common/messageids.h>
#include <Common/Opc/opcserver.h>
namespace MRL {

    constexpr const char *RT_SECTION = "Runtime";
    constexpr const char *CFG_SECTION = "Configure";
    constexpr const char *VALUE_TAG = "Value";
    constexpr const char *STATE_TAG = "State";



/*!
     * \brief The PathTag struct
     */
    struct PathTag
    {
        std::string _path; // path to object
        std::string _tag; // input or output
        PathTag() {}
        PathTag(const std::string &p, const std::string &t ) : _path(p), _tag(t) {}
        bool parse(const std::string &p)
        {
            //
            _path.clear();
            _tag.clear();
            //
            int n = p.find(TAG_SEPERATOR);
            if(unsigned(n)  != std::string::npos)
            {
                _path = p.substr(0,n);
                _tag = p.substr(n + 1);
                return true;
            }
            return false;
        }
    };

    /*!
        \brief The RTObject class
    */
    class RTObject : public BObject {
            //
            // Measuring is acquiring data from a source
            // publishing is forwarding the data, after processing, to other objects
            // objects do not always have to publish data
            //
            int _measureInterval = 60; // seconds between value measures
            int _publishInterval = 60; // seconds between value publish
            //
            VariantPropertyTree _configuration; // tree of configuration values
            VariantPropertyTree _runtime; // tree of runtime values
            //
            PropertyPath _path; // path in models
            std::string  _pathAsString;
            bool _fault = false;
            //
            bool _signalsConnected = false;
            //
            static StringVector _emptyList;
            //
        public:
            /*!
                \brief RTObject
                \param id
            */
            RTObject(unsigned id) : BObject(id) {
                message().connect(Daq::instance(), &BObject::queueItem); // connect notification to daq object
                Common::configuration().getPath(dbId(), _path);

            }

            /*!
             * \brief type
             */
            unsigned type() const  {
                return Common::objectType(dbId()); // get the  object manager type code
            }

            // accessors
            /*!
             * \brief measureInterval
             * \return
             */
            int measureInterval() const {
                return _measureInterval;   // seconds between value measures
            }
            /*!
             * \brief publishInterval
             * \return
             */
            int publishInterval() const {
                return _publishInterval;   // seconds between value publish
            }
            /*!
             * \brief setMeasureInterval
             * \param v
             */
            void setMeasureInterval(int v) {
                _measureInterval = v;   // seconds between value measures
            }
            /*!
             * \brief setPublishInterval
             * \param v
             */
            void setPublishInterval(int v) {
                _publishInterval = v;   // seconds between value publish
            }
            //
            // timer flags - which timers are used
            /*!
             * \brief useProcessTimer
             * \return true if process timer is used
             */
            virtual bool useProcessTimer() {
                return false;   // used to drive state machines
            }
            /*!
             * \brief useOneSecondTimer
             * \return true if one second timer is used
             */
            virtual bool useOneSecondTimer() {
                return true;   // used to drive sampling
            }
            /*!
             * \brief useOneMinuteTimer
             * \return true if use the one minute timer
             */
            virtual bool useOneMinuteTimer() {
                return false;   // used to drive timeouts and house keeping
            }
            /*!
             * \brief useOneHourTimer
             * \return true if use the one hour timer
             */
            virtual bool useOneHourTimer() {
                return false;   // used to drive timeouts and house keeping
            }

            /*!
             * \brief useOneDayTimer
             * \return true if use the one day timer
             */
            virtual bool useOneDayTimer() {
                return false;   // used to drive timeouts and house keeping
            }

            /*!
             * \brief loadConfiguration
             */
            void loadConfiguration(); // load configuration hook
            /*!
                \brief path
                \return path to object in models
            */
            PropertyPath &path() {
                return _path;
            }

            const std::string & pathAsString() { return  _pathAsString;}

            /*!
                \brief configuration
                \return the configuration tree
            */
            VariantPropertyTree &configuration() {
                return  _configuration;   // tree of configuration values
            }
            /*!
                \brief runtime
                \return the runtime data tree
            */
            VariantPropertyTree &runtime() {
                return _runtime;   // tree of runtime values
            }
            /*!
                \brief subscribe to object message handler
                \param id
            */
            void subscribe(unsigned id);
            /*!
                \brief processQueueItem
                \param o
            */
            bool processQueueItem(const Message &o);

            /*!
                \brief start
            */
            virtual void start();
            /*!
                \brief stop
            */
            virtual void stop();

            /*!
                \brief onOneSecond
                \param t
            */
            virtual void onOneSecond(time_t t) {
                if (enabled() && started()) {
                    if (_measureInterval && !(t % _measureInterval)) { // get data from source
                        measure();
                    }
                    if (_publishInterval && !(t % _publishInterval)) { // push data to destination
                        publish();
                    }
                }
            }
            /*!
                \brief onOneMinute
                minute granular events - such as timeouts, midnight handling
                \param t
            */
            virtual void onOneMinute(time_t /*t*/) {

            }

            /*!
             * \brief onOneHour
             */
            virtual void onOneHour(time_t /*t*/) {

            }

            /*!
             * \brief onOneDay
             */
            virtual void onOneDay(time_t /*t*/) {

            }


            /*!
                \brief handleProcess
            */
            void handleProcess(time_t /*t*/) {
                if(enabled())process();
            }

            /*!
                \brief handleOneSecond
                \param t
            */
            void handleOneSecond(time_t t) {
                if(enabled())onOneSecond(t);
            }
            /*!
                \brief handleOneMinute
                \param t
            */
            void handleOneMinute(time_t t) {
                if(enabled())onOneMinute(t);
            }
            /*!
             * \brief handleOneHour
             * \param t
             */
            void handleOneHour(time_t t) {
                if(enabled())onOneHour(t);
            }
            /*!
             * \brief handleOneDay
             * \param t
             */
            void handleOneDay(time_t t) {
                if(enabled())onOneDay(t);
            }

            /*!
                \brief publish
            */
            virtual void publish();
            /*!
                \brief measure
            */
            virtual void measure();
            /*!
                \brief setFault
                \param f
            */
            virtual void setFault(bool f = true, const std::string &msg = "Fault");

            /*!
                \brief logEvent
                \param type
            */
            virtual void logEvent(const std::string &type, const std::string &s);


            /*!
                \brief updateOpc
                \param server
                \param objectNode
            */
            virtual void updateOpc(OpcServer &/*server*/, Open62541::NodeId &/*objectNode*/) {}
            /*!
             * \brief initialiseOpc
             */
            virtual void initialiseOpc(OpcServer &/*server*/, Open62541::NodeId &/*objectNode*/) {}
            //
            // Standard message builders
            //
            template <typename T>
            /*!
                \brief toUpdateValue
                Single value with state and timestamp
                \param m
                \param v
                \param state
                \param timeStamp
            */
            void toUpdateValue(Message &m, const T &v,
                               const std::string &tag = VALUE_TAG,
                               int state = STATES::States::StateOk,
                               time_t timeStamp = wxDateTime::GetTimeNow()) {
                m.data().add(PARAMETERID::ObjectId,dbId()); // set the source of the message
                m.data().add(PARAMETERID::Value, v);
                m.data().add(PARAMETERID::Tag,tag);
                m.data().add(PARAMETERID::Status, state);
                m.data().add(PARAMETERID::Timestamp, timeStamp);
                //
            }

            std::string getInputAsString(const std::string &tag = VALUE_TAG)
            {
                PropertyPath p;
                p.push_back(tag);
                return runtime().getAsString(p,VALUE_TAG);
            }

            template <typename T>
            /*!
             * \brief getInput
             * \param tag
             * \return the input value
             */
            T getInput(const std::string &tag = VALUE_TAG)
            {
                T res;
                PropertyPath p;
                p.push_back(tag);
                res = runtime().getValue<T>(p,VALUE_TAG);
                return res;
            }
            /*!
             * \brief getStatus
             * \param tag
             * \return return the status value
             */
            int getStatus(const std::string &tag = VALUE_TAG)
            {
                PropertyPath p;
                p.push_back(tag);
                return runtime().getValue<int>(p,STATE_TAG);
            }

            template <typename T>
            /*!
             * \brief publishValue
             * \param v
             * \param tag
             * \param state
             */
            void publishValue(const T& v, const std::string &tag = "", int state = STATES::States::StateOk)
            {
                Message m(MESSAGEID::Publish_Object,0,dbId());
                toUpdateValue(m,v,tag,state);
                message().emit(m);
            }


            template <typename T>
            /*!
             * \brief publishValue
             * \param v
             * \param tag
             * \param state
             */
            void publishValue(const T& v, const PropertyPath &tag , int state = STATES::States::StateOk)
            {
                std::string t;
                tag.toString(t);
                Message m(MESSAGEID::Publish_Object,0,dbId());
                toUpdateValue(m,v,t,state);
                message().emit(m);
            }


            template <typename T>
            /*!
             * \brief publishValue
             * \param path
             * \param tag
             * \param v
             * \param state
             */
            void publishValue( PropertyPath &path , const std::string &tag, const T& v,int state = STATES::States::StateOk)
            {
                path.push_back(tag);
                std::string t;
                path.toString(t);
                Message m(MESSAGEID::Publish_Object,0,dbId());
                toUpdateValue(m,v,t,state);
                message().emit(m);
                path.pop_back();
            }



            /*!
             * \brief publishRecord
             * \param state
             * \param v
             */
            void publishRecord(const std::string &v,int state = STATES::States::StateOk )
            {
                Message m(MESSAGEID::Publish_Record,0,dbId());
                m.data().add(PARAMETERID::ObjectId,dbId()); // set the source of the message
                m.data().add(PARAMETERID::Status, state);
                m.data().add(PARAMETERID::Record, v); // the record data could be JSON or CSV or XML or ...
                message().emit(m);
            }

            /*!
             * \brief publishRecord
             * \param v record in JSON
             * \param state
             */
            void publishRecord(Json::Value &v,int state = STATES::States::StateOk)
            {
                std::string s;
                if(MRL::jsonToString(v,s))
                {
                    publishRecord(s, state);
                }
            }



            template <typename T>
            /*!
             * \brief updateValue
             * \param v
             */
            void updateValue(const T &v, const std::string & tag = VALUE_TAG, int state = STATES::States::StateOk)
            {
                //
                PropertyPath p;
                p.push_back(tag);
                runtime().setValue(p,VALUE_TAG,v);
                runtime().setValue(p,STATE_TAG,state);
                //
                Message m(MESSAGEID::Update_Value,0,dbId());
                // say we are posting an update
                toUpdateValue(m,v,tag,state);
                // now post the value
                message().emit(m);
            }


            /*!
             * \brief updateObject
             * \param state
             */
            void updateObject(int state = STATES::States::StateOk, time_t timeStamp = wxDateTime::GetTimeNow())
            {
                Message m(MESSAGEID::Update_Object,0,dbId());
                m.data().add(PARAMETERID::ObjectId,dbId()); // set the source of the message
                m.data().add(PARAMETERID::Status, state);
                m.data().add(PARAMETERID::Timestamp, timeStamp);
                // now post the value
                message().emit(m);
            }

            /*!
             * \brief updateValue
             * \param tag
             * \param v
             */
            void updateValue(const std::string &tag, const Open62541::Variant& vIn);

            /*!
             * \brief toUpdateValue
             * \param m
             * \param tag
             * \param v
             */
            void toUpdateValue(Message &m, const std::string &tag, const Open62541::Variant &vIn);

            /*!
             * \brief publishHtmlPage
             */
            virtual void publishHtmlPage(std::string &s); // publish the current runtime status as HTML text

            /*!
             * \brief measureHtmlPage
             */
            virtual void measureHtmlPage(std::string &s);

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
            virtual bool hasInputs() const { return false;}
            /*!
             * \brief hasOutputs
             * \return true if has outputs
             */
            virtual bool hasOutputs() const { return false;}
            /*!
             * \brief inputs
             * \return array of input names
             */
            virtual StringVector & inputs() { return _emptyList;}
            /*!
             * \brief outputs
             * \return array of outputs
             */
            virtual StringVector &outputs() { return _emptyList;}

            /*!
             * \brief createTabView
             * \return true if a tab window is added when the object starts (if it does not exist already)
             * and the GUI and/or web is enabled - query the configuration
             */
            virtual bool createTabView()
            {
                if (configuration().exists("EnableTabView")) {
                    return configuration().getValue<bool>("EnableTabView");
                }
                return false;
            }

            /*!
             * \brief createTopWindow
             * \return when this object is created a top level view window is created - if one does not exist
             */
            virtual bool createTopWindow()
            {
                if (configuration().exists("EnableTopWindow")) {
                    return configuration().getValue<bool>("EnableTopWindow");
                }
                return false;
            }
    };

    typedef std::unique_ptr<RTObject> RtObjectRef;
}
#endif // RTOBJECT_H
