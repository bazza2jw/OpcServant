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

#include "storagedatabase.h"

/*!
    \brief MRL:StorageDatabase::StorageDatabase
*/
MRL::StorageDatabase::StorageDatabase(const std::string &base)
    : _dataWrite(&_db),
      _logWrite(&_db),
      _auditWrite(&_db),
      _diagnosticWrite(&_db),
      _recordWrite(&_db),
      _baseName(base) {

}
/*!
    \brief MRL:StorageDatabase::createTables
*/
bool MRL::StorageDatabase::createTables() {
    try {
        if (_db.connect(_database.c_str(), _host.c_str(), _username.c_str(), _password.c_str(), _port)) { // open it
            // initialise
            std::string cmd =       "CREATE TABLE IF NOT EXISTS " + _baseName + "_DATA_TABLE "
                                    "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
                                    "F_TYPE VARCHAR(32), F_STATE VARCHAR(32),"
                                    "F_VALUE VARCHAR(32),F_DOUBLEVALUE DOUBLE, F_ALIAS VARCHAR(256));";
            _db.query(cmd).execute();
            //
            // event messages
            cmd =  "CREATE TABLE IF NOT EXISTS " + _baseName + "_LOG_TABLE"
                   "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
                   "F_TYPE VARCHAR(32), F_MESSAGE VARCHAR(256));";
            _db.query(cmd).execute();
            // auditing - log on/off parameter change etc - probably not needed
            cmd =  "CREATE TABLE IF NOT EXISTS " + _baseName + "_AUDIT_TABLE"
                   "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
                   "F_TYPE VARCHAR(32), F_MESSAGE VARCHAR(256));";
            _db.query(cmd).execute();

            cmd =  "CREATE TABLE IF NOT EXISTS " + _baseName + "_DIAGNOSTIC_TABLE"
                   "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
                   "F_TYPE VARCHAR(32), F_MESSAGE VARCHAR(256));";
            _db.query(cmd).execute();


            //
            // Records written as JSON or CSV
            //
            cmd =       "CREATE TABLE IF NOT EXISTS " + _baseName + "_RECORD_TABLE "
                                    "(F_TIMESTAMP DATETIME, F_SOURCE VARCHAR(256),"
                                    "F_TYPE VARCHAR(32), F_STATE VARCHAR(32),"
                                    "F_RECORD VARCHAR(512),F_ALIAS VARCHAR(256));";
            _db.query(cmd).execute();


            cmd = "CREATE INDEX IF NOT EXISTS " + _baseName + "_DATA_TABLE_INDEX ON " + _baseName + "_DATA_TABLE (F_SOURCE,F_TIMESTAMP);";
            _db.query(cmd).execute();

            cmd = "CREATE INDEX IF NOT EXISTS " + _baseName + "_DATA_TABLE_ALIAS_INDEX ON " + _baseName + "_DATA_TABLE (F_ALIAS,F_TIMESTAMP);";
            _db.query(cmd).execute();

            cmd = "CREATE INDEX IF NOT EXISTS " + _baseName + "_RECORD_TABLE_INDEX ON " + _baseName + "_RECORD_TABLE (F_SOURCE,F_TIMESTAMP);";
            _db.query(cmd).execute();

            cmd = "CREATE INDEX IF NOT EXISTS " + _baseName + "_RECORD_TABLE_ALIAS_INDEX ON " + _baseName + "_RECORD_TABLE (F_ALIAS,F_TIMESTAMP);";
            _db.query(cmd).execute();

            cmd = "CREATE INDEX IF NOT EXISTS " + _baseName + "_LOG_TABLE_INDEX ON " + _baseName + "_LOG_TABLE (F_SOURCE, F_TIMESTAMP);";
            _db.query(cmd).execute();

            cmd = "CREATE INDEX IF NOT EXISTS " + _baseName + "_AUDIT_TABLE_INDEX ON " + _baseName + "_AUDIT_TABLE (F_SOURCE, F_TIMESTAMP);";
            _db.query(cmd).execute();

            cmd = "CREATE INDEX IF NOT EXISTS " + _baseName + "_DIAGNOSTIC_TABLE_INDEX ON " + _baseName + "_DIAGNOSTIC_TABLE (F_SOURCE, F_TIMESTAMP);";
            _db.query(cmd).execute();


            //
            // Prepare
            //
            _dataWrite.reset();
            cmd = "INSERT INTO " + _baseName +
                  "_DATA_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_STATE,F_VALUE,F_DOUBLEVALUE,F_ALIAS)"
                  "VALUES (%0q,%1q,%2q,%3q,%4q,%5,%6q);";
            _dataWrite = _db.query(cmd);
            _dataWrite.parse();
            _logWrite.reset();
            //
            cmd = "INSERT INTO " + _baseName +
                  "_LOG_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_MESSAGE) VALUES(%0q,%1q,%2q,%3q);";
            _logWrite =  _db.query(cmd);
            _logWrite.parse();
            //
            _auditWrite.reset();
            cmd = "INSERT INTO " + _baseName +
                  "_AUDIT_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_MESSAGE) VALUES(%0q,%1q,%2q,%3q);";
            _auditWrite = _db.query(cmd);
            _auditWrite.parse();

            _diagnosticWrite.reset();
            cmd = "INSERT INTO " + _baseName +
                  "_DIAGNOSTIC_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_MESSAGE) VALUES(%0q,%1q,%2q,%3q);";
            _diagnosticWrite = _db.query(cmd);
            _diagnosticWrite.parse();


            //
            _recordWrite.reset();
            cmd = "INSERT INTO " + _baseName +
                  "_RECORD_TABLE (F_TIMESTAMP,F_SOURCE,F_TYPE,F_STATE,F_VALUE,F_ALIAS)"
                  "VALUES (%0q,%1q,%2q,%3q,%4q,%5q);";
            _recordWrite = _db.query(cmd);
            _recordWrite.parse();


            //
            _db.disconnect();
            return true;
        }
    }
    catch (mysqlpp::Exception &e) {
        wxLogDebug("Database Exception in %s: %s", __FUNCTION__, e.what());
    }
    return false;
}

