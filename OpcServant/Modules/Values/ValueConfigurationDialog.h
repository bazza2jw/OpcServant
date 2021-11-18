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
#ifndef VALUECONFIGURATIONDIALOG_H
#define VALUECONFIGURATIONDIALOG_H
#include "ValuesUI.h"
#include <Common/common.h>
/*!
 * \brief The ValueConfigurationDialog class
 */
class ValueConfigurationDialog : public ValueConfigurationDialogBase
{
    MRL::PropertyPath  _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree &_configuration;
public:
    ValueConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path,MRL::VariantPropertyTree &c);
    virtual ~ValueConfigurationDialog();
protected:
    virtual void onOk(wxCommandEvent& event);
    virtual void OnOk(wxCommandEvent& event);
};
#endif // VALUECONFIGURATIONDIALOG_H
