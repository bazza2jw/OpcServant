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
#ifndef ALIASCONFIGURATIONDIALOG_H
#define ALIASCONFIGURATIONDIALOG_H
#include "CommonGUI.h"

/*!
 * \brief The AliasConfigurationDialog class
 * Configure aliases
 */
class AliasConfigurationDialog : public AliasConfigurationDialogBase
{
public:
    AliasConfigurationDialog(wxWindow* parent);
    virtual ~AliasConfigurationDialog();
protected:
    virtual void onAdd(wxCommandEvent& event);
    virtual void onDelete(wxCommandEvent& event);
    virtual void onEdit(wxCommandEvent& event);
    virtual void onOK(wxCommandEvent& event);
};
#endif // ALIASCONFIGURATIONDIALOG_H
