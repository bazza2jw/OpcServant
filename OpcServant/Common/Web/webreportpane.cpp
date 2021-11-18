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
#include "webreportpane.h"
#include <Wt/WGridLayout.h>
#include <Common/Web/webdialog.h>
#include <Wt/WMessageBox.h>
#include <Common/reportgraph.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WFitLayout.h>
#include <wx/dir.h>
#include <Common/reportresultdatabase.h>
#include <Common/Daq/daq.h>
#include <Common/Daq/daqcommon.h>
#include <wx/utils.h>
/*!
 * \brief MRL::WebReportPane::WebReportPane
 */
MRL::WebReportPane::WebReportPane()
{
    auto boxLayout = std::make_unique<Wt::WVBoxLayout>();
    _toolBar = boxLayout->addWidget(std::make_unique<Wt::WToolBar>()); // add the tool bar
    auto layout = boxLayout->addLayout(std::make_unique<Wt::WFitLayout>(),2);

    //
    // add the buttons to the tool bar
    //
    _newReport = createColorButton("btn-primary", _TR("New"));
    _loadReport = createColorButton("btn-primary",_TR("Load"));
    _saveReport = createColorButton("btn-primary", _TR("Save"));
    _removeReport = createColorButton("btn-primary", _TR("Remove"));
    _makeReport = createColorButton("btn-primary", _TR("Make")); // this makes the report and generates the PDF that is then displayed - the browser can print this locally
    _viewReport = createColorButton("btn-primary", _TR("View"));
    //
    // Add the windows
    auto grid = std::make_unique<Wt::WGridLayout>();
    grid->setColumnStretch(1,1);
    // Setup panel
    grid->addWidget(std::make_unique<Wt::WText>(_TR("Report")), 0, 0);
    _reportList = grid->addWidget(std::make_unique<Wt::WComboBox>(),0,1);
    grid->addWidget(std::make_unique<Wt::WText>(_TR("Report Type")), 1, 0);
//    //
    _reportType = grid->addWidget(std::make_unique<Wt::WComboBox>(),1,1);
    _reportType->addItem(_TR("Last Hour"));
    _reportType->addItem(_TR("Last Day"));
    _reportType->addItem(_TR("Last Week"));
    _reportType->addItem(_TR("Exact"));
    //
    grid->addWidget(std::make_unique<Wt::WText>(_TR("Start Date")), 2, 0);
    _startDate = grid->addWidget(std::make_unique<Wt::WDateEdit>(),2,1);
    grid->addWidget(std::make_unique<Wt::WText>(_TR("Start Time")), 3, 0);
    _startTime = grid->addWidget(std::make_unique<Wt::WTimeEdit>(),3,1);
    _startTime->setTime(Wt::WTime::currentTime());
    grid->addWidget(std::make_unique<Wt::WText>(_TR("End Date")), 4, 0);
    _endDate = grid->addWidget(std::make_unique<Wt::WDateEdit>(),4,1);
    grid->addWidget(std::make_unique<Wt::WText>(_TR("End Time")), 5, 0);
    _endTime = grid->addWidget(std::make_unique<Wt::WTimeEdit>(),5,1);
    _endTime->setTime(Wt::WTime::currentTime());
//    // Values list
    grid->addWidget(std::make_unique<Wt::WText>(_TR("Values")), 6, 0);
    _valueList = grid->addWidget(std::make_unique<Wt::WSelectionBox>(),6,1) ; // list of available values
    _valueList->setSelectionMode(Wt::SelectionMode::Extended);
    grid->setRowStretch(6,1); // this row can expand

//    //
    grid->addWidget(std::make_unique<Wt::WText>(_TR("Alias")), 7, 0);
    _aliasList = grid->addWidget(std::make_unique<Wt::WSelectionBox>(),7,1) ; // list of available values
    _aliasList->setSelectionMode(Wt::SelectionMode::Extended);
    grid->setRowStretch(7,1); // this row can expand
    //

    layout->addItem(std::move(grid));
//    //
//    // Populate as necessary from the report directory
//    //
//    // Set default values
    Wt::WDateTime et = Wt::WDateTime::currentDateTime();
    Wt::WDateTime st = et.addDays(-1);

    _startDate->setDate(st.date());
    _startTime->setTime(st.time());
    _endDate->setDate(et.date());
    _endTime->setTime(et.time());
    //
    // connect signals to slots
    //
    _newReport->clicked().connect(this, &MRL::WebReportPane::newReport);
    _loadReport->clicked().connect(this, &MRL::WebReportPane::loadReport);
    _saveReport->clicked().connect(this, &MRL::WebReportPane::saveReport);
    _removeReport->clicked().connect(this, &MRL::WebReportPane::removeReport);
    _makeReport->clicked().connect(this, &MRL::WebReportPane::makeReport);
    _viewReport->clicked().connect(this, &MRL::WebReportPane::viewReport);

    //
    updateReportList();
    //
    setLayout(std::move(boxLayout));
}

