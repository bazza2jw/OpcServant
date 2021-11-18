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
#ifndef SBCSUPPORTTABPANEL_H
#define SBCSUPPORTTABPANEL_H
#include "SbcSupportUI.h"

/*!
 * \brief The SbcSupportTabPanel class
 */
class SbcSupportTabPanel : public SbcSupportTabPanelBase
{
    unsigned _objectId = 0;
public:
    SbcSupportTabPanel(wxWindow* parent, unsigned id);
    virtual ~SbcSupportTabPanel();

protected:
    virtual void onFactoryDefaults(wxCommandEvent& event);
    virtual void onOpenConsole(wxCommandEvent& event);
    virtual void onRestart(wxCommandEvent& event);
    virtual void onSetTime(wxCommandEvent& event);
};
#endif // SBCSUPPORTTABPANEL_H
