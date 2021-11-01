
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
#ifndef SBCSUPPORTSETUPDIALOG_H
#define SBCSUPPORTSETUPDIALOG_H
#include "SbcSupportUI.h"
#include "sbcsupportrt.h"
#include "sbcsupportobjectmanager.h"
/*!
 * \brief The SbcSupportSetupDialog class
 */
class SbcSupportSetupDialog : public SbcSupportSetupDialogBase
{
    const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
public:
    SbcSupportSetupDialog(wxWindow* parent,const MRL::PropertyPath &path);
    virtual ~SbcSupportSetupDialog();
protected:
    virtual void OnOK(wxCommandEvent& event);
};
#endif // SBCSUPPORTSETUPDIALOG_H