/*!
 * \brief MRL::WebReportPane::updateReportList
 */
void MRL::WebReportPane::updateReportList()
{
    _valueList->clear();
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
                std::string s = r->pathAsString() + ":" + in[j];
                _valueList->addItem(s);
            }
        }
    }
    //
    _aliasList->clear();
    MRL::StringMap &a = MRL::Common::aliasMap();
    for(auto i = a.begin(); i != a.end(); i++)
    {
        _aliasList->addItem(i->first);
    }
    //
    // get the report list
    std::string rd = MRL::Common::baseDir();
    rd +=  MRL::ReportConfig::reportDir;
    //
    wxArrayString files;
    wxDir::GetAllFiles 	(rd,&files, "*.rpt");
    if(files.Count() > 0)
    {
        for(int i = 0; i < files.Count(); i++)
        {
            std::cerr << files[i].ToStdString() << std::endl;
            wxFileName fn(files[i]);
            std::string rn = fn.GetName().ToStdString();
            _reportList->addItem(rn);
        }
    }
    _reportList->setCurrentIndex(0);


}


/*!
 * \brief MRL::WebReportPane::newReport
 */
void MRL::WebReportPane::newReport()
{
    // get report name
    Wt::WString rn;
    if(WebGetInput::getInput(this,_TR("New Report"),rn) && (!rn.empty()))
    {
        _reportList->addItem(rn);
        // set defaults
        _reportType->setCurrentIndex(0);
        // Set default values
        Wt::WDateTime et = Wt::WDateTime::currentDateTime();
        Wt::WDateTime st = et.addDays(-1);

        _startDate->setDate(st.date());
        _startTime->setTime(st.time());
        _endDate->setDate(et.date());
        _endTime->setTime(et.time());
        //
        toConfig();
        _config.saveAs(rn.toUTF8());
    }
}
/*!
 * \brief MRL::WebReportPane::loadReport
 */
void MRL::WebReportPane::loadReport()
{
    if(_reportList->count() > 0)
    {
        Wt::WString s = _reportList->currentText();
        if(!_config.load(s.toUTF8()))
        {
            wxLogDebug("Report Load Failed","Load Report");
        }
        else
        {
            // populate the fields
            MRL::PropertyPath p;
            p.push_back("Report");
            int tfs = _config.config().getValue<int>(p,"Timeframe");
            _reportType->setCurrentIndex(tfs);
            // get the datetime
            Wt::WDateTime start;
            std::string s = _config.config().getAsString(p,"Start");
            start = Wt::WDateTime::fromString(s,ISO_DATE_I_FMT);
            _startTime->setTime(start.time());
            _startDate->setDate(start.date());
            //
            s = _config.config().getAsString(p,"End");
            Wt::WDateTime end;
            end =   Wt::WDateTime::fromString(s,ISO_DATE_I_FMT);
            _endTime->setTime(end.time());
            _endDate->setDate(end.date());
            //
            // Now get the list of points in the report
            // clear value selections
            _valueList->clearSelection();
            p.push_back("Values");
            // get the list of child items
            {
                MRL::StringList l;
                _config.config().listChildren(p,l);
                std::set<int> sel;
                for(auto i  = l.begin(); i != l.end(); i++)
                {
                    std::string v = _config.config().getAsString(p, *i);
                    int j = _valueList->findText(v);
                    if(j >= 0) sel.insert(j);
                }
                _valueList->setSelectedIndexes(sel);
            }
            //
            p.pop_back();
            {
                _aliasList->clearSelection();
                p.push_back("Alias");
                // get the list of child items
                MRL::StringList l;
                _config.config().listChildren(p,l);
                std::set<int> sel;
                for(auto i  = l.begin(); i != l.end(); i++)
                {
                    std::string v = _config.config().getAsString(p, *i);
                    int j = _aliasList->findText(v);
                    if(j >= 0) sel.insert(j);
                }
                _aliasList->setSelectedIndexes(sel);
            }
        }
    }
}
/*!
 * \brief MRL::WebReportPane::saveReport
 */
void MRL::WebReportPane::saveReport()
{
    if(_reportList->count() > 0)
    {
        toConfig();
        _config.save();
    }
}
/*!
 * \brief MRL::WebReportPane::removeReport
 */
