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
#include <memory>
#include "mrllib_global.h"
#include <wx/thread.h>
#include <wx/datetime.h>
#include <wx/log.h>
#include <string>
#include <sqlite3.h>
#include <vector>
#include <list>

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
/*!
 * \brief The SqlLiteStatement class
 */
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
        \return true if db ok
    */
    bool ok() {
        return _db && _stmt;
    }
    //
    //
    std::string getString(int i) { return std::string(data(i)); }
    int getInt(int i)    {  return atoi(data(i));}
    double getDouble(int i) { return atof(data(i));}
    bool getDateTime(int i, wxDateTime &dt)
    {
        wxString s(data(i));
        return dt.ParseISOCombined(s);
    }
    //
    /*!
        \brief bindText
        \param i
        \param s
        \return true on success
    */

    bool bindText(int i, const char *s) {
        return (i > 0) && s && ok() && (sqlite3_bind_text(_stmt, i, s, strlen(s), SQLITE_STATIC) == SQLITE_OK);
    }


    /*!
        \brief bindString
        \param i
        \param s
        \return true on success
    */
    bool bindString(int i, const std::string &s) {
        return (i > 0) && ok() && (sqlite3_bind_text(_stmt, i, s.c_str(), s.size(), SQLITE_TRANSIENT) == SQLITE_OK);
    }

    /*!
        \brief bindInt
        \param i
        \param v
        \return true on success
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
        \return true on success
    */
    bool bindDouble(int i, double v) {
        return (i > 0) && ok() && (sqlite3_bind_double(_stmt, i, v) != SQLITE_OK);
    }

    /*!
        \brief bindNull
        \param i
        \return true on success
    */
    bool bindNull(int i) {
        return (i > 0) && ok() && (sqlite3_bind_null(_stmt, i) != SQLITE_OK);
    }

    /*!
        \brief bindDateTime
        \param i
        \param t
        \return true on success
    */
    bool bindDateTime(int i, const wxDateTime &t) {
        std::string s = t.FormatISOCombined().ToStdString();
        return bindString(i, s);
    }

    /*!
     * \brief bindDateTime
     * \param i
     * \param t
     * \return true on success
     */
    bool bindDateTime(int i, time_t &t) {
        wxDateTime dt(t);
        return bindDateTime(i,dt);
    }


    /*!
        \brief lastError
        \return last error string
    */
    const char *lastError() {
        return _db ? sqlite3_errmsg(_db->getConnection()) : "No Database";
    }

};
//
// this emulates a scrolling database cursor
// Requires index to have been created on key fields
// in the select the scroll key field must be first in the list
// There are two variables, the key and the record count
// All SQLITE data is stored as strings
/*
To Scroll Down
 SELECT title FROM tracks
     WHERE singer='Madonna' <--- Fixed key field
       AND title>:lasttitle <--- variable key field
     ORDER BY title
     LIMIT 5;
To scroll up, run this query:

    SELECT title FROM tracks
     WHERE singer='Madonna'
       AND title<:firsttitle
     ORDER BY title DESC
     LIMIT 5;
*/

class SqliteRecord : public std::vector<std::string>
{
public:
    SqliteRecord(size_t n = 1) : std::vector<std::string>(n) {}
    virtual ~SqliteRecord(){ clear();}
    SqliteRecord(const SqliteRecord &) = default;
    //
    void fetch(SqlLiteStatement *s)
    {
        if(s && (s->columnCount() > 0))
        {
            if(size() != s->columnCount())
            {
                resize(s->columnCount()); // set record size
            }
            for(int i = 0; i < s->columnCount(); i++)
            {
                at(i) = std::string(s->data(i));
            }
        }
        else
        {
            clear();
        }
    }

};
//
// a set of records pulled into memory
// use with scrolling
//
class SqliteRecordSet : public std::list<SqliteRecord>
{
public:
    SqliteRecordSet() {}
    virtual ~SqliteRecordSet() {}
    void fetch(SqlLiteStatement *s, bool reverse = false)
    {
        clear();
        while(s->next())
        {
            SqliteRecord r(s->columnCount());
            r.fetch(s);
            if(reverse)
            {
                push_front(r);
            }
            else
            {
                push_back(r);
            }
        }
    }
};



class SqliteScroll
{
    SQLiteDB * _db = nullptr;
    std::unique_ptr<SqlLiteStatement> _forward;
    std::unique_ptr<SqlLiteStatement> _backward;
    //
    std::string _first;
    std::string _last;
    //
    std::string _key;
    //
    SqliteRecordSet _records;
    //

public:
    /*!
     * \brief SqliteScroll
     * \param db
     */
    SqliteScroll(SQLiteDB *db = nullptr) : _db(db)
    {

    }
    /*!
     * \brief ~SqliteScroll
     */
    virtual ~SqliteScroll() { }

    /*!
     * \brief first
     * \return
     */
    const std::string & first() const { return _first;}
    /*!
     * \brief last
     * \return
     */
    const std::string & last() const { return _last;}

    /*!
     * \brief records
     * \return
     */
    SqliteRecordSet & records() { return _records;}

    /*!
     * \brief setCursors
     * \param k
     * \param s
     */
    void setCursors(const std::string &k, const std::string &s)
    {
        // configure cursors
        if(_db)
        {
            _key = k;
            std::string f = s + " AND (" + k + " > ? ) ORDER BY " + k + " LIMIT ?;"; // build the queries
            std::string b = s + " AND (" + k + " < ? )  ORDER BY " + k + " DESC LIMIT ?;";
            _forward = std::make_unique<SqlLiteStatement>(_db,f);
            _backward = std::make_unique<SqlLiteStatement>(_db,b);
        }
    }

    /*!
     * \brief scrollForwards
     * \param key
     * \param count
     * \return result set
     */
    bool  scrollForwards(const std::string &key, int count = 10)
    {
        _forward->clear();
        _forward->bindString(0,key);
        _forward->bindInt(1,count);
        //
        bool ret =  _forward->exec();
        _first = _last = "";
        if(ret)
        {
            _records.fetch(_forward.get());
            if(_records.size() > 0)
            {
                SqliteRecord &f = _records.front();
                SqliteRecord &b = _records.back();
                _first = f[0];
                _last = b[0];
            }
        }
        else
        {
            _records.clear();
        }

        _forward->reset();
        return ret;
    }

    /*!
     * \brief scrollBackwards
     * \param key
     * \param count
     * \return
     */
    bool scrollBackwards(const std::string &key, int count = 10)
    {
        _backward->clear();
        _backward->bindString(0,key);
        _backward->bindInt(1,count);
        bool ret = _backward->exec();
        if(ret)
        {
            _records.fetch(_backward.get(),true);
            if(_records.size() > 0)
            {
                SqliteRecord &f = _records.front();
                SqliteRecord &b = _records.back();
                _first = f[0];
                _last = b[0];
            }
        }
        else
        {
            _records.clear();
            _first = _last = "";
        }
        _backward->reset();
        return ret;
    }
};

}

#endif // SQLITEDB_H
