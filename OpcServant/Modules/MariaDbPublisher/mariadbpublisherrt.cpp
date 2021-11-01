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
#include "mariadbpublisherrt.h"
#include <Common/Daq/daqcommon.h>
#include <Common/messageids.h>
#include <Common/stockobjectids.h>
#include <MrlLib/variantpropertytree.h>
#include <open62541cpp/open62541server.h>

/*!
    \brief MRL::MariaDbPublisherRt::MariaDbPublisherRt
*/
MRL::MariaDbPublisherRt::MariaDbPublisherRt(int id)  : RTObject(id) {
    Daq::instance()->publishMessage().connect(this, &MariaDbPublisherRt::publishQueueItem);
    MRL::Common::instance()->messageNotify().connect(this,&MRL::MariaDbPublisherRt::handleMessage);
    MRL::Common::instance()->auditNotify().connect(this,&MRL::MariaDbPublisherRt::handleAudit);
    MRL::Common::instance()->diagnosticNotify().connect(this,&MRL::MariaDbPublisherRt::handleDiagnostic);
}

/*!
    \brief Mainframe::logMessage
    \param m
*/
void MRL::MariaDbPublisherRt::handleMessage(const MRL::LogMessage &m) {
    // format the message and put into message buffer
    std::stringstream os;
    os << std::ctime(&m._ts) << ":"  <<   m._state << ":"  <<  m._source << ":" << m._msg;
    _db.writeLog(m._ts,m._source,m._state,m._msg);
}

/*!
 * \brief MRL::MariaDbPublisherRt::handleAudit
 * \param m
 */
void MRL::MariaDbPublisherRt::handleAudit(const MRL::LogMessage &m) {
    // format the message and put into message buffer
    std::stringstream os;
    os << std::ctime(&m._ts) << ":"  <<   m._state << ":"  <<  m._source << ":" << m._msg;
    _db.writeAudit(m._ts,m._source,m._state,m._msg);
}

/*!
 * \brief MRL::MariaDbPublisherRt::handleDiagnostic
 * \param m
 */
void MRL::MariaDbPublisherRt::handleDiagnostic(const MRL::LogMessage &m) {
    // format the message and put into message buffer
    std::stringstream os;
    os << std::ctime(&m._ts) << ":"  <<   m._state << ":"  <<  m._source << ":" << m._msg;
    _db.writeDiagnostic(m._ts,m._source,m._state,m._msg);
}


/*!
    \brief start
*/
void MRL::MariaDbPublisherRt::start() {
    RTObject::start();
    // get the database configuration
    //
    setMeasureInterval(10);
    setPublishInterval(0);
    // Get the configuration
    //
    std::string ht = configuration().getAsString("Host");
    std::string db = configuration().getAsString("Database");
    int         pt = configuration().getValue<int>("Port");
    std::string us = configuration().getAsString("Username");
    std::string ps = configuration().getAsString("Password");
    bool ei = configuration().getValue<bool>("Enabled");
    setEnabled(ei);
    if (ei) {
        _db.set(ht, db, us, ps, pt); // set the data base
        if (!_db.createTables()) { // initialise
            _lastStatus = "Create Table Fail";
            updateObject();
            setEnabled(false);
        }
    }

}
/*!
    \brief stop
*/
void MRL::MariaDbPublisherRt::stop() {
    if (enabled()) {
        _db.flush();
        _db.close();
    }
    RTObject::stop();
}

/*!
    \brief publish
*/
void MRL::MariaDbPublisherRt::publish() {
    //RTObject::publish();
}
/*!
    \brief measure
*/
void MRL::MariaDbPublisherRt::measure() {
    RTObject::measure();
    // flush to database - open - write - close
    if (_db.flush()) {
        if (_lastStatus != "Ok") {
            _lastStatus = "Ok";
            updateObject();
        }
    }
    else {
        _lastStatus = "Error";
        updateObject();
    }
}

