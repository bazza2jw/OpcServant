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
#ifndef ICPCON7017PANEL_H
#define ICPCON7017PANEL_H
#include "../icpcon.h"
#include "ICPCON7017UI.h"
#include <Common/bobject.h>
#include "icpcon7017objectmanager.h"
#include "icpcon7017rt.h"
/*!
 * \brief The ICPCON7017Panel class
 */
class ICPCON7017Panel : public ICPCON7017PanelBase, public MRL::BObject
{
    unsigned _id;

public:
    ICPCON7017Panel(wxWindow* parent, unsigned id);
    virtual ~ICPCON7017Panel();
    bool processQueueItem(const MRL::Message &);
    //
    wxStaticText * _values[MRL::NUMBER_7017_CHAN];
    wxStaticText * _units[MRL::NUMBER_7017_CHAN];
    //
    void loadFields();

protected:
    virtual void onProcessTimer(wxTimerEvent& event);
    virtual void onPause(wxCommandEvent& event);
};
#endif // ICPCON7017PANEL_H
