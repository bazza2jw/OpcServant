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
#include "reporter.h"
#include <stdexcept>
#include <wx/datetime.h>
#include <fstream>
#include <Common/mathplot.h>
#include "reportdoc.h"
#include "reportgraph.h"
#include "reportconfig.h"
#include <wx/dir.h>
#include <sstream>
#include <Common/graph.hpp>
#include <Common/Daq/daq.h>


int MRL::Reporter::_reportNo = 0;
/*!
 * \brief MRL::Reporter::Reporter
 */
MRL::Reporter::Reporter(const std::string &name, const std::string &dir, const std::string &file) : _name(name), _dir(dir), _file(file)
{
    std::string d = MRL::Common::baseDir()  + ReportConfig::reportDir;
    _rdb = std::make_unique<ReportResultDatabase>(d,name); // where the results go
}


/*!
 * \brief MRL::Reporter::getReportList
 * \param l
 */
void MRL::Reporter::getReportList(MRL::StringList &l)
{
    l.clear();
    std::string rd = MRL::Common::baseDir();
    rd +=  MRL::ReportConfig::reportDir;
    //
    wxArrayString files;
    wxDir::GetAllFiles 	(rd,&files, "*.rpt");
    if(files.Count() > 0)
    {
        for(int i = 0; i < int(files.Count()); i++)
        {
            wxFileName fn(files[i]);
            l.push_back(fn.GetName().ToStdString());
        }
    }
}



/*!
 * \brief MRL::Reporter::statsTowxVector
 * \param item
 * \param data
 */
void MRL::Reporter::statsTowxVector(const std::string &item,  wxVector<wxVariant> &data)
{
    // prepare a row for adding to a list control from a statistics structure - TDB add SPC values and action / alert counts as options
    Statistics stats;
    resultsdb().getStatistics(item,stats);
    data.clear();
    data.push_back(wxVariant(item.c_str()));
    data.push_back(wxVariant(wxString::Format("%8.2f",stats.getMean())));
    data.push_back(wxVariant(wxString::Format("%8.2f",stats.getMinimum())));
    data.push_back(wxVariant(wxString::Format("%8.2f",stats.getMaximum())));
    data.push_back(wxVariant(wxString::Format("%8.2f",stats.getStdDev())));
    data.push_back(wxVariant(wxString::Format("%d",stats.getNumberSamples())));
}


/*!
 * \brief MRL::Reporter::fetch
 * \param limit
 * \return true on success
 */
bool MRL::Reporter::fetch( ReporterSet &st, ReportSpec &rs) // fetch a set of results
{
    try
    {
        std::unique_ptr<MRL::LocalDb> &db = MRL::Daq::instance()->localDb(); // get the database
        WriteLock l(db->mutex()); // Watch out for SQLITE database locking problems
        if(db->db()->OpenConnection(_file,_dir,10000))
        {
            _smt = std::make_unique<SqlLiteStatement>(db->db().get());
            if(!_smt->prepare("SELECT F_TIMESTAMP,F_TYPE,F_STATE,F_VALUE,F_DOUBLEVALUE FROM DATA_TABLE WHERE F_SOURCE = ? AND F_TIMESTAMP BETWEEN ? AND ?;"))
            {
                throw std::runtime_error( " Prepare Failed");
            }

            //
            _rdb->createTable(rs._item);
            // create the table for the item or clear an existing one
            //
            _smt->reset(); // fetch data from local database
            if(rs._item[0] == '@')
            {
                std::string k = rs._item.substr(1);
                std::string a = MRL::Common::aliasMap()[k];
                if(a.empty()) return false; // no alias
                _smt->bindString(1,a);
            }
            else
            {
                _smt->bindString(1,rs._item);
            }
            _smt->bindDateTime(2,rs._start);
            _smt->bindDateTime(3,rs._end);
            //
            //
            st.spec() = rs;
            st.clear(); // reset the list
            //
            _rdb->db()->BeginTransaction();
            //
            while(_smt->next())
            {
                // get the values
                // timestamp
                ReporterItem i;
                i._timeStr = std::string(_smt->data(0));
                wxDateTime dt;
                dt.ParseISOCombined(i._timeStr);
                i._timeStamp = dt;
                // type
                i._type = std::string(_smt->data(1));
                // state
                i._state = std::string(_smt->data(2));
                // value as text
                i._valueStr = std::string(_smt->data(3));
                // value as double
                i._value = std::atof(_smt->data(4));
                st.statistics().setValue(i._value);
                _rdb->writeRecord(rs._item,i);
            }
            //
            if(!_rdb->updateStatistics(rs._item,st.statistics()))
            {
                std::cerr << "Failed to update statistics " << time(nullptr) << std::endl;
            }
            //
            _rdb->db()->CommitTransaction();
            db->db()->CloseConnection();
        }
    }
    catch(...)
    {
        return false;
    }
    return true;
}
/*!
 * \brief MRL::Reporter::fetch
 * \param limit
 * \return true on success
 */