void MRL::MariaDbPublisherRt::publishQueueItem(const MRL::Message &msg) {
    Message &m = const_cast<Message &>(msg);
    switch (m.id()) {
        case MESSAGEID::Publish_Record:
            if (enabled()) {
                unsigned id = 0;
                m.data().get(PARAMETERID::ObjectId, id);
                // unpack the message
                DataNodeIdMap &mm = Common::configuration().idMap();
                DataNode *d  = mm[id];
                if (d) {
                    auto o =  ObjectManager::find(d->data().type());
                    if (o) {


                        int state = 0;
                        time_t ts = 0;
                        std::string v;
                        std::string src;
                        //
                        RtObjectRef &j = MRL::Common::daq().objects()[id];
                        j->path().toString(src);

                         // set the source of the message
                        m.data().asString(PARAMETERID::Record, v);
                        m.data().get(PARAMETERID::Status, state);
                        m.data().get(PARAMETERID::Timestamp, ts);

                        // get the state as a string
                        std::string st = _("Ok").ToStdString();
                        switch (state) {
                            case STATES::States::StateFault: {
                                st = _("Fault").ToStdString();
                            }
                            break;
                            case STATES::States::StateAction: {
                                st = _("Action").ToStdString();
                            }
                            break;
                            case STATES::States::StateAlert: {
                                st = _("Alert").ToStdString();
                            }
                            default:
                                break;
                        }

                        _db.writeRecord(ts, src, o->name(), st, v);
                    }
                }
            }
            break;
        case MESSAGEID::Publish_Object: {
            if (enabled()) {
                // unpack the message
                unsigned id = 0;
                int state = 0;
                time_t ts = 0;
                std::string v;
                double dv = 0.0;
                std::string tag;
                //
                m.data().get(PARAMETERID::ObjectId, id); // set the source of the message
                m.data().asString(PARAMETERID::Value, v);
                m.data().get(PARAMETERID::Tag, tag); // get any tag value - blank if nonexistant
                m.data().get(PARAMETERID::Status, state);
                m.data().get(PARAMETERID::Timestamp, ts);
                //
                if (m.data().isType<double>(PARAMETERID::Value)) {
                    m.data().get(PARAMETERID::Value, dv);
                }
                // get source as string

                // get object type as string
                DataNodeIdMap &mm = Common::configuration().idMap();
                DataNode *d  = mm[id];
                if (d) {
                    auto o =  ObjectManager::find(d->data().type());
                    if (o) {
                        std::string src;
                        std::string st = _("Ok").ToStdString();
                        switch (state) {
                            case STATES::States::StateAction: {
                                st = _("Action").ToStdString();
                            }
                            break;
                            case STATES::States::StateAlert: {
                                st = _("Alert").ToStdString();
                            }
                            default:
                                break;
                        }

                        //
                        RtObjectRef &j = MRL::Common::daq().objects()[id];
                        j->path().toString(src);
                        if (!tag.empty()) {
                            src += TAG_SEPERATOR;
                            src += tag;
                        }
                        // get the state as a string
                        _db.writeData(ts, src, o->name(), st, v, dv);
                    }
                }
            }
        }
        break;
        default:
            break;
    }

}

/*!
    \brief MRL::SimulatorRT::updateOpc
    \param server
    \param objectNode
*/
void MRL::MariaDbPublisherRt::updateOpc(Open62541::Server &server, Open62541::NodeId &/*objectNode*/) {
    // update the value
    Open62541::Variant v(_lastStatus);
    // find the child node called Value
    server.writeValue(_statusNode, v);
}


/*!
    \brief MRL::SimulatorRT::initialiseOpc
    \param server
    \param objectNode
*/
void MRL::MariaDbPublisherRt::initialiseOpc(Open62541::Server &server, Open62541::NodeId &node) {
    // set up the variables
    server.getChild(node, "Status", _statusNode);
}

