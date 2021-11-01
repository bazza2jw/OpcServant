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
#ifndef VALUEGROUPCONFIGURATIONDIALOG_H
#define VALUEGROUPCONFIGURATIONDIALOG_H
#include "ValuesUI.h"
#include <Common/common.h>
#include <MrlLib/variantpropertytree.h>
/*!
 * \brief The ValueGroupConfigurationDialog class
 */
class ValueGroupConfigurationDialog : public ValueGroupConfigurationDialogBase
{
    const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree _configuration; // tree of configuration values

public:
    ValueGroupConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~ValueGroupConfigurationDialog();
protected:
    virtual void onAlias(wxCommandEvent& event);
    virtual void onOk(wxCommandEvent& event);
    virtual void onAdd(wxCommandEvent& event);
    virtual void onEdit(wxCommandEvent& event);
    virtual void onRemove(wxCommandEvent& event);
};
#endif // VALUEGROUPCONFIGURATIONDIALOG_H
