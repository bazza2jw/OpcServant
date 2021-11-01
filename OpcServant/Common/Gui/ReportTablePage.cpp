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
#include "ReportTablePage.h"
#include <Common/reporter.h>
#include <wx/font.h>

/*!
 * \brief ReportTablePage::ReportTablePage
 * \param r
 * \param n
 * \param parent
 */
ReportTablePage::ReportTablePage(MRL::Reporter *r, const std::string &n, wxWindow* parent)
    : ReportTablePageBase(parent),
      _report(r),
      _name(n)
{
    m_itemName->SetLabelText(n);
}

/*!
 * \brief ReportTablePage::~ReportTablePage
 */
ReportTablePage::~ReportTablePage()
{

}

/*!
 * \brief ReportTablePage::loadPage
 * \param page
 */
void ReportTablePage::loadPage(int page)
{
    if(_report)
    {
        // page size depends on the number of visible rows
        MRL::ReportItemList l;
        _report->resultsdb().getPage(_name,page,l,_nrow);
        GetTable()->DeleteAllItems();
        wxVector<wxVariant> data;
        for(auto i = l.begin(); i != l.end(); i++)
        {
            (*i).towxVector(data);
            GetTable()->AppendItem(data); // page in
        }
    }
}

/*!
 * \brief ReportTablePage::onScrollChanged
 * \param event
 */
void ReportTablePage::onScrollChanged(wxScrollEvent& event)
{
    if(_npages > 0)
    {
        _page = event.GetPosition();
        loadPage(_page);
    }
}

/*!
 * \brief ReportTablePage::onSize
 * \param event
 */
void ReportTablePage::onSize(wxSizeEvent& /*event*/)
{
    wxFont f = GetTable()->GetFont();
    wxSize sz = GetSize();
    if(sz.GetHeight() > 0)
    {
        int fh = f.GetPixelSize().GetY();
        _nrow = sz.GetHeight() / (fh + 2) - 2; // number of rows per page
        // now set the scrollbar range to the total number of pages
        if((_nrow > 0) && _report)
        {
            _npages = _report->resultsdb().getPageCount(_name,_nrow); // we now have the number of pages
            if(_npages > 0)
            {
                GetTableScrollBar()->SetRange(_npages);
                GetTableScrollBar()->SetThumbPosition(0);
            }
            else
            {
                GetTableScrollBar()->SetRange(1);
                GetTableScrollBar()->SetThumbPosition(0);
            }
        }
    }
}
