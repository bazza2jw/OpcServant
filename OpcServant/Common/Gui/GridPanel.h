#ifndef GRIDPANEL_H
#define GRIDPANEL_H

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

#include "BourbonCommonUI.h"
#include <Common/Daq/daqcommon.h>
#include <Common/common.h>
/*!
 * \brief The GridPanel class
 * Displays an RTObject's input values
 */
class GridPanel : public GridPanelBase
{
    unsigned _id = 0;

public:
    GridPanel(wxWindow* parent, unsigned id);
    virtual ~GridPanel();
    MRL::RtObjectRef & object()
    {
         return MRL::Common::daq().objects()[_id];
    }
    virtual void setupTable();
    virtual void updateTable();
    void setStatus(int r, const std::string &item);

protected:
    virtual void onTimer(wxTimerEvent& event);
};
#endif // GRIDPANEL_H