bool MRL::Reporter::fetch( ReportGroup &rg) // fetch a group of results with the same time frame
{
    rg._results.resize(rg._items.size());
    for(size_t i = 0; i <  rg._items.size(); i++ )
    {
        ReportSpec s(rg._items[i], rg._start, rg._end);
        if(!fetch(rg._results[i],s)) return false;
    }
    return true;
}

/*!
 * \brief MRL::Reporter::generateHtml
 * \param os
 * \param item
 * \param page
 * \return true on success
 */
bool MRL::Reporter::generateHtml( std::ostream &os, std::string item, int page)
{
    const char * tableHeader = "<th>Timestamp</th><th>State</th><th>Value</th>";
    try
    {
        if(os)
        {
            //
            ReportItemList l;
            _rdb->getPage(item, page, l, recordsPerRead());
            //
            if(l.size() > 0)
            {
                os << "<HTML>" << std::endl;
                if(page && addPageBreaks()) os << "<div style=\"page-break-before:always\"></div>" << std::endl;
                os << "<p><h3>" << item << "</h3></p>" << std::endl;
                os << "<table border=\"1\" style=\"width:100%\">" << std::endl;
                os << "<tr>";
                for(int i = 0; i < numberCols(); i++) os << tableHeader;
                os << "</tr>" << std::endl;

                int iRec = 0;
                _rdb->processRecords(item,
                [&](const ReporterItem &a) {
                    if(iRec % _numberCols == 0)
                    {
                        if(iRec > 0) os << "</tr>" ;
                        os << std::endl << "<tr>";
                    }
                    os << "<td>" << a._timeStr << "</td><td>" << a._state <<  "</td><td>" << a._valueStr <<  "</td>";
                    iRec++;

                },
                page *recordsPerRead(), (page +1)*recordsPerRead() -1 );
                //
                // close off table and HTML
                os << "</tr>" << std::endl << "</table>" << std::endl;
                os << "</HTML>" << std::endl;
            }
            return true;
        }
    }
    catch(std::exception &e)
    {
        std::cerr << "Exception " << __FUNCTION__ << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown Exception " << __FUNCTION__  << std::endl;
    }
    return false;
}

/*!
 * \brief MRL::Reporter::generateGraphHtml
 * \return
 */
bool MRL::Reporter::generateGraphHtml(std::ostream &os, std::string item, int page)
{
    std::string g = _outputDir + "/" + item + "_Image.png" ;
    if(os)
    {
        os << "<HTML>" << std::endl;
        if(page && addPageBreaks()) os << "<div style=\"page-break-before:always\"></div>" << std::endl;
        os << "<p><h3>" << item << "</h3></p>" << std::endl;
        os << "<img src=\"" << g << "\" width=80% height=60% alt=\"\">" << std::endl;
        os << "</HTML>" << std::endl;
        return true;
    }
    return false;
}

