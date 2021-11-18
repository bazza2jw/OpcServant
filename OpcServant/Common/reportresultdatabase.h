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
#ifndef REPORTRESULTDATABASE_H
#define REPORTRESULTDATABASE_H

#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <MrlLib/sqlitedb.h>
#include <MrlLib/stats.hpp>
#include <functional>
#include <Common/reporteritem.h>

namespace MRL{
/*!
 * \brief The ReportResultDatabase class
 * This database stores the results of a report in a format easier to use when displaying a scrolling through reports
 *
 */


    class ReportResultDatabase
    {
        int _table_id = 1;
        std::map<std::string , std::string> _map; // map table id to item name
        std::map<std::string , int> _countMap;
        std::unique_ptr<SQLiteDB> _db;
        std::unique_ptr<SQLiteDB> _localDb;
        //
        std::string _dir; // directory name
        std::string _name; // report name
        //
        std::unique_ptr<SqlLiteStatement> _addIndex; // the data read cursor
        std::unique_ptr<SqlLiteStatement> _updateStatistics; // update the statistics entry
        std::unique_ptr<SqlLiteStatement> _getStatistics; // get the statistics entry for a given item
        //



    public:
        ReportResultDatabase(const std::string &reportDir, const std::string &reportName);
        bool initialise();
        bool createTable(const std::string &name); // create a table
        bool writeRecord(const std::string &name, const ReporterItem &); // write a single record to a table
        bool getRecords(const std::string &name,int start, int end, ReportItemList &l); // fetch a block of records from a given table
        bool getPage(const std::string &name,int page, ReportItemList &l , int pageLen = 60); // get one page (A4 sized) of values
        bool processRecords(const std::string &name,  std::function<void (const ReporterItem &)> func ,int start = 0, int end = -1);
        bool updateStatistics(const std::string &name, Statistics &stats); // set the statistics for an item
        bool getStatistics(const std::string &name, Statistics &stats);
        int getPageCount(const std::string &name, int pageLen = 60) // number of pages
        {
            int ret =  _countMap[name] / pageLen;
            if(_countMap[name] % pageLen) ret++;
            return ret;
        }
        //
        std::map<std::string , std::string> & map() { return _map;}
        std::map<std::string , int> &countMap() { return _countMap;}
        //
        std::unique_ptr<SQLiteDB> & db() { return _db;}
        std::unique_ptr<SQLiteDB> & localDb() { return _localDb;}
    };
}
#endif // REPORTRESULTDATABASE_H
