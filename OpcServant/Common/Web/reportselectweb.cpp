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
#include "reportselectweb.h"
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
 * \brief MRL::ReportSelectWeb::ReportSelectWeb
 */
MRL::ReportSelectWeb::ReportSelectWeb()
{
    auto boxLayout = std::make_unique<Wt::WVBoxLayout>();
    _toolBar = boxLayout->addWidget(std::make_unique<Wt::WToolBar>()); // add the tool bar
    auto layout = boxLayout->addLayout(std::make_unique<Wt::WFitLayout>(),2);
    _makeReport = createColorButton("btn-primary", _TR("Make")); // this makes the report and generates the PDF that is then displayed - the browser can print this locally
    _makeReport->clicked().connect(this, &MRL::ReportSelectWeb::makeReport);


    auto grid = std::make_unique<Wt::WGridLayout>();
    grid->addWidget(std::make_unique<Wt::WText>(_TR("Report")), 0, 0);
    _reportList = grid->addWidget(std::make_unique<Wt::WComboBox>(),0,1);
    grid->addWidget(std::make_unique<Wt::WText>(_TR("Select Report to Make")), 1, 0);
    grid->setRowStretch(0,0);
    grid->setRowStretch(1,2);

    layout->addItem(std::move(grid));

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
    //
    setLayout(std::move(boxLayout));
}

/*!
 * \brief MRL::ReportSelectWeb::makeReport
 */
void MRL::ReportSelectWeb::makeReport()
{
    MRL::ReportConfig _config; // current configuration
    MRL::ReportGroup _group; // the results of a search
    std::unique_ptr<MRL::Reporter> _report; // report generator
    //
    // load the report
    if(_reportList->count() > 0)
    {
        Wt::WString s = _reportList->currentText();
        if(!_config.load(s.toUTF8()))
        {
            wxLogDebug("Report Load Failed","Load Report");
        }
        else
        {
            if(_config.toReportGroup(_group))
            {
                Wt::WString s = _reportList->currentText();
                std::string rd = MRL::Common::baseDir() + MRL::ReportConfig::reportDir + "/" + s.toUTF8();
                //
                if(!wxDir::Exists(rd)) wxDir::Make(rd); // create the directory if necessary
                //
                _report = std::make_unique<MRL::Reporter>(s.toUTF8(), MRL::Common::baseDir(),MRL::LOCAL_DB_NAME);
                _report->setOutputDir(rd);
                //
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
                        {
                            std::stringstream os;
                            os <<  "http://" << wxGetHostName().ToStdString() << "/reports/" << _report->name() << "/" << _report->name() << ".pdf";
                            WebReportCompleteDialog dlg(os.str());
                            dlg.exec(); // blocks
                        }
                    }
                }
                _report.reset();
            }
        }
    }
}

