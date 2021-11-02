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
#include "localdb.h"
#include <Common/Daq/daq.h>
#include <Common/Daq/daqcommon.h>
#include <Common/objectmanager.h>


/*!
 * \brief MRL::LocalDb::LocalDb
 * The local database is an SQLITE database used to store data for report generation and for data scrolling in displays
 * The objective is that it is local to the application and independent of any network
 */
MRL::LocalDb::LocalDb(const std::string &dir, const std::string &name) : BObject(0),_dir(dir), _name(name)
{
    Daq::instance()->publishMessage().connect(this, &MRL::LocalDb::publishQueueItem);
    Daq::instance()->oneSecondTimer().connect(this, &MRL::LocalDb::handleOneSecond);
    MRL::Common::instance()->messageNotify().connect(this,&MRL::LocalDb::handleMessage);
    MRL::Common::instance()->auditNotify().connect(this,&MRL::LocalDb::handleAudit);
    MRL::Common::instance()->diagnosticNotify().connect(this,&MRL::LocalDb::handleDiagnostic);
    setEnabled(true);
    //
    _db = std::make_unique<SQLiteDB>();
    // create the
    _db->OpenConnection(name,dir);
    //
    // now create the schema if necessary
    //
    std::string cmd =       "CREATE TABLE IF NOT EXISTS DATA_TABLE "
                            "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
                            "F_TYPE VARCHAR(32), F_STATE VARCHAR(32),"
                            "F_VALUE VARCHAR(32),F_DOUBLEVALUE DOUBLE, F_ALIAS VARCHAR(256));";
    _db->Execute(cmd);
    //
    cmd =  "CREATE TABLE IF NOT EXISTS LOG_TABLE"
           "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
           "F_TYPE VARCHAR(32), F_MESSAGE VARCHAR(256));";
    _db->Execute(cmd);
    //
    cmd =  "CREATE TABLE IF NOT EXISTS AUDIT_TABLE"
           "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
           "F_TYPE VARCHAR(32), F_MESSAGE VARCHAR(256));";
    _db->Execute(cmd);
    //
    cmd =  "CREATE TABLE IF NOT EXISTS DIAGNOSTIC_TABLE"
           "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
           "F_TYPE VARCHAR(32), F_MESSAGE VARCHAR(256));";
    _db->Execute(cmd);
    //
    // Records written as JSON or CSV
    //
    cmd =       "CREATE TABLE IF NOT EXISTS RECORD_TABLE "
                            "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
                            "F_TYPE VARCHAR(32), F_STATE VARCHAR(32),"
                            "F_RECORD VARCHAR(512), F_ALIAS VARCHAR(256));";
    _db->Execute(cmd);


    cmd = "CREATE INDEX IF NOT EXISTS DATA_TABLE_INDEX ON DATA_TABLE (F_SOURCE,F_TIMESTAMP);";
    _db->Execute(cmd);
    cmd = "CREATE INDEX IF NOT EXISTS DATA_TABLE_ALIAS_INDEX ON DATA_TABLE (F_ALIAS,F_TIMESTAMP);";
    _db->Execute(cmd);

    cmd = "CREATE INDEX IF NOT EXISTS RECORD_TABLE_INDEX ON RECORD_TABLE (F_SOURCE,F_TIMESTAMP);";
    _db->Execute(cmd);
    cmd = "CREATE INDEX IF NOT EXISTS RECORD_TABLE_INDEX ON RECORD_TABLE (F_ALIAS,F_TIMESTAMP);";
    _db->Execute(cmd);

    cmd = "CREATE INDEX IF NOT EXISTS LOG_TABLE_INDEX ON LOG_TABLE (F_SOURCE, F_TIMESTAMP);";
    _db->Execute(cmd);
    cmd = "CREATE INDEX IF NOT EXISTS AUDIT_TABLE_INDEX ON AUDIT_TABLE (F_SOURCE, F_TIMESTAMP);";
    _db->Execute(cmd);
    cmd = "CREATE INDEX IF NOT EXISTS DIAGNOSTIC_TABLE_INDEX ON DIAGNOSTIC_TABLE (F_SOURCE, F_TIMESTAMP);";
    _db->Execute(cmd);
    //
    // Create the prepared statement
    //
    cmd = "INSERT INTO DATA_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_STATE,F_VALUE,F_DOUBLEVALUE,F_ALIAS) VALUES (?,?,?,?,?,?,?);";
    _dataWrite = std::make_unique<SqlLiteStatement>(_db.get(),cmd);
    //
    cmd = "INSERT INTO LOG_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_MESSAGE) VALUES(?,?,?,?);";
    _logWrite =  std::make_unique<SqlLiteStatement>(_db.get(),cmd);
    //
    cmd = "INSERT INTO AUDIT_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_MESSAGE) VALUES(?,?,?,?);";
    _auditWrite =  std::make_unique<SqlLiteStatement>(_db.get(),cmd);
    //
    cmd = "INSERT INTO DIAGNOSTIC_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_MESSAGE) VALUES(?,?,?,?);";
    _diagnosticWrite =  std::make_unique<SqlLiteStatement>(_db.get(),cmd);
    //
    cmd = "INSERT INTO RECORD_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_STATE,F_RECORD,F_ALIAS) VALUES (?,?,?,?,?,?);";
    _recordWrite =  std::make_unique<SqlLiteStatement>(_db.get(),cmd);
    //
    _db->CloseConnection();
}

