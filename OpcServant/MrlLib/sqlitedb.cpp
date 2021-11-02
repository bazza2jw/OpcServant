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
#include "sqlitedb.h"
#include <boost/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/bind.hpp>
typedef boost::shared_mutex ReadWriteMutex;
typedef boost::shared_lock<boost::shared_mutex> ReadLock;
typedef boost::unique_lock<boost::shared_mutex> WriteLock;

/*!
 * \brief The MRL::SQLITECONNECTIONOBJECT struct
 */
struct MRL::SQLITECONNECTIONOBJECT
{
    ReadWriteMutex   _mutex;               // access mutex
    std::string		 SQLiteDatabaseName;   //Database Name
    std::string		 SQLiteDBPath;		   //Databse File Dir
    sqlite3		 *pCon = nullptr;	       //SQLite Connection Object
    sqlite3_stmt *pRes = nullptr;		   //SQLite statement object
};

/*!
 * \brief MRL::SQLiteDB::getConnection
 * \return
 */
sqlite3		 * MRL::SQLiteDB::getConnection() const
{
    return pSQLiteConn->pCon;
}


// SQLITE is used as the local database
/*!
 * \brief MRL::SQLiteDB::SQLiteDB
 */
MRL::SQLiteDB::SQLiteDB()
{
    m_bConnected=false;
    pSQLiteConn = new SQLITECONNECTIONOBJECT();
}

/*!
 * \brief MRL::SQLiteDB::~SQLiteDB
 */
MRL::SQLiteDB::~SQLiteDB()
{
    CloseConnection();
    delete pSQLiteConn;
}


void MRL::SQLiteDB::getErrString() {
    m_strLastError = std::string(sqlite3_errmsg(pSQLiteConn->pCon));
}
/*!
 * \brief MRL::SQLiteDB::CloseConnection
 */
void MRL::SQLiteDB::CloseConnection()
{
    WriteLock m(pSQLiteConn->_mutex);
    if(pSQLiteConn->pCon)
        sqlite3_close(pSQLiteConn->pCon);
}

/*!
 * \brief MRL::SQLiteDB::GetLastError
 * \return
 */
std::string MRL::SQLiteDB::GetLastError()
{
    ReadLock m(pSQLiteConn->_mutex);
    return m_strLastError;
}

/*!
 * \brief MRL::SQLiteDB::isConnected
 * \return
 */
bool   MRL::SQLiteDB::isConnected()
{
    ReadLock m(pSQLiteConn->_mutex);
    return m_bConnected;
}

/*!
 * \brief MRL::SQLiteDB::OpenConnection
 * \param DatabaseName
 * \param DatabaseDir
 * \return
 */
bool MRL::SQLiteDB::OpenConnection(std::string DatabaseName, std::string DatabaseDir, int cache)
{
    WriteLock m(pSQLiteConn->_mutex);
    pSQLiteConn->SQLiteDatabaseName = DatabaseName;
    pSQLiteConn->SQLiteDBPath	    = DatabaseDir;
    m_bConnected = true;
    std::string db=pSQLiteConn->SQLiteDatabaseName;
    std::string dir=pSQLiteConn->SQLiteDBPath;
    std::string path=dir + "/" + db;
    //
    {
        int rc = sqlite3_open(path.c_str(), &(pSQLiteConn->pCon));
        m_strLastError = std::string(sqlite3_errmsg(pSQLiteConn->pCon));

        if(!rc)
        {
            if(m_strLastError.find("not an error") == std::string::npos)
                m_bConnected = false;
        }
        else
        {
            sqlite3_busy_timeout(pSQLiteConn->pCon, 100);
            if(cache > 0)
            {
                Execute("PRAGMA cache_size=" + std::to_string(cache));
            }
        }
    }
    //
    return m_bConnected;
}

/*!
 * \brief MRL::SQLiteDB::BeginTransaction
 */
