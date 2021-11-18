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
#ifndef SQLITEDB_H
#define SQLITEDB_H
#include "mrllib_global.h"
#include <wx/thread.h>
#include <wx/datetime.h>
#include <wx/log.h>
#include <string>
#include <sqlite3.h>

namespace MRL {
//SQLite Header and library

class SQLITECONNECTIONOBJECT;

/*Interface class for Result data of query*/
/*!
    \brief The IResult class
    Result of a query
*/
class MRLLIBSHARED_EXPORT IResult {
public:
    /*  This function return of count of column
        present in result set of last excueted query*/
    virtual int     GetColumnCount() = 0;

    /*Get the next coloumn name*/
    virtual const char  *NextColumnName(int iClmnCount) = 0;

    /*  This function returns TRUE if still rows are
        der in result set of last excueted query FALSE
        if no row present*/
    virtual bool  Next() = 0;

    /*Get the next coloumn data*/
    virtual const char  *ColumnData(int clmNum) = 0;

    /*RELEASE all result set as well as RESET all data*/
    virtual void Release() = 0;
};






/*!
    \brief The SQLiteDB class
    C++ wrapper for SQLITE
*/
class MRLLIBSHARED_EXPORT SQLiteDB : public IResult {

public:
    SQLiteDB();
    ~SQLiteDB();

    /*Open Connection*/
    bool OpenConnection(std::string DatabaseName, std::string DatabaseDir, int cache = 0);


    /*Close Connection*/
    void CloseConnection();

    /*Query Wrapper*/
    /*For large insert operation Memory Insert option for SQLLITE dbJournal*/
    void BeginTransaction();
    void CommitTransaction();

    /*  This Method called when SELECT Query to be excuted.
        Return RESULTSET class pointer on success else NULL of failed*/
    IResult  *ExecuteSelect(const std::string &Query);

    /*  This Method called when INSERT/DELETE/UPDATE Query to be excuted.
        Return UINT count of effected data on success*/
    unsigned   Execute(const std::string &Query);


    /*Get Last Error of excution*/
    std::string GetLastError();

    /*Return TRUE if databse is connected else FALSE*/
    bool  isConnected() ;

    sqlite3       *getConnection() const;
    void getErrString();

protected:
    /*SQLite Connection Object*/


    //SQLite Connection Details
    SQLITECONNECTIONOBJECT   *pSQLiteConn = nullptr;


    bool    m_bConnected = false;      /*Is Connected To DB*/
    bool    m_bConsole = false;        /*If Console Application*/
    std::string  m_strLastError;    /*Last Error String*/
    int     m_iColumnCount = 0;    /*No.Of Column in Result*/


private:
    /*  This function return of count of column
        present in result set of last excueted query*/
    int     GetColumnCount();

    /*Get the next coloumn name*/
    const char *NextColumnName(int iClmnCount);

    /*  This function returns TRUE if still rows are
        der in result set of last excueted query FALSE
        if no row present*/
    bool  Next();

    /*Get the next coloumn data*/
    const char  *ColumnData(int clmNum);

    /*RELEASE all result set as well as RESET all data*/
    void Release();

};

class MRLLIBSHARED_EXPORT SqlLiteStatement {
    SQLiteDB *_db = nullptr;
    sqlite3_stmt *_stmt = nullptr;
    int _columnCount = 0;
public:
    /*!
        \brief SqlLiteStatement
        \param db
        \param s
    */
    SqlLiteStatement(SQLiteDB *db = nullptr, const std::string &s = std::string()): _db(db) {
        if (_db && s.size()) prepare(s);
    }
    /*!
    */
    ~SqlLiteStatement() {
        if (_stmt) sqlite3_finalize(_stmt);
    }

    /*!
        \brief setDatabase
        \param d
    */
    void setDatabase(SQLiteDB *d) {
        _db = d;
    }

