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
#ifndef REPORTTABLEPAGE_H
#define REPORTTABLEPAGE_H
#include "CommonGUI.h"

namespace MRL
{
    class Reporter;
}
/*!
 * \brief The ReportTablePage class
 */
class ReportTablePage : public ReportTablePageBase
{
    MRL::Reporter * _report = nullptr;
    std::string _name; // object name
    int _nrow = 5;
    int _page = 0;
    int _npages = 1;
public:
    ReportTablePage(MRL::Reporter * r, const std::string &n, wxWindow* parent);
    virtual ~ReportTablePage();
    void setReporter(MRL::Reporter *r = nullptr) { _report = r;}
    //
    // Populate the table from the database
    void loadPage(int page);
protected:
    virtual void onSize(wxSizeEvent& event);
    virtual void onScrollChanged(wxScrollEvent& event);
};
#endif // REPORTTABLEPAGE_H