/*!
 * \brief MRL::LocalDb::~LocalDb
 */
MRL::LocalDb::~LocalDb()
{
    if(_db)
    {
        _db->CloseConnection();
    }
}


/*!
 * \brief MRL::LocalDb::handleOneSecond
 * \param t
 */
void MRL::LocalDb::handleOneSecond(time_t t)
{
    flush();
}
/*!
 * \brief MRL::LocalDb::purgeDiagnostic
 */
void MRL::LocalDb::purgeDiagnostic()
{
    _db->BeginTransaction();
    _db->Execute("DELETE FROM DIAGNOSTIC_TABLE WHERE F_TIMESTAMP < date('now','start of month','-3 month');");
    _db->CommitTransaction();
}
/*!
 * \brief MRL::LocalDb::purgeMessage
 */
void MRL::LocalDb::purgeMessage()
{
    _db->BeginTransaction();
    _db->Execute("DELETE FROM LOG_TABLE WHERE F_TIMESTAMP < date('now','start of month','-3 month');");
    _db->CommitTransaction();
}
/*!
 * \brief MRL::LocalDb::purgeAudit
 */
void MRL::LocalDb::purgeAudit()
{
    _db->BeginTransaction();
    _db->Execute("DELETE FROM PURGE_TABLE WHERE F_TIMESTAMP < date('now','start of month','-3 month');");
    _db->CommitTransaction();
}
/*!
 * \brief MRL::LocalDb::purgeData
 */
void MRL::LocalDb::purgeData()
{
    _db->BeginTransaction();
    _db->Execute("DELETE FROM DATA_TABLE WHERE F_TIMESTAMP < date('now','start of month','-3 month');");
    _db->CommitTransaction();
}
/*!
 * \brief MRL::LocalDb::purgeAll
 */
void MRL::LocalDb::purgeAll()
{
    purgeDiagnostic();
    purgeMessage();
    purgeAudit();
    purgeData();
}


/*!
 * \brief MRL::LocalDb::publishQueueItem
 * \param msg
 */
