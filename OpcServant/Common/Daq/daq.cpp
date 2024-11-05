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
#include <Common/datastore.h>
#include <Common/objectmanager.h>
#include <Common/Daq/daqcommon.h>
#include <Common/messageids.h>
#include <Common/stockobjectids.h>
MRL::Daq   *MRL::Daq::_instance = nullptr; //!< singleton
/*!
    \brief MRL::Daq::Daq
*/
MRL::Daq::Daq() {
    _instance = this;
    setEnabled(true);
    _localDb = std::make_unique<LocalDb>(MRL::LocalDb::LOCAL_DB_DIR, LOCAL_DB_NAME);
}

/*!
 * \brief MRL::Daq::~Daq
 */
MRL::Daq::~Daq() {
    _instance = nullptr;
}



/*!
    \brief MRL::Daq::addObject
    \param id
*/
void MRL::Daq::addObject(unsigned id) {
    DataNodeIdMap &m = Common::configuration().idMap();
    DataNode *d  = m[id];
    if (d) {
        auto o =  ObjectManager::find(d->data().type());
        if (o && o->canRun()) {
            RtObjectRef r;
            wxLogDebug("Starting Object %d", id);
            if (o->createRtObject(id, r)) { // create daq object instance
                removeObject(id); // if it exists remove it
                auto p = r.get();
                Common::daq().objects()[id] = std::move(r); // store in map key by object id
                p->loadConfiguration();
                p->start();
                //
                Message msg(MESSAGEID::Add_Object);
                msg.data().add(PARAMETERID::ObjectId, id);
                message().emit(msg);
                //
            }
        }
    }
}

/*!
    \brief MRL::Daq::removeObject
    remove from DAQ - may have been removed from the configuration
    \param id
*/
void MRL::Daq::removeObject(unsigned id) {
    auto j = Common::daq().objects().find(id);
    if (j != Common::daq().objects().end()) {
        if ((j->second).get()) {
            RTObject *rp = (j->second).get();
            if (rp) {
                rp->stop();
                //
                Message msg(MESSAGEID::Remove_Object);
                msg.data().add(PARAMETERID::ObjectId, id);
                message().emit(msg);
                //
            }
        }
        Common::daq().objects().erase(id); // deletes object
    }
}

/*!
    \brief MRL::Daq::start
*/
void MRL::Daq::start() {
    _now = wxDateTime::Now().GetTicks();
    try {
        DataNodeIdMap &m = Common::configuration().idMap();
        stop(); // halt and clear
        for (auto i = m.begin(); i != m.end(); i++) {
            try
            {
                addObject(i->first);
            }
            catch(...)
            {
                removeObject(i->first);
            }
        }
        //
        setEnabled(true);
        BObject::start();
        //
        Message msg(MESSAGEID::Started);
        msg.data().add(PARAMETERID::ObjectId, STOCKOBJECTIDS::Daq);
        message().emit(msg);
        //
        _now = 0; // one second ticks
        sw.Start();
        //
        MRL::PropertyPath p;
        p.push_back("System");
        setAutoPurgeDatabase(MRL::SETTINGS().getValue<bool>(p, "AutoPurge"));
        //
    }
    CATCH_DEF
}

/*!
    \brief MRL::Daq::stop
    Full stop - this is bulk stop
*/
void MRL::Daq::stop() {
    BObject::stop();
    wxMilliSleep(100); // allow the DAQ thread to finish
    try {
        DataNodeIdMap &m = Common::configuration().idMap(); // get the configuration
        for (auto i = m.begin(); i != m.end(); i++) {
            removeObject(i->first);
        }
        //
        Message msg(MESSAGEID::Stopped);
        msg.data().add(PARAMETERID::ObjectId, STOCKOBJECTIDS::Daq);
        message().emit(msg);
        //
        setAutoPurgeDatabase(false);
    }
    CATCH_DEF
}

/*!
    \brief MRL::Daq::process
*/
void MRL::Daq::process() {
    try {
        // Generate timer events - one second and one minute
        if (started()) {
            time_t t = std::time(nullptr);
            if (t > _now) {
                _now = t;
                _oneSecondTimer(_now);
                //
                if (!(_now % secondsPerMinute)) {
                    _oneMinuteTimer(_now);
                    // hourly tasks
                    if (!(_now % secondsPerHour)) {
                        _oneHourTimer(_now);
                        if(_autoPurgeDatabase) _localDb->purgeAll();
                        if (!(_now % secondsPerHour)) { // daily tasks
                            _oneDayTimer(_now);
                        }
                    }
                }
            }
            _processTimer(_now); // drive any state machines
        }
        // process messages
        BObject::process();
    }
    CATCH_DEF
}

/*!
    \brief MRL::Daq::processQueueItem
    \param m
    \return true if the message is processed
*/
bool MRL::Daq::processQueueItem(const Message &msg) { //!< process an item in the queue
    try
    {
        if (!BObject::processQueueItem(msg)) {
            // work out what to do with the message
            Message &m = const_cast<Message &>(msg);
            switch (m.id()) {
            case MESSAGEID::Daq_Purge:
            {
                _localDb->purgeAll(); // purge the database
            }
            break;
            case MESSAGEID::IdleTimeout:
            case MESSAGEID::Update_Object:
            case MESSAGEID::Update_Value:
            case MESSAGEID::CreateTopView:
            case MESSAGEID::CreateTabView:
                message().emit(m); // forward the message to any listeners
                break;
            //
            case MESSAGEID::Publish_Record:
            {   // publication
                message().emit(m); // forward the message to any listeners
                publishMessage().emit(m); // forward the message to any listeners
                //
                // Route to the ISAM database
            }
            break;

            case MESSAGEID::Publish_Object: { // publication
                message().emit(m); // forward the message to any listeners
                publishMessage().emit(m); // forward the message to any listeners
                //
                // Route to the ISAM database if enabled
                //
            }
            break;

            case MESSAGEID::Add_Object: {
                //
                unsigned id;
                m.data().get(PARAMETERID::ObjectId, id);
                addObject(id);
                message().emit(m); // forward the message to any listeners
            }
            break;
            case MESSAGEID::Remove_Object: {
                unsigned id;
                m.data().get(PARAMETERID::ObjectId, id);
                removeObject(id);
                message().emit(m); // forward the message to any listeners
            }
            break;
            case MESSAGEID::Daq_Write_Value: { // set a value in an object - OPC write value
                unsigned id;
                m.data().get(PARAMETERID::ObjectId, id);
            }
            break;
            case MESSAGEID::Daq_Exec_Method: { // get object to exec method
            }
            break;
            case MESSAGEID::Daq_Start:
                start();
                break;
            case MESSAGEID::Daq_Stop:
                stop();
                break;
            case MESSAGEID::Daq_Restart: // restart the DAQ process
                start();
                stop();
                break;
            case MESSAGEID::PowerOff:
                stop();
                message().emit(m); // forward the message to any listeners
                break;
            default:
                break;
            }
        }
    }
    CATCH_DEF
    return true;
}
