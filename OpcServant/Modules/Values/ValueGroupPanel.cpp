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
#include "ValueGroupPanel.h"
#include <Common/Daq/daqcommon.h>
#include "valuert.h"


/*!
 * \brief ValueGroupPanel::ValueGroupPanel
 * \param parent
 * \param id
 */
ValueGroupPanel::ValueGroupPanel(wxWindow* parent, unsigned id)
    : ValueGroupPanelBase(parent), MRL::BObject(id)
{
    join(MRL::Daq::instance()); // attach to the DAQ manager
    MRL::BObject::setEnabled(true); // enable this object
    //
    int w = GetValues()->GetSize().GetWidth() / 5;
    GetValues()->SetColSize(0,w);
    GetValues()->SetColSize(1,w);
    GetValues()->SetColSize(2,w);
    GetValues()->SetColSize(3,w);
    GetValues()->SetColSize(4,w);
    setupTable();

}

/*!
 * \brief ValueGroupPanel::~ValueGroupPanel
 */
ValueGroupPanel::~ValueGroupPanel()
{
}

/*!
 * \brief ValueGroupPanel::setup
 */
void ValueGroupPanel::setupTable() // set up the grid - called on start
{
    GetValues()->ClearGrid();
    MRL::ValueRT *v =  MRL::ValueRT::find(dbId());
    if(v)
    {
        if(v->values().size() > 0)
        {
            int j = 0;
            GetValues()->InsertRows(0, v->values().size());
            for(auto i = v->values().begin(); i != v->values().end(); i++, j++)
            {
                GetValues()->SetRowLabelValue(j,i->first);
                GetValues()->SetCellValue(j,0,"0.0");
                GetValues()->SetCellValue(j,1,"0.0");
                GetValues()->SetCellValue(j,2,"0.0");
                GetValues()->SetCellValue(j,3,"0.0");
                GetValues()->SetCellValue(j,4,"OK");
            }
        }
    }
}

/*!
 * \brief ValueGroupPanel::update
 */
void ValueGroupPanel::updateTable() // update the display
{
    MRL::ValueRT *v =  MRL::ValueRT::find(dbId());
    if(v)
    {
        for(auto i = v->values().begin(); i != v->values().end(); i++)
        {
            MRL::ValueRT::ValueItem &a = i->second;
            //
            // find the row
            //
            wxString n(i->first);
            //
            for(int j = 0; j < GetValues()->GetNumberRows(); j++)
            {
                if(GetValues()->GetRowLabelValue(j) == n)
                {
                    //
                    GetValues()->SetCellValue(j,0,wxString::Format("%5.1f",a.stats().statistics().getLastValue()));
                    GetValues()->SetCellValue(j,1,wxString::Format("%5.1f",a.stats().statistics().getMean()));
                    GetValues()->SetCellValue(j,2,wxString::Format("%5.1f",a.stats().statistics().getMinimum()));
                    GetValues()->SetCellValue(j,3,wxString::Format("%5.1f",a.stats().statistics().getMaximum()));
                    //
                    switch(a.state())
                    {
                    case STATES::States::StateOk:
                        GetValues()->SetCellValue(j,4,_("OK"));
                        GetValues()->SetCellBackgroundColour(j,4,*wxGREEN);
                        GetValues()->SetCellTextColour(j,4,*wxBLACK);
                        break;
                    case STATES::States::StateAction:
                        GetValues()->SetCellValue(j,4,_("ACTION"));
                        GetValues()->SetCellBackgroundColour(j,4,*wxRED);
                        GetValues()->SetCellTextColour(j,4,*wxWHITE);
                        break;
                    case STATES::States::StateAlert:
                        GetValues()->SetCellValue(j,4,_("ALERT"));
                        GetValues()->SetCellBackgroundColour(j,4,*wxYELLOW);
                        GetValues()->SetCellTextColour(j,4,*wxBLACK);
                        break;
                    default:
                        GetValues()->SetCellValue(j,4,_(""));
                        GetValues()->SetCellBackgroundColour(j,4,*wxWHITE);
                        GetValues()->SetCellTextColour(j,4,*wxBLACK);
                        break;
                    }
                    break;
                }
            }
        }


        switch(v->runtime().getInt("/groupState"))
        {
        case STATES::States::StateOk:
            GetStatus()->SetLabelText(_("OK"));
            GetStatus()->SetBackgroundColour(*wxGREEN);
            GetStatus()->SetForegroundColour(*wxBLACK);
            break;
        case STATES::States::StateAction:
            GetStatus()->SetLabelText(_("ACTION"));
            GetStatus()->SetBackgroundColour(*wxRED);
            GetStatus()->SetForegroundColour(*wxWHITE);
            break;
        case STATES::States::StateAlert:
            GetStatus()->SetLabelText(_("ALERT"));
            GetStatus()->SetBackgroundColour(*wxYELLOW);
            GetStatus()->SetForegroundColour(*wxBLACK);
            break;
        default:
            GetStatus()->SetLabelText(_(""));
            GetStatus()->SetBackgroundColour(*wxWHITE);
            GetStatus()->SetForegroundColour(*wxBLACK);
            break;
        }

    }
    else
    {
        GetStatus()->SetLabelText(_("MISSING"));
    }
}


/*!
 * \brief ValueGroupPanel::processQueueItem
 * \param msg
 * \return
 */
bool ValueGroupPanel::processQueueItem(const MRL::Message &msg)
{
    if (!MRL::BObject::processQueueItem(msg)) {
        // notifications from DAQ thread
        switch (msg.id()) {
        //
        case MESSAGEID::Started: // object has started
        {
            if(msg.source() == dbId())
            {
                GetStatus()->SetBackgroundColour(*wxYELLOW);
                GetStatus()->SetForegroundColour(*wxBLACK);
                GetStatus()->SetLabelText(_("STARTED"));
            }
        }
        break;

        case MESSAGEID::Stopped: // object has started
        {
            if(msg.source() == dbId())
            {
                GetStatus()->SetBackgroundColour(*wxYELLOW);
                GetStatus()->SetForegroundColour(*wxBLACK);
                GetStatus()->SetLabelText(_("STOPPED"));
            }
        }
        break;

        case MESSAGEID::IdleTimeout:

            break;

        case MESSAGEID::Update_Object:
        {
            if(msg.source() == dbId())
            {
                updateTable();
            }
        }
        break;

        default:
            return false;
        }
    }
    return true;
}

/*!
 * \brief ValueGroupPanel::onPeriodicTimer
 */
void ValueGroupPanel::onPeriodicTimer(wxTimerEvent& /*event*/)
{
   process(); // drive the event queue
}
/*!
 * \brief ValueGroupPanel::onSizeGrid
 * \param event
 */
void ValueGroupPanel::onSizeGrid(wxSizeEvent& event)
{
    int w = GetValues()->GetSize().GetWidth() / 5;
    GetValues()->SetColSize(0,w);
    GetValues()->SetColSize(1,w);
    GetValues()->SetColSize(2,w);
    GetValues()->SetColSize(3,w);
    GetValues()->SetColSize(4,w);
}
