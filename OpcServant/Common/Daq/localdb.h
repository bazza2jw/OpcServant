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
#ifndef LOCALDB_H
#define LOCALDB_H
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/bobject.h>
#include <Common/storagedatabase.h>
#include <queue>
#include <MrlLib/sqlitedb.h>


namespace MRL
{
/*!
 * \brief The LocalDb class
 */
class LocalDb : public MRL::BObject
{
    const size_t MaxQueueSize = 1000;
    std::string _dir;
    std::string _name;
    std::unique_ptr<SQLiteDB> _db;
    std::unique_ptr<SqlLiteStatement> _dataWrite;
    std::unique_ptr<SqlLiteStatement> _logWrite;
    std::unique_ptr<SqlLiteStatement> _recordWrite;
    std::unique_ptr<SqlLiteStatement> _auditWrite;
    std::unique_ptr<SqlLiteStatement> _diagnosticWrite;

    std::queue<WriteData> _data; // data queue
    std::queue<WriteLog> _log;   // log queue
    std::queue<WriteLog> _audit;   // log queue
    std::queue<WriteLog> _diagnostic;   // log queue
    std::queue<WriteRecord> _record;   // record queue

    /*!
     * \brief writeData
     * \param timestamp
     * \param source
     * \param type
     * \param state
     * \param value
     * \param doubleValue
     * \return
     */
    bool writeData(time_t timestamp, const std::string &source, const std::string &type,
                   const std::string &state, const std::string &value, double doubleValue = 0.0);

    /*!
     * \brief writeRecord
     * \param timestamp
     * \param source
     * \param type
     * \param state
     * \param value
     * \return
     */
    bool writeRecord(time_t timestamp, const std::string &source, const std::string &type,
                     const std::string &state, const std::string &value);

    /*!
     * \brief writeLog
     * \param timestamp
     * \param source
     * \param type
     * \param message
     * \return
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

    /*!
     * \brief flush
     * \return
     */
    bool flush();

    MRL::ReadWriteMutex _mutex;

public:
    //
    constexpr static const char *LOCAL_DB_DIR = OPCSERVANT_DEF_APP_DIR "/data";
    //
    LocalDb(const std::string &dir, const std::string &name);
    ~LocalDb();

    MRL::ReadWriteMutex & mutex() { return _mutex;}

    /*!
     * \brief publishQueueItem
     * \param msg
     */
    void publishQueueItem(const Message &msg);
    /*!
     * \brief handleMessage
     * \param msg
     */
    void handleMessage(const MRL::LogMessage &msg);
    void handleAudit(const MRL::LogMessage &msg);
    void handleDiagnostic(const MRL::LogMessage &msg);
    // purge the database to 90 days
    void purgeDiagnostic();
    void purgeMessage();
    void purgeAudit();
    void purgeData();
    void purgeAll();
    //
    /*!
     * \brief useOneSecondTimer
     * \return
     */
    bool useOneSecondTimer() {
        return true;
    }
    /*!
     * \brief handleOneSecond
     * \param t
     */
    void handleOneSecond(time_t t);
    /*!
     * \brief db
     * \return
     */
    std::unique_ptr<SQLiteDB> & db() {
        return _db;
    }
};
}
#endif // LOCALDB_H