/*!
    \brief writeRecord
    \return true on success
*/
bool MRL::StorageDatabase::writeRecord(time_t timestamp, const std::string &source, const std::string &type,
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
bool MRL::StorageDatabase::writeData(time_t timestamp, const std::string &source, const std::string &type,
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
bool MRL::StorageDatabase::writeLog(time_t timestamp, const std::string &source,
                                    const std::string &type, const std::string &message) {
    if (_log.size() < MaxQueueSize) {
        _log.push(WriteLog(timestamp, source, type, message));
        return true;
    }
    return false;
}

/*!
 * \brief MRL::StorageDatabase::writeAudit
 * \param timestamp
 * \param source
 * \param type
 * \param message
 * \return true on success
 */
bool MRL::StorageDatabase::writeAudit(time_t timestamp, const std::string &source,
                                    const std::string &type, const std::string &message) {
    if (_audit.size() < MaxQueueSize) {
        _audit.push(WriteLog(timestamp, source, type, message));
        return true;
    }
    return false;
}

/*!
 * \brief MRL::StorageDatabase::writeDiagnostic
 * \param timestamp
 * \param source
 * \param type
 * \param message
 * \return true on success
 */
bool MRL::StorageDatabase::writeDiagnostic(time_t timestamp, const std::string &source,
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
bool MRL::StorageDatabase::flush() {
    // write data in queue to database as a batch - this is more efficent - probably
    if (open()) {
        try {
            // flush data queue
            while (_data.size() > 0) {
                WriteData &d = _data.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                std::string k = Common::reverseAliasMap()[d.source];
                if(k.empty()) k = d.source;
                _dataWrite.execute(ts, d.source, d.type, d.state, d.value, d.doubleValue,k);
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
                _recordWrite.execute(ts, d.source, d.type, d.state, d.record,k);
                _record.pop();
            }


            // flush log queue
            while (_log.size()) {
                WriteLog &d = _log.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                _logWrite.execute(ts, d.source, d.type, d.message);
                _log.pop();
            }


            // flush audit queue
            while (_audit.size()) {
                WriteLog &d = _audit.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                _auditWrite.execute(ts, d.source, d.type, d.message);
                _audit.pop();
            }

            // flush diagnostic queue
            while (_diagnostic.size()) {
                WriteLog &d = _diagnostic.front();
                // format time stamp as iso
                wxDateTime dt(d.timestamp);
                std::string ts = dt.FormatISOCombined().ToStdString();
                _diagnosticWrite.execute(ts, d.source, d.type, d.message);
                _diagnostic.pop();
            }
            close();
            return true;
        }
        catch (mysqlpp::Exception &e) {
            wxLogDebug("Database Exception in %s: %s", __FUNCTION__, e.what());
        }

    }
    return false;
}