void MRL::LocalDb::publishQueueItem(const MRL::Message &msg) {
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

                        writeRecord(ts, src, o->name(), st, v);
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
                        writeData(ts, src, o->name(), st, v, dv);
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
 * \brief handleMessage
 */
void MRL::LocalDb::handleMessage(const MRL::LogMessage &m)
{
    // format the message and put into message buffer
    std::stringstream os;
    os << std::ctime(&m._ts) << ":"  <<   m._state << ":"  <<  m._source << ":" << m._msg;
    writeLog(m._ts,m._source,m._state,m._msg);
}
/*!
 * \brief MRL::LocalDb::handleAudit
 * \param m
 */
void MRL::LocalDb::handleAudit(const MRL::LogMessage &m)
{
    // format the message and put into message buffer
    std::stringstream os;
    os << std::ctime(&m._ts) << ":"  <<   m._state << ":"  <<  m._source << ":" << m._msg;
    writeAudit(m._ts,m._source,m._state,m._msg);
}
/*!
 * \brief MRL::LocalDb::handleDiagnostic
 * \param m
 */
void MRL::LocalDb::handleDiagnostic(const MRL::LogMessage &m)
{
    // format the message and put into message buffer
    std::stringstream os;
    os << std::ctime(&m._ts) << ":"  <<   m._state << ":"  <<  m._source << ":" << m._msg;
    writeDiagnostic(m._ts,m._source,m._state,m._msg);
}

/*!
    \brief writeRecord
    \return
*/
bool MRL::LocalDb::writeRecord(time_t timestamp, const std::string &source, const std::string &type,
               const std::string &state, const std::string &value) {
    if (_record.size() < MaxQueueSize) {
        _record.push(WriteRecord(timestamp, source, type, state, value));
        return true;
    }
    return false;
}

/*!
    \brief writeData
    \return true on queue
*/
bool MRL::LocalDb::writeData(time_t timestamp, const std::string &source, const std::string &type,
                                     const std::string &state, const std::string &value, double doubleValue) {
    if (_data.size() < MaxQueueSize) {
        _data.push(WriteData(timestamp, source, type, state, value, doubleValue));
        return true;
    }
    return false;
}
/*!
    \brief writeLog
    \return true on queue
*/
bool MRL::LocalDb::writeLog(time_t timestamp, const std::string &source,
                                    const std::string &type, const std::string &message) {
    if (_log.size() < MaxQueueSize) {
        _log.push(WriteLog(timestamp, source, type, message));
        return true;
    }
    return false;
}

bool MRL::LocalDb::writeAudit(time_t timestamp, const std::string &source,
                                    const std::string &type, const std::string &message) {
    if (_audit.size() < MaxQueueSize) {
        _audit.push(WriteLog(timestamp, source, type, message));
        return true;
    }
    return false;
}

bool MRL::LocalDb::writeDiagnostic(time_t timestamp, const std::string &source,
                                    const std::string &type, const std::string &message) {
    if (_diagnostic.size() < MaxQueueSize) {
        _diagnostic.push(WriteLog(timestamp, source, type, message));
        return true;
    }
    return false;
}


/*!
    \brief MRL::StorageDatabase::flush
*/
bool MRL::LocalDb::flush() {
    // write data in queue to database as a batch - this is more efficent - probably
    bool ret = false;
    _db->OpenConnection(_name,_dir,1000);
    if (_db->isConnected()) {
        try {
            _db->BeginTransaction();
            // flush data queue
            while (_data.size() > 0) {
                WriteData &d = _data.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                std::string k = Common::reverseAliasMap()[d.source];
                if(k.empty()) k = d.source;

                _dataWrite->reset();
                _dataWrite->bindString(1,ts);
                _dataWrite->bindString(2,d.source);
                _dataWrite->bindString(3,d.type);
                _dataWrite->bindString(4,d.state);
                _dataWrite->bindString(5,d.value);
                _dataWrite->bindDouble(6,d.doubleValue);
                _dataWrite->bindString(7,k);
                _dataWrite->exec();
                _data.pop();
            }

            // flush data queue
            while (_record.size() > 0) {
                WriteRecord &d = _record.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                std::string k = Common::reverseAliasMap()[d.source];
                if(k.empty()) k = d.source;
                //
                _recordWrite->reset();
                _recordWrite->bindString(1,ts);
                _recordWrite->bindString(2,d.source);
                _recordWrite->bindString(3,d.type);
                _recordWrite->bindString(4,d.state);
                _recordWrite->bindString(5,d.record);
                _recordWrite->bindString(6,k);
                _recordWrite->exec();
                _record.pop();
            }
            // flush log queue
            while (_log.size()) {
                WriteLog &d = _log.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                _logWrite->reset();
                _logWrite->bindString(1,ts);
                _logWrite->bindString(2,d.source);
                _logWrite->bindString(3,d.type);
                _logWrite->bindString(4,d.message);
                _log.pop();
            }
            // flush audit queue
            while (_audit.size()) {
                WriteLog &d = _audit.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                _auditWrite->reset();
                _auditWrite->bindString(1,ts);
                _auditWrite->bindString(2,d.source);
                _auditWrite->bindString(3,d.type);
                _auditWrite->bindString(4,d.message);
                _audit.pop();
            }
            // flush diagnostic queue
            while (_diagnostic.size()) {
                WriteLog &d = _diagnostic.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                _diagnosticWrite->reset();
                _diagnosticWrite->bindString(1,ts);
                _diagnosticWrite->bindString(2,d.source);
                _diagnosticWrite->bindString(3,d.type);
                _diagnosticWrite->bindString(4,d.message);
                _diagnostic.pop();
            }
            _db->CommitTransaction();
            ret = true;
        }
        catch(...)
        {

        }
        _db->CloseConnection();
    }
    return ret;
}


