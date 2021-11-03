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
#include "ReportGeneratorPanel.h"
#include <wx/msgdlg.h>
#include <Common/common.h>
#include <wx/dir.h>
#include <Common/reporter.h>
#include <Common/objectmanager.h>
#include <Common/Daq/daqcommon.h>
#include <Common/Daq/localdb.h>
#include <Servant/buiapp.h>
#include <wx/app.h>
#include <Common/reportdoc.h>
#include "ReportPrintOptions.h"


/*!
 * \brief ReportGeneratorPanel::ReportGeneratorPanel
 * \param parent
 */
ReportGeneratorPanel::ReportGeneratorPanel(wxWindow* parent)
    : ReportGeneratorPanelBase(parent), _updateTrigger(this)
{
    setReportList();
    setValueList();
}

/*!
 * \brief ReportGeneratorPanel::setReportList
 */
void ReportGeneratorPanel::setReportList()
{
    std::string rd = MRL::Common::baseDir();
    rd +=  MRL::ReportConfig::reportDir;
    //
    wxArrayString files;
    wxDir::GetAllFiles 	(rd,&files, "*.rpt");
    GetListDataPoints()->Clear();
    if(files.Count() > 0)
    {
        for(int i = 0; i < files.Count(); i++)
        {
            //std::cerr << files[i].ToStdString() << std::endl;
            wxFileName fn(files[i]);
            GetListDataPoints()->Append(fn.GetName()); // build the report name list
        }
    }
    if(GetListDataPoints()->GetCount() > 0) GetListDataPoints()->SetSelection(0);
}


/*!
 * \brief ReportGeneratorPanel::setValueList
 */
void ReportGeneratorPanel::setValueList()
{
    //
    // now populate the value list - really need to use aliases for easy reference - TBD
    //
    GetValueList()->Clear();
    MRL::DataNodeIdMap &m = MRL::Common::configuration().idMap();
    for (auto i = m.begin(); i != m.end(); i++) {
        MRL::PropertyPath p;
        MRL::Common::configuration().getPath(i->first, p);
        std::string pt;
        p.toString(pt);
        //
        MRL::RtObjectRef &r = MRL::Common::daq().objects()[i->first]; // get the RT object
        if(r)
        {
            MRL::StringVector &in =  r->inputs();
            // now build the list
            for(int j = 0; j < in.size(); j++)
            {
                std::string s = r->pathAsString() + TAG_SEPERATOR + in[j];
                GetValueList()->Append(s);
            }
        }
    }
    // Meaningful names for values
    GetAliasList()->Clear();
    MRL::StringMap &a = MRL::Common::aliasMap();
    for(auto i = a.begin(); i != a.end(); i++)
    {
        GetAliasList()->Append(i->first);
    }
}


/*!
 * \brief ReportGeneratorPanel::~ReportGeneratorPanel
 */
ReportGeneratorPanel::~ReportGeneratorPanel()
{
}
/*!
 * \brief ReportGeneratorPanel::OnTimeframeChoice
 */
void ReportGeneratorPanel::OnTimeframeChoice(wxCommandEvent& /*event*/)
{
    switch(GetChoiceTimeFrame()->GetCurrentSelection())
    {
    case MRL::ReportConfig::TF_LAST_HOUR:
    case MRL::ReportConfig::TF_LAST_DAY:
    case MRL::ReportConfig::TF_LAST_WEEK:
        GetTimeStart()->Disable();
        GetDateStart()->Disable();
        GetDateEnd()->Disable();
        GetTimeEnd()->Disable();
        break;
    case MRL::ReportConfig::TF_EXACT:
        GetTimeStart()->Enable();
        GetDateStart()->Enable();
        GetDateEnd()->Enable();
        GetTimeEnd()->Enable();
        break;
    default:
        break;
    }
}

/*!
 * \brief ReportGeneratorPanel::toConfig
 */