/*!
 * \brief MRL::Reporter::generateGraphHtml
 * \param os
 * \param item
 * \param page
 * \return
 */
bool MRL::Reporter::generateTablesHtml(std::ostream &os, ReportGroup &g, int page)
{
    for(int i = 0; i < int(g._items.size()); i++)
    {
        int nPage = _rdb->getPageCount(g._items[i], recordsPerRead());
        if(nPage > 0)
        {
            if((i != 0) && addPageBreaks()) os << "<div style=\"page-break-before:always\"></div>" << std::endl;

            for(int j = 0; j < nPage; j++)
            {
                generateHtml(os,g._items[i],j);
            }
        }
    }
    return true;
}


/*!
 * \brief MRL::Reporter::createReportHtml
 * \param g
 * \return
 */
bool MRL::Reporter::createReportHtml(ReportGroup &g, bool printTable, bool printGraph, bool printStats)
{
    std::string f = _outputDir + "/" + _name + ".html" ;
    std::ofstream os(f,std::ios_base::out | std::ios_base::trunc);
    if(os)
    {
        _currentPage = 0;
        if(printTable)
        {
            generateTablesHtml(os, g, _currentPage++);
        }
        if(printGraph || printStats)
        {

            if(printGraph) generateGraphHtml(os, _name, _currentPage++);
            if(printGraph) _addPageBreaks = false;
            if(printStats) generateStatisticsHtml(os,g,_currentPage);
        }
    }
    return true;
}

// generate the report as HTML tables
/*!
 * \brief MRL::Reporter::createTable
 * \return true on success
 */
bool MRL::Reporter::createTable( const std::string &item)
{
    // generate an HTML table for a single value - item names are not file path friendly
    _currentPage = 0;
    std::string f = _outputDir + "/" + itemNameToFileName(item)   + ".html" ;
    std::ofstream os(f,std::ios_base::out | std::ios_base::trunc);
    int nPage = _rdb->getPageCount(item);
    if(nPage > 0)
    {
        for(int i = 0; i < nPage; i++)
        {
            generateHtml(os,item,i);
        }
    }
    return true;
}
/*!
 * \brief MRL::Reporter::createTables
 * \return true on success
 */
bool MRL::Reporter::createTables(ReportGroup &g, bool asOneFile)
{
    _currentPage = 0;
    if(asOneFile)
    {
        // generate as one file
        std::string f = _outputDir + "/" + _name + ".html" ;
        std::ofstream os(f,std::ios_base::out | std::ios_base::trunc);
        if(os)
        {
            return generateTablesHtml(os,g,0);
        }
    }
    else
    {
        // generate as separate files
        for(int i = 0; i < int(g._items.size()); i++)
        {
            if(!createTable( g._items[i])) return false;
        }
    }
    return true;
}



/*!
 * \brief MRL::Reporter::generateStatisticsHtml
 * \param os
 * \param sg
 * \param page
 * \return
 */
bool MRL::Reporter::generateStatisticsHtml(std::ostream &os, ReportGroup &sg, int page)
{
    if(os)
    {
        os << "<HTML>" << std::endl;
        if(page && addPageBreaks()) os << "<div style=\"page-break-before:always\"></div>" << std::endl;
        os << "<header><br><h4>Statistics:" << _name << "</h4></header>" << std::endl;
        os << "<table border=\"1\" style=\"width:100%\">" << std::endl;
        os << "<tr><th>Item</th><th>Mean</th><th>Std.Dev.</th><th>Maximum</th><th>Minimum</th><th>Count</th></tr>" << std::endl;
        for(int i = 0; i < int(sg._items.size()); i++)
        {
            Statistics stats;
            resultsdb().getStatistics(sg._items[i],stats);
            os << "<tr><td>" << sg._items[i] << "</td><td>"  << stats.getMean() << "</td><td>"
               << stats.getStdDev() << "</td><td>" << stats.getMaximum() << "</td><td>"
               << stats.getMinimum() << "</td><td>" << stats.getNumberSamples() << "</td></tr>" << std::endl;
        }
        os << "</table>" << std::endl;
        os << "</HTML>" << std::endl;
        return true;
    }
    return false;
}


