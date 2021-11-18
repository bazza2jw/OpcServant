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
#ifndef REPORTDOC_H
#define REPORTDOC_H
#include <list>
#include <math.h>
#include <stdlib.h>
#include <MrlLib/mrllib.h>
#include <MrlLib/stats.hpp>
#include <reportdocument/report/reportdocument.h>
#include <MrlLib/variantpropertytree.h>


namespace MRL{

struct ReporterItem;
struct ReportSpec;
struct ReportGroup;
struct ReporterSet;
class ReportResultDatabase;
/*!
 * \brief The ReportDoc class
 */
class ReportDoc : public wxReportDocument
{
    static const size_t pageLen = 60; // page length
    wxReportPageStyle _pageStyle; // construct the page style
    wxReportTextStyle _style; // text style
    wxReportTextStyle _colStyle; // text style
    //
    public:
    ReportDoc()
    {
        _pageStyle.SetMargins(10, 10, 65, 20);
        SetPageStyle(_pageStyle);
        _style.SetBorder(wxRP_ALLBORDER, *wxBLACK, 0.3);
        _style.SetFont(wxFont (12,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxT("Times New Roman")));
        // instead of '%d' will be inserted the number of the page
        InsertPageNumbering(wxT("-%d-"), _style, wxRP_CENTER, 285);
        //
        _colStyle.SetBorder(wxRP_ALLBORDER, *wxBLACK, 0.3);
        _colStyle.SetBackgroundColor(wxColour(200, 200, 200));
        _colStyle.SetFont(wxFont (14,wxFONTFAMILY_ROMAN,wxFONTSTYLE_ITALIC,wxFONTWEIGHT_BOLD,false,wxT("Times New Roman")));
       //
    }
    //
    //
    wxReportPageStyle &pageStyle() { return  _pageStyle;} // construct the page style
    wxReportTextStyle &style() { return _style;} // text style
    //
    // add a page using the report results set iterator
    void addTablePage(ReportResultDatabase &rdb, const std::string &item);
    //
    void addStatisticsPage(ReportResultDatabase &rdb, const std::string &item);
    //
    void addStatisticsPage(ReportResultDatabase &rdb,ReportGroup &rg);
    //
    // build a graph page
    void addGraphPage(wxString path);
    /*!
     * \brief addGraphPage
     * \param rs
     */
    void addGraphPage(ReportResultDatabase &rdb, const std::string &item);
    /*!
     * \brief addGraphPage
     * \param rg
     */
    void addGraphPage(ReportResultDatabase &rdb,ReportGroup &rg);

    //
};
}

#endif // REPORTDOC_H
