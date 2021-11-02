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
#ifndef REPORTER_H
#define REPORTER_H

#include <Common/reporteritem.h>
#include <Common/reportresultdatabase.h>
#include <algorithm>
#include <string>
#include <Common/graph.hpp>
// the graphing window

namespace MRL
{


/*!
 * \brief The Reporter class
 */
class ReportDoc;
class ReportGraph;
class ReportResultDatabase;
/*!
 * \brief The Reporter class
 */
class Reporter
{
    std::string _name;
    std::string _dir;
    std::string _file;
    std::unique_ptr<SqlLiteStatement> _smt;
    std::string _outputDir; // where to put the generated report
    std::unique_ptr<SQLiteDB>  _db; // connection to the local database - SQLITE will serialise access - no need for locking
    std::unique_ptr<ReportResultDatabase> _rdb;
    static const int PAGE_LEN=60;
    int _pageLength = PAGE_LEN;
    int _numberCols = 1;
    int table_id = 1;
    ReportGraph * _graph = nullptr;
    int _currentPage = 0;
    bool _addPageBreaks = false;
    std::string itemNameToFileName(const std::string &item)
    {
        std::string s = item;
        std::replace(s.begin(), s.end(), '/','_');
        return s;
    }
public:
    Reporter(const std::string &name, const std::string &dir, const std::string &file);
    virtual ~Reporter() {}
    bool fetch( ReporterSet &st, ReportSpec &rs); // fetch a set of results into a results database table
    bool fetch(ReportGroup &); // fetch a group of results with the same time frame
    //
    // generate the report as HTML tables
    bool generateHtml(std::ostream &os, std::string item, int page);
    bool createTable( const std::string &item);
    bool createTables(ReportGroup &sg, bool asOneFile = false);
    bool createStatistics(ReportGroup &sg);
    bool createStatistics(const std::string &item);
    //
    bool generateCsv(const std::string &item, std::ostream &os);
    bool createCsv(const std::string &item);
    bool createCsv( ReportGroup &sg);
    bool createStatisticsCsv(const std::string &item);
    bool createStatisticsCsv(ReportGroup &sg);
    bool createReportHtml(ReportGroup &g, bool printTable = true, bool printGraph = false, bool printStats = false);
    //
    bool generateGraphHtml(std::ostream &os, std::string item, int page);
    bool generateStatisticsHtml(std::ostream &os, ReportGroup &sg, int page);
    bool generateTablesHtml(std::ostream &os, ReportGroup &g, int page);


    //
    bool createGraph( const std::string &item, ReportGraph *w); // create graph with one data set
    bool createGraph( ReportGroup &, ReportGraph *w); // plot all data on one graph
    //
    bool createGraph( const std::string &item, Graph &w); // create graph with one data set
    bool createGraph(ReportGroup &rg, Graph &w); // plot all data on one graph

    static void itemToFilename(std::string &i){
        int n = i.find(":");
        if(n != std::string::npos) i[n] = '_';
        n = i.find("/");
        while(n != std::string::npos)
        {
            i[n] = '_';
            n = i.find("/");
        }
    }

    //
    bool toPDF(const std::string &name); // generate as a PDF
    bool pdfExists();
    //
    SQLiteDB &db() {
        return *_db;
    }
    //
    const std::string & outputDir() const {
        return _outputDir;
    }
    void setOutputDir(const std::string &s) {
        _outputDir = s;
    }
    const std::string &name() const {
        return _name;
    }
    ReportResultDatabase & resultsdb() {
        return *_rdb;
    }
    void statsTowxVector(const std::string &item,  wxVector<wxVariant> &data);
    //
    static void getReportList(MRL::StringList &l);
    int pageLength() const {
        return _pageLength;
    }
    int numberCols() const {
        return _numberCols;
    }
    void setPageLayout(int nc, int pl ) {
        _pageLength = pl;
        _numberCols = nc;
    }
    int recordsPerRead() const {
        return  pageLength() * numberCols();
    }
    bool addPageBreaks() const {
        return _addPageBreaks;
    }
    void setAddPageBreaks(bool f) {
        _addPageBreaks = f;
    }
};
}
#endif // REPORTER_H
