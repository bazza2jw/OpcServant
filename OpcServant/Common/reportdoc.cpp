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
#include "reportdoc.h"
#include "reporter.h"
/*!
 * \brief MRL::ReportDoc::addTablePage
 * \param rs
 * \param it
 */
void MRL::ReportDoc::addTablePage(ReportResultDatabase &rdb, const std::string &item)
{
    // Build table
    wxReportTableItem table(item);
    table.SetCellsStyle(_style);
    table.SetPosition(wxRP_CENTER, 0);
    //
    //
    // Build the columns into string arrays
    //
    wxArrayString timeS;
    wxArrayString stateS;
    wxArrayString valueS;
    //
    int nI = 0;
    timeS.SetCount(rdb.countMap()[item]);
    stateS.SetCount(rdb.countMap()[item]);
    valueS.SetCount(rdb.countMap()[item]);
    //
    // What should happen is the page breaks should be added as required
    rdb.processRecords(item, [&](const ReporterItem &a)
    {
        timeS[nI] = a._timeStr;
        stateS[nI] = a._state;
        valueS[nI] = a._valueStr;
        nI++;
    });

    // now Add the columns
    table.AddColumn(timeS,"Times");
    table.AddColumn(stateS,"State");
    table.AddColumn(valueS,"Value");
    //
    table.SetColumnsHeadersStyle(_colStyle);
    //
    AddItem(table);
}

/*!
 * \brief MRL::ReportDoc::addGraphPage
 * \param path
 */
void MRL::ReportDoc::addGraphPage(wxString path)
{
    wxReportImageItem i("Graph", path , 10, 10);
    AddPage();
    AddItem(i);
}


/*!
 * \brief MRL::ReportDoc::addGraphPage
 * \param rs
 */
void MRL::ReportDoc::addGraphPage(ReportResultDatabase &rdb, const std::string &item)
{
    // plot one
}

/*!
 * \brief MRL::ReportDoc::addGraphPage
 * \param rg
 */
void MRL::ReportDoc::addGraphPage(ReportResultDatabase &rdb,ReportGroup &rg)
{

}

/*!
 * \brief MRL::ReportDoc::addStatisticsPage
 * \param rs
 */
void MRL::ReportDoc::addStatisticsPage(ReportResultDatabase &rdb, const std::string &item)
{
    // build the statistics
    wxReportTableItem table(item);

    wxArrayString itemS;
    wxArrayString meanS;
    wxArrayString stdDevS;
    wxArrayString maxS;
    wxArrayString minS;
    wxArrayString numS;
    //
    itemS.SetCount(1);
    meanS.SetCount(1);
    stdDevS.SetCount(1);
    maxS.SetCount(1);
    minS.SetCount(1);
    numS.SetCount(1);
    //
    Statistics stats;
    rdb.getStatistics(item,stats);
    itemS[1] = item;
    meanS[1] = wxString::Format("%8.2f", stats.getMean());
    stdDevS[1] = wxString::Format("%8.2f", stats.getStdDev());
    maxS[1] = wxString::Format("%8.2f", stats.getMaximum());
    minS[1] = wxString::Format("%8.2f", stats.getMinimum());
    numS[1] = wxString::Format("%d", stats.getNumberSamples());
   //
    table.AddColumn(itemS,"Item");
    table.AddColumn(meanS,"Mean");
    table.AddColumn(stdDevS,"Std.Dev");
    table.AddColumn(maxS,"Maximum");
    table.AddColumn(minS,"Minimum");
    table.AddColumn(numS,"Number");
    AddItem(table);
}
/*!
 * \brief MRL::ReportDoc::addStatisticsPage
 * \param rg
 */
void MRL::ReportDoc::addStatisticsPage(ReportResultDatabase &rdb,ReportGroup &rg)
{

    wxArrayString itemS;
    wxArrayString meanS;
    wxArrayString stdDevS;
    wxArrayString maxS;
    wxArrayString minS;
    wxArrayString numS;
    //
    itemS.SetCount(rg._items.size());
    meanS.SetCount(rg._items.size());
    stdDevS.SetCount(rg._items.size());
    maxS.SetCount(rg._items.size());
    minS.SetCount(rg._items.size());
    numS.SetCount(rg._items.size());
    //
    for(int i = 0; i < rg._items.size(); i++)
    {
    Statistics stats;
    rdb.getStatistics(rg._items[i],stats);
    itemS[i] = rg._items[i];
    meanS[i] = wxString::Format("%8.2f", stats.getMean());
    stdDevS[i] = wxString::Format("%8.2f", stats.getStdDev());
    maxS[i] = wxString::Format("%8.2f", stats.getMaximum());
    minS[i] = wxString::Format("%8.2f", stats.getMinimum());
    numS[i] = wxString::Format("%d", stats.getNumberSamples());
    }
   //
    wxReportTableItem table("Statistics");
    table.AddColumn(itemS,"Item");
    table.AddColumn(meanS,"Mean");
    table.AddColumn(stdDevS,"Std.Dev");
    table.AddColumn(maxS,"Maximum");
    table.AddColumn(minS,"Minimum");
    table.AddColumn(numS,"Number");
    AddItem(table);
}
