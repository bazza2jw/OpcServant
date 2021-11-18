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
#include "reportresultdatabase.h"
#include <Common/reporter.h>
#include <sstream>
/*!
 * \brief MRL::ReportResultDatabase::ReportResultDatabase
 * \param dir
 * \param name
 */
MRL::ReportResultDatabase::ReportResultDatabase(const std::string &reportDir, const std::string &reportName)
    : _dir(reportDir), _name(reportName)
{
    _db = std::make_unique<SQLiteDB>(); // construct the database
    initialise();
}
/*!
 * \brief MRL::ReportResultDatabase::initialise
 * \return true on success
 */
bool MRL::ReportResultDatabase::initialise()
{
    std::string d = _dir + "/" + _name;
    std::string n = _name + ".DB";
    if( _db->OpenConnection(n,d,10000))
    {
        // we need a table to index tables and to hold summary information
        std::string cmd = "CREATE TABLE IF NOT EXISTS INDEX_TABLE "
                          "( F_NAME VARCHAR(256) PRIMARY KEY, "
                          "F_ID VARCHAR(32) ,"
                          "F_COUNT INTEGER DEFAULT 0, "
                          "F_MIN DOUBLE DEFAULT 0,"
                          "F_MAX DOUBLE DEFAULT 0, "
                          "F_MEAN DOUBLE DEFAULT 0, "
                          "F_STDDEV DOUBLE DEFAULT 0,"
                          "F_STATS VARCHAR(2000));"; // map table id to item name - item names are not valid sql table names
        //
        _db->Execute(cmd);
        //
        cmd = "REPLACE INTO INDEX_TABLE (F_NAME, F_ID, F_COUNT) VALUES(?,?,0);";
        _addIndex = std::make_unique<SqlLiteStatement>(_db.get(),cmd);
        //
        // update the statistics in the index table
        cmd ="UPDATE INDEX_TABLE "
             "SET F_COUNT=?, F_MIN=?,F_MAX=?,F_MEAN=?,F_STDDEV=?,F_STATS=?"
             " WHERE F_ID=?;";
        _updateStatistics = std::make_unique<SqlLiteStatement>(_db.get(),cmd);
        //
        cmd = "SELECT F_STATS FROM INDEX_TABLE WHERE F_ID=?;";
        _getStatistics = std::make_unique<SqlLiteStatement>(_db.get(),cmd);
        //
        // load the index table - reports may be reloaded
        //
        IResult *r = _db->ExecuteSelect("SELECT F_NAME, F_ID, F_COUNT FROM INDEX_TABLE;");
        if(r)
        {
            while(r->Next())
            {
                std::string n = std::string(r->ColumnData(0));
                std::string i = std::string(r->ColumnData(1));
                int ti = atoi(r->ColumnData(1) + 1); // get the table number
                if(_table_id <= ti) _table_id = ti+1; // track the table id
                int c = atoi(r->ColumnData(2)); // number of records
                _map[n] = i;
                _countMap[n] = c;
            }
        }
        //
    }
    return false;
}

/*!
 * \brief MRL::ReportResultDatabase::getStatistics
 * \param name
 * \param stats
 * \return
 */
bool MRL::ReportResultDatabase::getStatistics(const std::string &name, Statistics &stats)
{
    _getStatistics->reset();
    _getStatistics->bindString(1,_map[name]);
    if(_getStatistics->next())
    {
        Json::Value v;
        std::string s(_getStatistics->data(0));
        if(MRL::stringToJson(s,v))
        {
            stats.clear();
            stats.fromJson(v);
            return true;
        }
    }
    return false;
}

/*!
 * \brief MRL::ReportResultDatabase::updateStatistics
 * \param name
 * \param stats
 * \return
 */
bool MRL::ReportResultDatabase::updateStatistics(const std::string &name, Statistics &stats)
{
    _updateStatistics->reset();
    _updateStatistics->bindInt(1,stats.getNumberSamples());
    _updateStatistics->bindDouble(2,stats.getMinimum());
    _updateStatistics->bindDouble(3,stats.getMaximum());
    _updateStatistics->bindDouble(4,stats.getMean());
    _updateStatistics->bindDouble(5,stats.getStdDev());
    //
    Json::Value v;
    stats.toJson(v);
    std::string s;
    MRL::jsonToString(v,s);
    _updateStatistics->bindString(6,s);
    //
    _updateStatistics->bindString(7,_map[name]);
    _countMap[name] = stats.getNumberSamples();
    return _updateStatistics->exec();
}


