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
#include "LwsApexPanel.h"
#include "lwsapexrt.h"
/*!
 * \brief LwsApexPanel::LwsApexPanel
 * \param parent
 * \param id
 */
LwsApexPanel::LwsApexPanel(wxWindow* parent, unsigned id)
    : LwsApexPanelBase(parent),BObject(id)
{
}

LwsApexPanel::~LwsApexPanel()
{
}

bool LwsApexPanel::processQueueItem(const MRL::Message &msg)
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
                MRL::LwsApexRT * p = MRL::LwsApexRT::find(dbId());
                if(p)
                {
                    // get the counts
                    double p0 = p->runtime().getDouble("/0.5/Value");
                    double p1 = p->runtime().getDouble("/5.0/Value");
                    //
                    m_value_0_5->SetLabelText( wxString::Format("%6.1f",p0));
                    m_value_5_0->SetLabelText( wxString::Format("%6.1f",p1));
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
