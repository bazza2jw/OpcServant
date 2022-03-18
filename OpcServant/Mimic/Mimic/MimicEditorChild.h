/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#ifndef MimicEditorChild_H
#define MimicEditorChild_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>

#include <wx/mdi.h>

#include "MimicEditorPanel.h"

class MimicEditorChild : public wxMDIChildFrame
{
    MimicEditorPanel * _panel = nullptr;
protected:
    wxBoxSizer* canvasSizer = nullptr;
    MimicCanvas * _canvas = nullptr;

public:
    MimicEditorChild(wxMDIParentFrame * parent);
    virtual ~MimicEditorChild();
    void onClose(wxCommandEvent& event);

};
#endif // MimicEditorChild_H
