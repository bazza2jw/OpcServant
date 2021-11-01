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
#ifndef OPCN3CONFIGURATIONDIALOG_H
#define OPCN3CONFIGURATIONDIALOG_H
#include "OPCN3UI.h"
#include <Common/common.h>
/*!
 * \brief The OPCN3ConfigurationDialog class
 */
class OPCN3ConfigurationDialog : public OPCN3ConfigurationDialogBase
{
    const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree _configuration; // tree of configuration values

public:
    OPCN3ConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~OPCN3ConfigurationDialog();
protected:
    virtual void onOk(wxCommandEvent& event);
};
#endif // OPCN3CONFIGURATIONDIALOG_H