void MRL::WebReportPane::removeReport()
{
    if(_reportList->count() > 0)
    {
        if(Wt::WMessageBox::show(_TR("Remove Report"),_TR("Remove Report - Are You Sure?"),
                                 Wt::StandardButton::Ok | Wt::StandardButton::Cancel) == Wt::StandardButton::Ok)
        {
            Wt::WString s = _reportList->currentText();
            _config.deleteReport(s.toUTF8());
        }
    }
}
/*!
 * \brief MRL::WebReportPane::makeReport
 */
void MRL::WebReportPane::makeReport()
{
    if(_reportList->count() > 0)
    {
        toConfig();
        saveReport();
        if(_config.toReportGroup(_group))
        {
            Wt::WString s = _reportList->currentText();
            std::string rd = MRL::Common::baseDir() + MRL::ReportConfig::reportDir + "/" + s.toUTF8();
            //
            if(!wxDir::Exists(rd)) wxDir::Make(rd); // create the directory if necessary
            //
            _report = std::make_unique<MRL::Reporter>(s.toUTF8(), MRL::LocalDb::LOCAL_DB_DIR,MRL::LOCAL_DB_NAME);
            _report->setOutputDir(rd);
            if(_report->lock()) // avoid contention with other threads
            {
                //
                if(_report->fetch(_group))// generate the tables and the stats to a SQLITE database
                {
                    //
                    std::string rg = rd + "/" +  _report->name() + "_Image.png";
                    //
                    MRL::Graph g;
                    _report->createGraph(_group, g);
                    wxBitmap bm;
                    g.plotToBitmap(bm,800,600);
                    //
                    bm.SaveFile(rg,wxBITMAP_TYPE_PNG);
                    // now generate the HTML
                    _report->setPageLayout(3,60);
                    _report->setAddPageBreaks(true);
                    if(_report->createReportHtml(_group, true, true, true)) // generate the report as HTML
                    {
                        _report->toPDF(s.toUTF8()); // convert to PDF
                        viewReport();
                    }
                }
                _report->unlock();
            }
            _report.reset();
        }
    }
}


/*!
 * \brief MRL::WebReportPane::toConfig
 */
void MRL::WebReportPane::toConfig()
{
    _config.config().clear();
    // populate the fields
    MRL::PropertyPath p;
    p.push_back("Report");
    _config.config().setValue(p,"Timeframe",_reportType->currentIndex());
    //
    std::string s;
    // get the datetime
    Wt::WDateTime start;
    Wt::WDateTime end;
    //
    switch(_reportType->currentIndex())
    {
    case MRL::ReportConfig::TF_LAST_HOUR:
    {
        end = Wt::WDateTime::currentDateTime();
        start = end.addSecs(-3600);
    }
    break;
    case MRL::ReportConfig::TF_LAST_DAY:
    {
        end = Wt::WDateTime::currentDateTime();
        start = end.addDays(-1);
    }
    break;
    case MRL::ReportConfig::TF_LAST_WEEK:
    {
        end = Wt::WDateTime::currentDateTime();
        start = end.addDays(-7);
    }

    break;
    case MRL::ReportConfig::TF_EXACT:
    {
        //
        start.setTime(_startTime->time());
        start.setDate(_startDate->date());
        end.setTime(_endTime->time());
        end.setDate(_endDate->date());
    }
    break;
    default:
        break;
    }
    //
    _config.config().setValue(p,"Start",start.toString(ISO_DATE_Q_FMT).toUTF8());
    _config.config().setValue(p,"End",end.toString(ISO_DATE_Q_FMT).toUTF8());
    // get selected values to report on
    p.push_back("Values");
    //
    {
        const std::set<int> &sel = _valueList->selectedIndexes();
        int k = 0;
        for(auto i = sel.cbegin(); i != sel.cend(); i++)
        {
            std::string n = "I" + std::to_string(k++);
            _config.config().setValue(p,n,_valueList->itemText(*i).toUTF8());
        }
    }
    p.pop_back();
    // write back the selected alias
    p.push_back("Alias");
    {
        const std::set<int> &sel = _aliasList->selectedIndexes();
        int k = 0;
        for(auto i = sel.cbegin(); i != sel.cend(); i++)
        {
            std::string n = "I" + std::to_string(k++);
            _config.config().setValue(p,n,_aliasList->itemText(*i).toUTF8());
        }
    }
}

/*!
 * \brief MRL::WebReportPane::viewReport
 */
void MRL::WebReportPane::viewReport()
{
    if(_report && _report->pdfExists())
    {
        std::stringstream os;
        os <<  "http://" << wxGetHostName().ToStdString() << "/reports/" << _report->name() << "/" << _report->name() << ".pdf";
        WebReportCompleteDialog dlg(os.str());
        dlg.exec(); // blocks
    }
}


