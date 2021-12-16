/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#include "GridPanel.h"
#include <wx/timer.h>
/*!
 * \brief GridPanel::GridPanel
 * \param parent
 */
GridPanel::GridPanel(wxWindow* parent, unsigned id)
    : GridPanelBase(parent),_id(id)
{
    wxFont f = GetGrid()->GetFont();
    int w = MRL::getTextWidth(f,10);
    GetGrid()->SetColSize(0,w);
    GetGrid()->SetColSize(1,w);
    setupTable();
}

/*!
 * \brief GridPanel::~GridPanel
 */
GridPanel::~GridPanel()
{
}

/*!
 * \brief GridPanel::onTimer
 */
void GridPanel::onTimer(wxTimerEvent& /*event*/)
{
    updateTable();
}

/*!
 * \brief GridPanel::setStatus
 * \param r
 * \param item
 */
void GridPanel::setStatus( int r, const std::string &item)
{
    if(object() && (r < GetGrid()->GetNumberRows()))
    {
        MRL::PropertyPath p;
        p.push_back(item);
        int i = object()->runtime().getValue<int>(p,MRL::STATE_TAG);
        // set the colours
        switch(i)
        {
        case STATES::States::StateFault:
            GetGrid()->SetCellValue(r,1,_("FAULT"));
            GetGrid()->SetCellBackgroundColour (r, 0, *wxRED);
            GetGrid()->SetCellBackgroundColour (r, 1, *wxRED);
            GetGrid()->SetCellTextColour (r,0,*wxWHITE);
            GetGrid()->SetCellTextColour (r,1,*wxWHITE);

            break;
        case STATES::States::StateLoLo:
            GetGrid()->SetCellValue(r,1,_("UNDER"));
            GetGrid()->SetCellBackgroundColour (r, 0, *wxYELLOW);
            GetGrid()->SetCellBackgroundColour (r, 1, *wxYELLOW);
            GetGrid()->SetCellTextColour (r,0,*wxBLACK);
            GetGrid()->SetCellTextColour (r,1,*wxBLACK);
            break;
        case  STATES::States::StateHiHi:
            GetGrid()->SetCellValue(r,1,_("OVER"));
            GetGrid()->SetCellBackgroundColour (r, 0, *wxYELLOW);
            GetGrid()->SetCellBackgroundColour (r, 1, *wxYELLOW);
            GetGrid()->SetCellTextColour (r,0,*wxBLACK);
            GetGrid()->SetCellTextColour (r,1,*wxBLACK);
            break;
        case STATES::States::StateOk:
            GetGrid()->SetCellValue(r,1,_("OK"));
            GetGrid()->SetCellBackgroundColour (r, 0, *wxGREEN);
            GetGrid()->SetCellBackgroundColour (r, 1, *wxGREEN);
            GetGrid()->SetCellTextColour (r,0,*wxBLACK);
            GetGrid()->SetCellTextColour (r,1,*wxBLACK);
            break;
        default:
            GetGrid()->SetCellValue(r,1,_("Waiting"));
            GetGrid()->SetCellBackgroundColour (r, 0, *wxWHITE);
            GetGrid()->SetCellBackgroundColour (r, 1, *wxWHITE);
            GetGrid()->SetCellTextColour (r,0,*wxBLACK);
            GetGrid()->SetCellTextColour (r,1,*wxBLACK);
            break;
        }
    }
}


/*!
 * \brief GridPanel::setupTable
 */
void GridPanel::setupTable()
{
    MRL::RtObjectRef &r = object(); // get the object
    if(r)
    {
        MRL::StringVector &l = r->inputs(); // get the list of inputs
        if(l.size())
        {
            GetGrid()->AppendRows(l.size());
            for(size_t i = 0; i < l.size(); i++)
            {
                GetGrid()->SetRowLabelValue(i,l[i]);
                GetGrid()->SetCellValue(i,0,_("0.0"));
            }
        }
    }

}
/*!
 * \brief GridPanel::updateTable
 */
void GridPanel::updateTable()
{
    MRL::RtObjectRef &r = object(); // get the object
    if(r)
    {
        MRL::StringVector &l = r->inputs(); // get the list of inputs
        if(l.size())
        {
            for(size_t i = 0; i < l.size(); i++)
            {
                char b[32];
                sprintf(b,"%8.1f",r->getInput<double>(l[i]));
                GetGrid()->SetCellValue(i,0,b);
                setStatus( i, l[i]);
            }
        }
    }
}
