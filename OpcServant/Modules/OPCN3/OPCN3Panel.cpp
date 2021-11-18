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
#include "OPCN3Panel.h"
#include "OPCN3rt.h"
/*!
 * \brief OPCN3Panel::OPCN3Panel
 * \param parent
 * \param id
 */
OPCN3Panel::OPCN3Panel(wxWindow* parent, unsigned id)
    : OPCN3PanelBase(parent),BObject(id)
{
    join(MRL::Daq::instance()); // attach to the DAQ manager
    MRL::BObject::setEnabled(true); // enable this object
}

OPCN3Panel::~OPCN3Panel()
{
}

bool OPCN3Panel::processQueueItem(const MRL::Message &msg)
{
    if (!MRL::BObject::processQueueItem(msg)) {
        // notifications from DAQ thread
        switch (msg.id()) {
        //
        case MESSAGEID::Started: // object has started
        {
            if(msg.source() == dbId())
            {
                m_status->SetBackgroundColour(*wxYELLOW);
                m_status->SetForegroundColour(*wxBLACK);
                m_status->SetLabelText(_("STARTED"));
            }
        }
        break;

        case MESSAGEID::Stopped: // object has started
        {
            if(msg.source() == dbId())
            {
                m_status->SetBackgroundColour(*wxYELLOW);
                m_status->SetForegroundColour(*wxBLACK);
                m_status->SetLabelText(_("STOPPED"));
            }
        }
        break;

        case MESSAGEID::IdleTimeout:

            break;

        case MESSAGEID::Update_Object:
        {
            if(msg.source() == dbId())
            {
                MRL::OPCN3RT * p = MRL::OPCN3RT::find(dbId());
                if(p)
                {
                    // get the counts
                    double pm1 = p->runtime().getDouble("/PM1/Value");
                    double p2_5 = p->runtime().getDouble("/PM2.5/Value");
                    double p10 = p->runtime().getDouble("/PM10/Value");
                    //
                    m_pm1->SetLabelText( wxString::Format("%6.1f",pm1));
                    m_pm2_5->SetLabelText( wxString::Format("%6.1f",p2_5));
                    m_pm10->SetLabelText( wxString::Format("%6.1f",p10));
                    m_status->SetLabelText(p->runtime().getAsWxString("/StatusDesc"));
                }
            }
        }
        break;

        default:
            return false;
        }
    }
    return true;
}

void OPCN3Panel::onTimer(wxTimerEvent& /*event*/)
{
    BObject::process();
}
