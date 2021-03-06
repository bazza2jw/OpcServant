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
#ifndef ADDALIASDIALOG_H
#define ADDALIASDIALOG_H
#include "CommonGUI.h"

/*!
 * \brief The AddAliasDialog class
 * Get an alias name
 */
class AddAliasDialog : public AddAliasDialogBase
{
public:
    AddAliasDialog(wxWindow* parent);
    virtual ~AddAliasDialog();
protected:
    virtual void onOK(wxCommandEvent& event);
    virtual void onSelect(wxCommandEvent& event);
};
#endif // ADDALIASDIALOG_H
