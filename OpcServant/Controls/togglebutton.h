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
#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <wx/tglbtn.h>
#include <wx/button.h>
class ToggleButton : public wxBitmapToggleButton
{
public:
    ToggleButton(wxWindow *parent, wxWindowID id=wxID_ANY);
    void SetValue(bool f);
    void toggle(wxCommandEvent&);

};

#endif // TOGGLEBUTTON_H
