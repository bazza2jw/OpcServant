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
#ifndef SYSTEMPROPERTIESDIALOG_H
#define SYSTEMPROPERTIESDIALOG_H
#include "CommonGUI.h"
#include <Common/common.h>
/*!
 * \brief The SystemPropertiesDialog class
 */
class SystemPropertiesDialog : public SystemPropertiesDialogBase
{
    MRL::VariantPropertyTree _settings;

public:
    SystemPropertiesDialog(wxWindow* parent);
    virtual ~SystemPropertiesDialog();
protected:
    virtual void onUsers(wxCommandEvent& event);
    virtual void onPurge(wxCommandEvent& event);
    virtual void onFactoryReset(wxCommandEvent& event);
    virtual void OnOK(wxCommandEvent& event);
    void setup();
};
#endif // SYSTEMPROPERTIESDIALOG_H