/*!
 * \brief MRL::ReportResultDatabase::createTable
 * \param name
 * \return true on success
 */
bool MRL::ReportResultDatabase::createTable(const std::string &name) // create a table
{


    if(map().find(name) == map().end()) // does the table exist or not
    {
        std::string in = "T" + std::to_string(_table_id++);
        _map[name] = in; // cache it
        std::string  cmd = "CREATE TABLE IF NOT EXISTS " + in
                + " (F_TIMESTAMP DATETIME,F_TYPE VARCHAR(32),F_STATE VARCHAR(32),F_VALUE VARCHAR(32),F_DOUBLEVALUE DOUBLE);";
        _db->Execute(cmd);
    }
    else
    {
        // remove data, if any
        std::string c = "DELETE FROM " + _map[name] + ";";
        _db->Execute(c);
    }
    // create the index
    _addIndex->reset();
    _addIndex->bindString(1,name);
    _addIndex->bindString(2,_map[name]);
    if(!_addIndex->exec())
    {
        wxLogDebug("Failed to create index");
        return false;
    }
    //
    _countMap[name] = 0;
    //
    //
    return true;
}
/*!
 * \brief MRL::ReportResultDatabase::writeRecord
 * \param name
 * \return  true on success
 */
bool MRL::ReportResultDatabase::writeRecord(const std::string &name, const ReporterItem &i) // write a single record to a table
{
    std::ostringstream os;
    os << "INSERT INTO " << _map[name] <<  " (F_TIMESTAMP,F_STATE,F_VALUE,F_DOUBLEVALUE) VALUES ('"
       << i._timeStr << "','"<< i._state << "','"<< i._valueStr << "'," << i._valueStr<<");" << std::ends;
    _db->Execute(os.str());
    return true;
}
/*!
 * \brief MRL::ReportResultDatabase::getRecords
 * \param name
 * \param start
 * \param end
 * \param l
 * \return  true on success
 */
bool MRL::ReportResultDatabase::getRecords(const std::string &name, int start, int end, ReportItemList &l) // fetch a block of records from a given table
{
        l.clear();
        return processRecords(name,[&](const ReporterItem &i) { l.push_back(i);},  start,end);
}

/*!
 * \brief MRL::ReportResultDatabase::getPage
 * \param name
 * \param page
 * \param l
 * \param pageLen
 * \return
 */
bool MRL::ReportResultDatabase::getPage(const std::string &name,int page, ReportItemList &l, int pageLen)  // get one page (A4 sized) of values - but could be sets of points
{
    if(page >= 0)
    {
        int start = page * pageLen; // start row
        if(start < _countMap[name])
        {
            return getRecords(name, page * pageLen, ((page +1)*pageLen - 1), l);
        }
    }
    return false;
}


/*!
 * \brief MRL::ReportResultDatabase::processRecords
 * \param name
 * \param func
 * \param start
 * \param end
 * \return
 */
bool MRL::ReportResultDatabase::processRecords(const std::string &name,  std::function<void (const ReporterItem &)> func,int start, int end)
{
    if(end == -1) end = _countMap[name];
    if((start >= 0) && (end > start))
    {
        try
        {
            std::ostringstream  os;
            os <<  "SELECT F_TIMESTAMP,F_STATE,F_VALUE,F_DOUBLEVALUE FROM " << _map[name]  << " WHERE ROWID BETWEEN " <<  start  << " AND " << end << ";" << std::ends;
            IResult  *ir = _db->ExecuteSelect(os.str());
            if(ir)
            {
                while(ir->Next()) // fetch those in range
                {
                    ReporterItem i;
                    i._timeStr = std::string(ir->ColumnData(0));
                    wxDateTime dt;
                    dt.ParseISOCombined(i._timeStr);
                    i._timeStamp = dt;
                    // state
                    i._state = std::string(ir->ColumnData(1));
                    // value as text
                    i._valueStr = std::string(ir->ColumnData(2));
                    // value as double
                    i._value = std::atof(ir->ColumnData(3));
                    func(i); // process the record
                }
            }
        }
        catch(...)
        {
            return false;
        }
        return true;

    }
    return false;
}



