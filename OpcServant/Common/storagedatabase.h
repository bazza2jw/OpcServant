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
#ifndef STORAGEDATABASE_H
#define STORAGEDATABASE_H

#include <Common/common.h>
#include "mysql++/mysql++.h"
#include <queue>
namespace MRL {

/*!
 * \brief The WriteData struct
 */
struct WriteData {
    time_t timestamp = 0;
    std::string source;
    std::string type;
    std::string state;
    std::string value;
    double doubleValue = 0.0;
    WriteData() {}
    WriteData(time_t ts, const std::string &src, const std::string &t,
              const std::string &st, const std::string &v, double d = 0.0):
        timestamp(ts), source(src), type(t), state(st), value(v), doubleValue(d) {

    }
    WriteData(const WriteData &) = default;
};

/*!
 * \brief The WriteRecord struct
 */
struct WriteRecord {
    time_t timestamp = 0;
    std::string source;
    std::string type;
    std::string state;
    std::string record;
    WriteRecord() {}
    WriteRecord(time_t ts, const std::string &src, const std::string &t,
                const std::string &st, const std::string &v):
        timestamp(ts), source(src), type(t), state(st), record(v) {

    }
    WriteRecord(const WriteRecord &) = default;
};


/*!
 * \brief The WriteLog struct
 */
struct WriteLog {
    time_t timestamp = 0;
    std::string source;
    std::string type;
    std::string message;
    WriteLog() {}
    WriteLog(time_t ts, const std::string &src,
             const std::string &t, const std::string &m) :
        timestamp(ts), source(src), type(t), message(m) {

    }
    WriteLog(const WriteLog &) = default;
};


/*!
       \brief The StorageDatabase class
*/
class StorageDatabase {
    const size_t MaxQueueSize = 1000;
    //
    mysqlpp::Connection _db; // the connection
    mysqlpp::Query _dataWrite; // the data writer
    mysqlpp::Query _logWrite; // the log writer
    mysqlpp::Query _auditWrite; // the log writer
    mysqlpp::Query _diagnosticWrite; // the log writer
    mysqlpp::Query _recordWrite; // the record writer
    //
    // connection parameters
    std::string _host = "localhost";
    std::string _database = "DATA";
    std::string _username = "user";
    std::string _password = "password";
    int _port = 0;
    bool _enabled = false;
    //
    std::string _baseName = "OPCSERVANT";
    //
    std::queue<WriteData> _data; // data queue
    std::queue<WriteLog> _log;   // log queue
    std::queue<WriteLog> _audit;   // log queue
    std::queue<WriteLog> _diagnostic;   // log queue

    std::queue<WriteRecord> _record;   // record queue

    //
public:
    /*!
        \brief StorageDatabase
        \param base
    */
    StorageDatabase(const std::string &base = "OPCSERVANT");
    /*!
        \brief ~StorageDatabase
    */
    virtual ~StorageDatabase() {
        if (_db.connected()) _db.disconnect();
    }
    /*!
        \brief createTables
    */
    bool createTables();
    /*!
        \brief open
        \return
    */
    bool open() {
        try {
            if (_db.connected()) return true;
            return  _db.connect(_database.c_str(), _host.c_str(), _username.c_str(), _password.c_str(), _port);
        }
        catch (mysqlpp::Exception &e) {
            wxLogDebug("Database Exception in %s: %s", __FUNCTION__, e.what());
        }
        return false;
    }
    /*!
        \brief close
    */
    void close() {
        try
        {
            if (_db.connected()) _db.disconnect();
        }
        catch (mysqlpp::Exception &e) {
            wxLogDebug("Database Exception in %s: %s", __FUNCTION__, e.what());
        }

    }

    /*!
        \brief writeData
        \return
    */
    bool writeData(time_t timestamp, const std::string &source, const std::string &type,
                   const std::string &state, const std::string &value, double doubleValue = 0.0);

    /*!
        \brief writeRecord
        \return
    */
    bool writeRecord(time_t timestamp, const std::string &source, const std::string &type,
                     const std::string &state, const std::string &value);

    /*!
        \brief writeLog
        \return
    */
    bool writeLog(time_t timestamp, const std::string &source,
                  const std::string &type, const std::string &message);

    /*!
     * \brief writeAudit
     * \param timestamp
     * \param source
     * \param type
     * \param message
     * \return
     */
    bool writeAudit(time_t timestamp, const std::string &source,
                  const std::string &type, const std::string &message);

    /*!
     * \brief writeDiagnostic
     * \param timestamp
     * \param source
     * \param type
     * \param message
     * \return
     */
    bool writeDiagnostic(time_t timestamp, const std::string &source,
                  const std::string &type, const std::string &message);

    //
    /*!
        \brief set Set the database parameters
        \param h
        \param d
        \param u
        \param p
        \param port
    */
    void set(const std::string &h, const std::string &d, const std::string &u,
             const std::string &p, int port = 0) {
        _host = h;
        _database = d;
        _username = u;
        _password = p;
        _port = port;
    }

    bool flush();

};
}
#endif // STORAGEDATABASE_H