void ReportGeneratorPanel::toConfig()
{
    _config.config().clear();
    // populate the fields
    MRL::PropertyPath p;
    p.push_back("Report");
    _config.config().setValue(p,"Timeframe",GetChoiceTimeFrame()->GetCurrentSelection());
    //
    std::string s;
    // get the datetime
    wxDateTime T;
    wxDateTime D;
    //
    switch(GetChoiceTimeFrame()->GetCurrentSelection())
    {
    case MRL::ReportConfig::TF_LAST_HOUR:
    {
        wxTimeSpan ts(-1);
        s = wxDateTime::Now().FormatISOCombined().ToStdString();
        _config.config().setValue(p,"End",s);
        D = wxDateTime::Now().Add(ts);
        s = D.FormatISOCombined().ToStdString();
        _config.config().setValue(p,"Start",s);
    }
    break;
    case MRL::ReportConfig::TF_LAST_DAY:
    {
        wxTimeSpan ts(-24);
        s = wxDateTime::Now().FormatISOCombined().ToStdString();
        _config.config().setValue(p,"End",s);
        D = wxDateTime::Now().Add(ts);
        s = D.FormatISOCombined().ToStdString();
        _config.config().setValue(p,"Start",s);
    }
    break;
    case MRL::ReportConfig::TF_LAST_WEEK:
    {
        wxTimeSpan ts(-24 * 7);
        s = wxDateTime::Now().FormatISOCombined().ToStdString();
        _config.config().setValue(p,"End",s);
        D = wxDateTime::Now().Add(ts);
        s = D.FormatISOCombined().ToStdString();
        _config.config().setValue(p,"Start",s);
    }

    break;
    case MRL::ReportConfig::TF_EXACT:
    {
        //
        T = GetTimeStart()->GetValue();
        D = GetDateStart()->GetValue();
        //
        D.SetHour(T.GetHour());
        D.SetMinute(T.GetMinute());
        D.SetSecond(T.GetSecond());
        //
        s = D.FormatISOCombined().ToStdString();
        _config.config().setValue(p,"Start",s);
        //
        T = GetTimeEnd()->GetValue();
        D = GetDateEnd()->GetValue();
        //
        D.SetHour(T.GetHour());
        D.SetMinute(T.GetMinute());
        D.SetSecond(T.GetSecond());
        //
        s = D.FormatISOCombined().ToStdString();
        _config.config().setValue(p,"End",s);

    }
    break;
    default:
        break;
    }

    //
    //
    p.push_back("Values");
    //
    int k = 0;
    for(int i = 0; i < GetValueList()->GetCount(); i++)
    {
        // store selected value list
        if(GetValueList()->IsChecked(i))
        {
            std::string n = "I" + std::to_string(k++);
            _config.config().setValue(p,n,GetValueList()->GetString(i).ToStdString());
        }
    }
    p.pop_back();
    //
    k=0;
    p.push_back("Alias");
    for(int i = 0; i < GetAliasList()->GetCount(); i++)
    {
        // store selected value list
        if(GetAliasList()->IsChecked(i))
        {
            std::string n = "I" + std::to_string(k++);
            _config.config().setValue(p,n,GetAliasList()->GetString(i).ToStdString());
        }
    }

}

/*!
 * \brief ReportGeneratorPanel::onLoadReport
 */
void ReportGeneratorPanel::onLoadReport(wxCommandEvent& /*event*/)
{

    if(GetListDataPoints()->GetCount() > 0)
    {
        if(GetListDataPoints()->GetSelection() >= 0)
        {
            wxString s = GetListDataPoints()->GetString(GetListDataPoints()->GetSelection());
            if(!_config.load(s.ToStdString()))
            {
                wxMessageBox(_("Report Load Failed"),_("Load Report"));
            }
            else
            {
                setValueList();
                // populate the fields
                MRL::PropertyPath p;
                p.push_back("Report");
                int tfs = _config.config().getValue<int>(p,"Timeframe");
                GetChoiceTimeFrame()->SetSelection(tfs);
                // get the datetime
                wxDateTime start;
                std::string s = _config.config().getAsString(p,"Start");
                start.ParseISOCombined(s);
                GetTimeStart()->SetValue(start);
                GetDateStart()->SetValue(start);
                //
                s = _config.config().getAsString(p,"End");
                wxDateTime end;
                end.ParseISOCombined(s);
                GetTimeEnd()->SetValue(end);
                GetDateEnd()->SetValue(end);
                //
                // Now get the list of points in the report
                // clear value selections
                for(int i = 0; i < GetValueList()->GetCount(); i++)
                {
                    GetValueList()->Check(i,false);
                }
                p.push_back("Values");
                // get the list of child items
                MRL::StringList l;
                _config.config().listChildren(p,l);
                for(auto i  = l.begin(); i != l.end(); i++)
                {
                    std::string v = _config.config().getAsString(p, *i);
                    //
                    // get the value name
                    // set the check
                    int j = GetValueList()->FindString(v);
                    if(j !=  wxNOT_FOUND)
                    {
                        GetValueList()->Check(j,true);
                    }
                }
                //
                p.pop_back();
                //
                for(int i = 0; i < GetAliasList()->GetCount(); i++)
                {
                    GetAliasList()->Check(i,false);
                }
                p.push_back("Alias");
                // get the list of child items
                l.clear();
                _config.config().listChildren(p,l);
                for(auto i  = l.begin(); i != l.end(); i++)
                {
                    std::string v = _config.config().getAsString(p, *i);
                    //
                    // get the value name
                    // set the check
                    int j = GetAliasList()->FindString(v);
                    if(j !=  wxNOT_FOUND)
                    {
                        GetAliasList()->Check(j,true);
                    }
                }

                //
            }
        }
    }
}

