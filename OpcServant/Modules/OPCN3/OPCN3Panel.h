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
#ifndef OPCN3PANEL_H
#define OPCN3PANEL_H
#include "OPCN3UI.h"

#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/bobject.h>
/*!
 * \brief The OPCN3Panel class
 */
class OPCN3Panel : public OPCN3PanelBase, public MRL::BObject
{
public:
    OPCN3Panel(wxWindow* parent, unsigned id);
    virtual ~OPCN3Panel();
    bool processQueueItem(const MRL::Message &msg);

protected:
    virtual void onTimer(wxTimerEvent& event);
};
#endif // OPCN3PANEL_H
