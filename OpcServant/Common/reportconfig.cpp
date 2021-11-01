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
#include "reportconfig.h"
#include "reporter.h"
/*!
 * \brief MRL::ReportConfig::load
 * \param n
 * \return true on success
 */
bool MRL::ReportConfig::load(const std::string &n)
{
    std::string fn = MRL::Common::baseDir() + reportDir  + "/" + n + reportExt;
    _name = n;
    return _config.load(fn);
}
/*!
 * \brief MRL::ReportConfig::save
 * \return
 */
bool MRL::ReportConfig::save()
{
    return saveAs(_name);
}
/*!
 * \brief MRL::ReportConfig::saveAs
 * \param n
 * \return true on success
 */
bool MRL::ReportConfig::saveAs(const std::string &n)
{
    std::string fn = MRL::Common::baseDir() + reportDir + "/" + n + reportExt;
    _name = n;
    return _config.save(fn);
}


/*!
 * \brief MRL::ReportConfig::deleteReport
 * \return  true on success
 */
bool MRL::ReportConfig::deleteReport(const std::string &n)
{
    std::string dir = MRL::Common::baseDir() + reportDir + "/" + n; // directory with report results
    std::string fn = dir + reportExt; // report file
    wxRemoveFile(fn);
    return wxRmdir(dir, wxPATH_RMDIR_FULL);

}

/*!
 * \brief ReportConfig::toReportGroup
 * \return true on success
 */
bool MRL::ReportConfig::toReportGroup(ReportGroup &g)
{
    g.clear(); // delete all old results
    // populate the fields of a group from a configuration ready for a search
    MRL::PropertyPath p;
    p.push_back("Report");
    int tfs = _config.getValue<int>(p,"Timeframe");
    // get the datetime
    wxDateTime start;
    std::string s = _config.getAsString(p,"Start");
    start.ParseISOCombined(s);
    //
    s = _config.getAsString(p,"End");
    wxDateTime end;
    end.ParseISOCombined(s);
    //
    g._start = start.GetTicks();
    g._end = end.GetTicks();
    if(g._end > g._start)
    {
        //
        p.push_back("Values");
        // get the list of child items
        MRL::StringList l;
        _config.listChildren(p, l);
        g._items.clear();
        //
        size_t j = 0;
        for(auto i  = l.begin(); i != l.end(); i++)
        {
            std::string v = _config.getAsString(p, *i);
            g._items.push_back(v);
            //
        }
        //
        p.pop_back();
        p.push_back("Alias");
        // get the list of child items
        l.clear();
        _config.listChildren(p, l);
        //
        for(auto i  = l.begin(); i != l.end(); i++)
        {
            std::string v = _config.getAsString(p, *i);
            g._items.push_back("@" +  v); // add the aliases
        }
        return true;
    }
    return false;
}


