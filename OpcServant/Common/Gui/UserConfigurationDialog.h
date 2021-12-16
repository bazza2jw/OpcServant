/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef USERCONFIGURATIONDIALOG_H
#define USERCONFIGURATIONDIALOG_H
#include "CommonGUI.h"
#include <Common/common.h>

/*!
 * \brief The UserConfigurationDialog class
 */
class UserConfigurationDialog : public UserConfigurationDialogBase
{
    MRL::VariantPropertyTree &_settings;
public:
    UserConfigurationDialog(wxWindow* parent,MRL::VariantPropertyTree &s);
    virtual ~UserConfigurationDialog();
protected:
    virtual void onOk(wxCommandEvent& event);
    virtual void onSelection(wxCommandEvent& event);
    virtual void onApply(wxCommandEvent& event);
    virtual void onDelete(wxCommandEvent& event);
    virtual void onNew(wxCommandEvent& event);
    void setup();
};
#endif // USERCONFIGURATIONDIALOG_H