/*!
 * \brief ReportGeneratorPanel::addPages
 */
void ReportGeneratorPanel::addPages()
{
    //
    // add the pages to the tables - pages scroll records in and out from the report (which is in the form of an SQLITE database)
    if(_report)
    {
        for(int i = 0; i < _group._items.size(); i++)
        {
            ReportTablePage *p = new ReportTablePage(_report.get(), _group._items[i], GetReportTables());
            p->SetHelpText(_group._items[i]);
            GetReportTables()->AddPage(p,std::to_string(i+1)); // give an index number as the tab name - item names can be very long
            _tablePages.push_back(p);
        }
    }
}

/*!
 * \brief ReportGeneratorPanel::loadPages
 */
void ReportGeneratorPanel::loadPages()
{
    for(auto ip = _tablePages.begin(); ip != _tablePages.end(); ip++)
    {
        (*ip)->loadPage(0);
    }
}

/*!
 * \brief ReportGeneratorPanel::onMakeReport
 * \param event
 */
void ReportGeneratorPanel::onMakeReport(wxCommandEvent& /*event*/)
{
    if(GetListDataPoints()->GetCount() > 0)
    {
        if(GetListDataPoints()->GetSelection() >= 0)
        {
            toConfig(); // load the current configuration - maybe modified from last loaded one
            if(_config.toReportGroup(_group))
            {
                wxString rn = GetListDataPoints()->GetString(GetListDataPoints()->GetSelection());
                std::string rd = MRL::Common::baseDir() + MRL::ReportConfig::reportDir + "/" + rn.ToStdString();
                //
                if(!wxDir::Exists(rd)) wxDir::Make(rd); // create the directory if necessary
                //
                GetReportTables()->DeleteAllPages(); // clear all table pages
                _tablePages.clear();
                _report.reset();
                _report = std::make_unique<MRL::Reporter>(rn.ToStdString(), MRL::LocalDb::LOCAL_DB_DIR, MRL::LOCAL_DB_NAME);
                _report->setOutputDir(rd);
                if(_report->lock()) // it is possible for multiple accesses to the same report - eg desktop and web or multiple web accesses
                {
                    //
                    if(_report->fetch(_group))// generate the tables and the stats to a SQLITE database
                    {
                        // now populate the pages from the generated report
                        // Statistics
                        GetStatsTable()->DeleteAllItems();
                        // build the stats table
                        wxVector<wxVariant> data;
                        for(int i = 0; i < _group._items.size(); i++)
                        {
                            _report->statsTowxVector(_group._items[i], data);
                            GetStatsTable()->AppendItem(data);
                        }
                        // tables
                        addPages();
                        //
                        MRL::Graph g;
                        _report->createGraph(_group, g);
                        wxBitmap bm;
                        g.plotToBitmap(bm,800,600);
                        GetGraphBitmap()->SetBitmap(bm);
                        //
                        wxString gf;
                        gf = wxString::Format("%s/%s_Image.png",rd.c_str(),rn); // save the bitmap for printing
                        bm.SaveFile(gf,wxBITMAP_TYPE_PNG);
                        //
                        GetButtonPrint()->Enable();
                        //
                        GetNotebook()->SetSelection(GetNotebook()->FindPage(m_panelGraph));
                    }
                    _updateTrigger.StartOnce(10); // allow for window creation before populating
                    _report->unlock();
                }
            }
        }
    }
}
/*!
 * \brief ReportGeneratorPanel::onRemoveReport
 * \param event
 */
void ReportGeneratorPanel::onRemoveReport(wxCommandEvent& event)
{
    if(GetListDataPoints()->GetCount() > 0)
    {
        if(GetListDataPoints()->GetSelection() >= 0)
        {
            wxMessageDialog dlg(this,_("Remove Report - Are You Sure?"),_("Remove Report"),wxCANCEL | wxYES_NO);
            if(dlg.ShowModal() == wxID_YES)
            {
                wxString rn = GetListDataPoints()->GetString(GetListDataPoints()->GetSelection());
                _config.deleteReport(rn.ToStdString());
                //
                GetListDataPoints()->Delete(GetListDataPoints()->GetSelection());
                //
                // Delete the report files
                if(GetListDataPoints()->GetCount() > 0)
                {
                    GetListDataPoints()->SetSelection(0);
                    onLoadReport(event);
                }
            }
        }
    }
}
/*!
 * \brief ReportGeneratorPanel::onSaveReport
 * \param event
 */
