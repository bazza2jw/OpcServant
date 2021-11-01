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
#ifndef IOIFDISPLAY_H
#define IOIFDISPLAY_H
#include "IOIFUI.h"

class IoifDisplay : public IoifDisplayBase
{
public:
    IoifDisplay(wxWindow* parent);
    virtual ~IoifDisplay();
protected:
    virtual void onAllOff(wxCommandEvent& event);
    virtual void onAllOn(wxCommandEvent& event);
    virtual void onDacChanged(wxScrollEvent& event);
    virtual void onOUT0(wxCommandEvent& event);
    virtual void onOut1(wxCommandEvent& event);
    virtual void onOut2(wxCommandEvent& event);
    virtual void onOut3(wxCommandEvent& event);
    virtual void onOut4(wxCommandEvent& event);
};
#endif // IOIFDISPLAY_H
