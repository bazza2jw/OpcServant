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
#ifndef SELECTVALUEDIALOG_H
#define SELECTVALUEDIALOG_H
#include "CommonGUI.h"
/*!
 * \brief The SelectValueDialog class
 */
class SelectValueDialog : public SelectValueDialogBase
{
public:
    wxString selected;
    SelectValueDialog(wxWindow* parent);
    virtual ~SelectValueDialog();
protected:
    virtual void onSelect(wxCommandEvent& event);
    virtual void onOK(wxCommandEvent& event);
};
#endif // SELECTVALUEDIALOG_H