/*!
 * \brief MRL::Reporter::createStatistics
 * \param sg
 * \return
 */
bool MRL::Reporter::createStatistics(ReportGroup &sg)
{

    _currentPage = 0;
    std::string f = _outputDir + "/" + _name + "_Statistics.html" ;
    std::ofstream os(f,std::ios_base::out | std::ios_base::trunc);
    if(os)
    {
        return generateStatisticsHtml(os,sg, 0);
    }
    return false;
}

/*!
 * \brief MRL::Reporter::createStatistics
 * \param item
 * \return
 */
bool MRL::Reporter::createStatistics(const std::string &item)
{
    _currentPage = 0;
    std::string f = _outputDir + "/" + itemNameToFileName(item) + "_Statistics.html" ;
    std::ofstream os(f,std::ios_base::out | std::ios_base::trunc);
    if(os)
    {
        os << "<HTML>" << std::endl;
        os << "<header><h4>Statistics:" << item << "</h4></header>" << std::endl;
        os << "<table border=\"1\" style=\"width:100%\">" << std::endl;
        os << "<tr><th>Item</th><th>Mean</th><th>Std.Dev.</th><th>Maximum</th><th>Minimum</th><th>Count</th></tr>" << std::endl;
        //
        Statistics stats;
        resultsdb().getStatistics(item,stats);
        os << "<tr><td>" << item << "</td><td>"  << stats.getMean() << "</td><td>"
           << stats.getStdDev() << "</td><td>" << stats.getMaximum() << "</td><td>"
           << stats.getMinimum() << "</td><td>" << stats.getNumberSamples() << "</td></tr>" << std::endl;

        //
        os << "</table>" << std::endl;
        os << "</HTML>" << std::endl;
        return true;
    }
    return false;
}



/*!
 * \brief MRL::Reporter::generateCsv
 * \param file
 * \param st
 * \return
 */
bool MRL::Reporter::generateCsv(const std::string &item, std::ostream &os)
{
    if(os)
    {
        _rdb->processRecords(item, [&](const ReporterItem &a) {
            os << a._timeStr << "," << a._type << "," << a._state << "," << a._valueStr << std::endl;
        });
    }
    return true;
}

/*!
 * \brief MRL::Reporter::createCsv
 * \param s
 * \return
 */
bool MRL::Reporter::createCsv(const std::string &item)
{
    std::string i = item;
    itemToFilename(i);
    std::string f = _outputDir + "/" + i + ".csv" ;
    // replace colons with underscores
    std::ofstream os(f,std::ios_base::out | std::ios_base::trunc);
    return generateCsv(item,os);
}
/*!
 * \brief MRL::Reporter::createCsv
 * \return
 */
bool MRL::Reporter::createCsv(ReportGroup &g)
{
    for(int i = 0; i < int(g._items.size()); i++)
    {
        if(!createCsv(g._items[i])) return false;
    }
    return true;
}
/*!
 * \brief MRL::Reporter::createStatisticsCsv
 * \param item
 * \return
 */
bool MRL::Reporter::createStatisticsCsv(const std::string &item)
{
    std::string f = _outputDir + "/" + item + "Stats.csv" ;
    std::ofstream os(f,std::ios_base::out | std::ios_base::trunc);

    Statistics stats;
    resultsdb().getStatistics(item,stats);
    os  << item << ","  << stats.getMean() << "," << stats.getStdDev() << "," << stats.getMaximum() << ","
        << stats.getMinimum() << "," << stats.getNumberSamples()  << std::endl;
    return false;
}

/*!
 * \brief MRL::Reporter::createStatisticsCsv
 * \param sg
 * \return
 */
