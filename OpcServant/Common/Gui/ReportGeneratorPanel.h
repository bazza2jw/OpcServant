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
#ifndef REPORTGENERATORPANEL_H
#define REPORTGENERATORPANEL_H
#include "CommonGUI.h"
#include <Common/reporter.h>
#include <Common/reportconfig.h>
#include "ReportTablePage.h"
#include <Common/reportgraph.h>

/*!
 * \brief The ReportGeneratorPanel class
 */
class ReportGeneratorPanel : public ReportGeneratorPanelBase
{
    //
    MRL::ReportConfig _config; // current configuration
    MRL::ReportGroup _group; // the results of a search
    std::unique_ptr<MRL::Reporter> _report;
    //
    std::vector<ReportTablePage *> _tablePages;
    //
    class reportTimer : public wxTimer
    {
        ReportGeneratorPanel *_r = nullptr;
        public:
        reportTimer(ReportGeneratorPanel *w) : _r(w) {}
        void Notify()
        {
            _r->loadPages();
        }
    } _updateTrigger;
public:
    ReportGeneratorPanel(wxWindow* parent);
    virtual ~ReportGeneratorPanel();
    void toConfig();
    void setValueList();
    void addPages();
    void loadPages();
    void setReportList();
protected:
    virtual void onExport(wxCommandEvent& event);
    virtual void onPrint(wxCommandEvent& event);
    virtual void onRefreshValues(wxCommandEvent& event);
    virtual void OnNew(wxCommandEvent& event);
    virtual void OnTimeframeChoice(wxCommandEvent& event);
    virtual void onLoadReport(wxCommandEvent& event);
    virtual void onMakeReport(wxCommandEvent& event);
    virtual void onRemoveReport(wxCommandEvent& event);
    virtual void onSaveReport(wxCommandEvent& event);
};
#endif // REPORTGENERATORPANEL_H