void ReportGeneratorPanel::onSaveReport(wxCommandEvent &event)
{
    toConfig();
    if(GetListDataPoints()->GetCount() > 0)
    {
        if(GetListDataPoints()->GetSelection() >= 0)
        {
            std::string rn = GetListDataPoints()->GetString(GetListDataPoints()->GetSelection()).ToStdString();
            _config.saveAs(rn);
        }
    }
    else
    {
        OnNew(event);
    }
}
/*!
 * \brief ReportGeneratorPanel::OnNew
 * \param event
 */
void ReportGeneratorPanel::OnNew(wxCommandEvent& /*event*/)
{
    // get report name
    wxString rn =	wxGetTextFromUser (_("Report Name"), _("New Report"), _("Report"), this);
    if(!rn.IsEmpty())
    {
        // Add to the list
        GetListDataPoints()->Append(rn);
        wxDateTime d = wxDateTime::Now();
        //
        GetTimeStart()->SetValue(d);
        GetDateStart()->SetValue(d);
        GetTimeEnd()->SetValue(d);
        GetDateEnd()->SetValue(d);
        GetChoiceTimeFrame()->SetSelection(0);
        toConfig();
        _config.saveAs(rn.ToStdString());
    }
}
/*!
 * \brief ReportGeneratorPanel::onRefreshValues
 * \param event
 */
void ReportGeneratorPanel::onRefreshValues(wxCommandEvent & )
{
    setValueList();
    setReportList();
}
/*!
 * \brief ReportGeneratorPanel::onPrint
 * \param event
 */
void ReportGeneratorPanel::onPrint(wxCommandEvent& /*event*/)
{
    if(_report)
    {
        ReportPrintOptions dlg(this);
        if(dlg.ShowModal() == wxID_OK)
        {
            // get the HTML easy printer
            _report->setPageLayout(3,60);
            _report->setAddPageBreaks(true);
            _report->createReportHtml(_group, dlg.GetPrintTable()->GetValue(), dlg.GetPrintGraph()->GetValue(), dlg.GetPrintStats()->GetValue()); // generate the report as HTML
            wxString rn = GetListDataPoints()->GetString(GetListDataPoints()->GetSelection());
            std::string reportD = MRL::Common::baseDir() + MRL::ReportConfig::reportDir + "/" + rn.ToStdString();
            // Now do the print
            MRL::Common::htmlPrinter()->SetParentWindow(this); // map on to this window as parent
            MRL::Common::htmlPrinter()->SetStandardFonts(4);
            // Print the tables
            std::string tableFile = reportD + "/" + _report->name() + ".html";
            MRL::Common::htmlPrinter()->SetName(_report->name());
            MRL::Common::htmlPrinter()->SetHeader("@TITLE@ - @DATE@ @TIME@");
            MRL::Common::htmlPrinter()->SetFooter(_("Page @PAGENUM@ of @PAGESCNT@"));

            MRL::Common::htmlPrinter()->PrintFile(tableFile);
            //
        }
    }
}
/*!
 * \brief ReportGeneratorPanel::onExport
 * \param event
 */
void ReportGeneratorPanel::onExport(wxCommandEvent& /*event*/)
{
    // export CSV
    if(GetListDataPoints()->GetCount() > 0)
    {
        if(GetListDataPoints()->GetSelection() >= 0)
        {
            toConfig(); // load the current configuration - maybe modified from last loaded one
            if(_config.toReportGroup(_group))
            {
                wxString rn = GetListDataPoints()->GetString(GetListDataPoints()->GetSelection());
                std::string rd = MRL::Common::baseDir() + MRL::ReportConfig::reportDir + "/" + rn.ToStdString();
                //
                if(!wxDir::Exists(rd)) wxDir::Make(rd); // create the directory if necessary
                //
                _report.reset();
                _report = std::make_unique<MRL::Reporter>(rn.ToStdString(), MRL::LocalDb::LOCAL_DB_DIR, MRL::LOCAL_DB_NAME);
                _report->setOutputDir(rd);
                //
                if(_report->fetch(_group))// generate the tables and the stats to a SQLITE database
                {
                    //
                    _report->createCsv(_group);
                    wxString d = wxDirSelector( _("Select Destination Directory"));
                    //
                    if(!d.IsEmpty())
                    {
                        // do the file copy
                        for(int i = 0; i < _group._items.size(); i++)
                        {
                            std::string it = _group._items[i];
                            MRL::Reporter::itemToFilename(it);
                            wxString f = wxString(it) + ".csv";
                            wxString src = wxString(rd) + "/" + f;
                            wxString dest = d + "/" + f;
                            wxCopyFile( src, dest);
                        }
                    }
                }
            }
        }
    }
}