bool MRL::Reporter::createStatisticsCsv(ReportGroup &sg)
{
    std::string f = _outputDir + "/" +  "Stats.csv" ;
    std::ofstream os(f);
    for(int i = 0; i < int(sg._items.size()); i++)
    {
        Statistics stats;
        resultsdb().getStatistics(sg._items[i],stats);
        os  << sg._items[i] << ","  << stats.getMean() << "," << stats.getStdDev() << "," << stats.getMaximum() << ","
            << stats.getMinimum() << "," << stats.getNumberSamples()  << std::endl;
    }
    return false;
}


//
/*!
 * \brief MRL::Reporter::createGraph
 * \return true on success
 */

static const wxColour * colourList[] =
{
    wxBLACK,
    wxBLUE,
    wxCYAN,
    wxGREEN,
    wxYELLOW,
    wxRED
};
#define N_COLOUR_LIST (6)

bool MRL::Reporter::createGraph(const std::string &item, ReportGraph *w ) // create graph with one data set
{
    //
    // Construct chart the set has already been built
    // Build the graph
    static int colourIndex = 0;
    //
    if(_rdb->countMap()[item] > 0)
    {
        std::vector<double> t(_rdb->countMap()[item]);
        std::vector<double> v(_rdb->countMap()[item]);
        //
        int j = 0;
        _rdb->processRecords(item, [&](const ReporterItem &m)
        {
            t[j] = double(m._timeStamp.GetTicks());
            v[j] = m._value;
            j++;
        }
                            );
        //
        wxString s(item);
        w->addlayer(s,*colourList[colourIndex], t, v);
        colourIndex = (colourIndex + 1) % N_COLOUR_LIST; // TBD : map item name to colour
        return true;
    }
    return false;
}
/*!
 * \brief MRL::Reporter::createGraph
 * \return true on success
 */
bool MRL::Reporter::createGraph(ReportGroup &rg, ReportGraph *w) // plot all data on one graph
{
    // the group has already been built
    for(int i = 0; i < int(rg._items.size()); i++)
    {
        createGraph(rg._items[i], w );
    }
    return false;
}

/*!
 * \brief MRL::Reporter::createGraph
 * \param item
 * \param w
 * \return
 */
bool MRL::Reporter::createGraph( const std::string &item, Graph &w) // create graph with one data set
{
    if(_rdb->countMap()[item] > 0)
    {
        std::vector<double> t(_rdb->countMap()[item]);
        std::vector<double> v(_rdb->countMap()[item]);
        //
        int j = 0;
        _rdb->processRecords(item, [&](const ReporterItem &m)
        {
            t[j] = double(m._timeStamp.GetTicks());
            v[j] = m._value;
            j++;
        } );
        //
        w.setData(item,t,v);
        return true;
    }
    return false;

}
/*!
 * \brief MRL::Reporter::createGraph
 * \param w
 * \return
 */
bool MRL::Reporter::createGraph( ReportGroup &rg, Graph &w) // plot all data on one graph
{
    // the group has already been built
    for(int i = 0; i < int(rg._items.size()); i++)
    {
        createGraph(rg._items[i], w );
    }
    return false;
}


//
/*!
 * \brief MRL::Reporter::toPDF
 * \param name
 * \return true on success
 */
bool MRL::Reporter::toPDF(const std::string &n) // generate as a PDF
{
    // use htmldoc to do the conversion
    // this may have to be a configuration parameter if the application is ever ported to Microsoft Windows - for now we assume it is installed in /usr/bin
    std::string f = _outputDir + "/" + _name + ".html" ;
    std::stringstream os;
    os <<  "/usr/bin/htmldoc --webpage --footer \":..\" --fontsize 6  -f " <<  _outputDir <<  "/" <<  n  <<  ".pdf " << f << std::ends;
    std::system(os.str().c_str());
    return true;
}

bool MRL::Reporter::pdfExists()
{
    std::string f = _outputDir + "/" + _name + ".pdf";
    return wxFileExists(f);
}


