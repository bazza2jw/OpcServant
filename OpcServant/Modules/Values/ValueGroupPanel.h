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
#ifndef VALUEGROUPPANEL_H
#define VALUEGROUPPANEL_H
#include "ValuesUI.h"
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/bobject.h>

/*!
 * \brief The ValueGroupPanel class
 */
class ValueGroupPanel : public ValueGroupPanelBase , public MRL::BObject
{
public:
    ValueGroupPanel(wxWindow* parent, unsigned id);
    virtual ~ValueGroupPanel();
    void setupTable(); // set upthe grid - called on start
    void updateTable(); // update the display
    bool processQueueItem(const MRL::Message &msg);

protected:
    virtual void onSizeGrid(wxSizeEvent& event);
    virtual void onPeriodicTimer(wxTimerEvent& event);
};
#endif // VALUEGROUPPANEL_H
