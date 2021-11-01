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
#ifndef MARIADBSETUPDIALOG_H
#define MARIADBSETUPDIALOG_H
#include "MariaDbPubSetup.h"
#include <MrlLib/variantpropertytree.h>
/*!
 * \brief The MariaDbSetupDialog class
 */
class MariaDbSetupDialog : public MariaDbSetupDialogBase
{
    MRL::PropertyPath _path;
public:
    MariaDbSetupDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~MariaDbSetupDialog();
protected:
    virtual void OnOK(wxCommandEvent& event);
    virtual void OnTest(wxCommandEvent& event);
};
#endif // MARIADBSETUPDIALOG_H