void MRL::SQLiteDB::BeginTransaction()
{
    WriteLock m(pSQLiteConn->_mutex);
    sqlite3_exec(pSQLiteConn->pCon, "BEGIN TRANSACTION", NULL, NULL,NULL);
    getErrString();

}
/*!
 * \brief MRL::SQLiteDB::CommitTransection
 */

void MRL::SQLiteDB::CommitTransaction()
{
    WriteLock m(pSQLiteConn->_mutex);
    sqlite3_exec(pSQLiteConn->pCon, "COMMIT TRANSACTION", NULL, NULL,NULL);
    getErrString();
}

/*!
 * \brief MRL::SQLiteDB::ExecuteSelect
 * \param Query
 * \return
 */
MRL::IResult*  MRL::SQLiteDB::ExecuteSelect(const std::string &Query)
{
    if(!isConnected())  return nullptr;
    WriteLock m(pSQLiteConn->_mutex);
    if(sqlite3_prepare_v2(pSQLiteConn->pCon,Query.c_str(),-1, &pSQLiteConn->pRes, NULL) != SQLITE_OK)
        {
            m_strLastError=sqlite3_errmsg(pSQLiteConn->pCon);
            std::cerr << Query.c_str() << " " <<  sqlite3_errmsg(getConnection()) << std::endl;
            sqlite3_finalize(pSQLiteConn->pRes);
            return nullptr;
        }
        else
        {
            m_iColumnCount   =sqlite3_column_count(pSQLiteConn->pRes);
            IResult *ires=this;
            return ires;
        }
}

/*!
 * \brief MRL::SQLiteDB::Execute
 * \param Query
 */
unsigned MRL::SQLiteDB::Execute(const std::string &Query)
{  if(!isConnected())  return 0;
    WriteLock m(pSQLiteConn->_mutex);

    m_strLastError="";

    static char estr[2] = {0,0};
    char *err = estr;

    if(sqlite3_exec(pSQLiteConn->pCon, Query.c_str(), nullptr, 0, &err) != SQLITE_OK)
    {
        std::cerr << Query.c_str() << " " <<  sqlite3_errmsg(getConnection()) << std::endl;
        return 0;
    }
    return sqlite3_total_changes(pSQLiteConn->pCon);
}


/*Result Set Definations*/
/*!
 * \brief MRL::SQLiteDB::GetColumnCount
 * \return
 */
int	MRL::SQLiteDB::GetColumnCount()
{
    return m_iColumnCount;
}

/*!
 * \brief MRL::SQLiteDB::NextColumnName
 * \param iClmnCount
 * \return
 */
const char* MRL::SQLiteDB::NextColumnName(int iClmnCount)
{
    WriteLock m(pSQLiteConn->_mutex);
    if(iClmnCount > m_iColumnCount) return "";
    return sqlite3_column_name(pSQLiteConn->pRes,iClmnCount);
}

/*!
 * \brief MRL::SQLiteDB::Next
 * \return
 */
bool MRL::SQLiteDB:: Next()
{
    WriteLock m(pSQLiteConn->_mutex);
    return (sqlite3_step(pSQLiteConn->pRes)==SQLITE_ROW) ? true : false;
}

/*!
 * \brief MRL::SQLiteDB::ColumnData
 * \param clmNum
 * \return
 */
const char*  MRL::SQLiteDB::ColumnData(int clmNum)
{
    if(clmNum > m_iColumnCount)   return "";
    WriteLock m(pSQLiteConn->_mutex);
    return ((const char*)sqlite3_column_text(pSQLiteConn->pRes,clmNum));
}

/*!
 * \brief MRL::SQLiteDB::Release
 */
void MRL::SQLiteDB::Release()
{
    WriteLock m(pSQLiteConn->_mutex);
    sqlite3_finalize(pSQLiteConn->pRes);
    m_iColumnCount=0;
    m_strLastError="";
}