    /*!
        \brief columnCount
        \return
    */
    int columnCount() const {
        return _columnCount;
    }
    /*!
        \brief columnName
        \param i
        \return
    */
    const char *columnName(int i) {
        if (ok() && (i > 0) && (i <= _columnCount)) {
            return sqlite3_column_name(_stmt, i);
        }
        return "";
    }

    /*!
        \brief data
        \param i
        \return
    */
    const char *data(int i) {
        if (ok() && (i >= 0) && (i < _columnCount)) {
            return (const char *)sqlite3_column_text(_stmt, i);
        }
        return "";
    }

    /*!
        \brief prepare
        \param s
        \return
    */
    bool prepare(const std::string &s) {
        if (_db != nullptr) {
            if (sqlite3_prepare_v2(_db->getConnection(), s.c_str(), s.size(), &_stmt, 0) == SQLITE_OK) {
                _columnCount = sqlite3_column_count(_stmt);
                return true;
            }
        }
        _db->getErrString();
        std::cerr << " Failed to prepare " << s << " Error = " << _db->GetLastError() << std::endl;
        return false;
    }

    /*!
        \brief step
        \return
    */
    bool next() {
        if (ok()) {
            int rv = sqlite3_step(_stmt);
            return (rv == SQLITE_ROW);
        }
        return false;
    }

    bool exec() {
        if (ok()) {
            int rv = sqlite3_step(_stmt);
            return (rv == SQLITE_DONE);
        }
        return false;
    }

    /*!
        \brief reset
    */
    void reset() {
        if (_stmt) sqlite3_reset(_stmt);
    }

    /*!
        \brief clear
    */
    void clear() {
        if (ok()) sqlite3_clear_bindings(_stmt);
    }

    /*!
        \brief ok
        \return
    */
    bool ok() {
        return _db && _stmt;
    }

    /*!
        \brief bindText
        \param i
        \param s
        \return
    */

    bool bindText(int i, const char *s) {
        return (i > 0) && s && ok() && (sqlite3_bind_text(_stmt, i, s, strlen(s), SQLITE_STATIC) == SQLITE_OK);
    }


    /*!
        \brief bindString
        \param i
        \param s
        \return
    */
    bool bindString(int i, const std::string &s) {
        return (i > 0) && ok() && (sqlite3_bind_text(_stmt, i, s.c_str(), s.size(), SQLITE_TRANSIENT) == SQLITE_OK);
    }

    /*!
        \brief bindInt
        \param i
        \param v
        \return
    */
    bool bindInt(int i, int v) {
        return (i > 0) && ok() && (sqlite3_bind_int(_stmt, i, v) != SQLITE_OK);
    }

    /*!
        \brief bindInt
        \param i
        \param v
        \return
    */
    bool bindUnsigned(int i, unsigned v) {
        return (i > 0) && ok() && (sqlite3_bind_int64(_stmt, i, sqlite3_int64(v)) != SQLITE_OK);
    }

    /*!
        \brief bindDouble
        \param i
        \param v
        \return
    */
    bool bindDouble(int i, double v) {
        return (i > 0) && ok() && (sqlite3_bind_double(_stmt, i, v) != SQLITE_OK);
    }

    /*!
        \brief bindNull
        \param i
        \return
    */
    bool bindNull(int i) {
        return (i > 0) && ok() && (sqlite3_bind_null(_stmt, i) != SQLITE_OK);
    }

    /*!
        \brief bindDateTime
        \param i
        \param t
        \return
    */
    bool bindDateTime(int i, const wxDateTime &t) {
        std::string s = t.FormatISOCombined().ToStdString();
        return bindString(i, s);
    }


    bool bindDateTime(int i, time_t &t) {
        wxDateTime dt(t);
        return bindDateTime(i,dt);
    }


    /*!
        \brief lastError
        \return
    */
    const char *lastError() {
        return _db ? sqlite3_errmsg(_db->getConnection()) : "No Database";
    }

};
}

#endif // SQLITEDB_H
