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
#ifndef REPORTERITEM_H
#define REPORTERITEM_H

#include <list>
#include <math.h>
#include <stdlib.h>
#include <MrlLib/mrllib.h>
#include <MrlLib/sqlitedb.h>
#include <MrlLib/stats.hpp>
#include <reportdocument/report/reportdocument.h>
#include <Common/mathplot.h>
#include <MrlLib/variantpropertytree.h>
namespace MRL{
/*!
     * \brief The ReporterItem struct
     */
    struct ReporterItem
    {
        std::string _timeStr;
        wxDateTime _timeStamp;
        std::string _type;
        std::string _state;
        double _value;
        std::string _valueStr;
        // format for list control
        void towxVector(wxVector<wxVariant> &data)
        {
            data.clear();
            data.push_back(wxVariant(_timeStr.c_str())); // time
            data.push_back(wxVariant(_state.c_str())); // state
            data.push_back(wxVariant(wxString::Format("%8.2f",_value))); // formated value
        }
    };

    typedef std::list<ReporterItem> ReportItemList;
    typedef ReportItemList::iterator ReportItemListIter;



    /*!
     * \brief The ReportSpec struct
     */
    struct ReportSpec
    {
        std::string _item; // the value path to extract values for
        time_t _start = 0;
        time_t _end = 0;
        ReportSpec(){}
        ReportSpec(const ReportSpec &) = default;
        ReportSpec(const std::string &i, time_t s, time_t e) : _item(i),_start(s),_end(e) {}
    };


    /*!
     * \brief The ReporterSet class
     */
    struct ReporterSet : public ReportItemList
    {
        std::string _title;
        ReportSpec _spec;
        Statistics _statistics; // the stats for the report
        ReporterSet() {}
        ReporterSet(const ReporterSet &) = default;
        Statistics & statistics() { return _statistics;}
        ReportSpec & spec() { return _spec;}
        std::string & title() { return _title;}
    };


    /*!
    *!     \brief The ReportGroup struct
    *!
    */
    struct ReportGroup
    {
        //
        StringVector  _items; // list of items
        //
        time_t _start = 0; // time frame
        time_t _end = 0;
        std::vector<ReporterSet> _results;
        void clear()
        {
            _items.clear();
            _results.clear();
            _start = 0;
            _end = 0;
        }
    };

}
#endif // REPORTERITEM_H
