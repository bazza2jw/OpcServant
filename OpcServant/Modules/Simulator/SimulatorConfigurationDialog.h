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
#ifndef SIMULATORCONFIGURATIONDIALOG_H
#define SIMULATORCONFIGURATIONDIALOG_H
#include "Simulator.h"
#include <Common/common.h>
/*!
 * \brief The SimulatorConfigurationDialog class
 */
class SimulatorConfigurationDialog : public SimulatorConfigurationDialogBase
{
     const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
public:
    SimulatorConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~SimulatorConfigurationDialog();
protected:
    virtual void OnOK(wxCommandEvent& event);
};
#endif // SIMULATORCONFIGURATIONDIALOG_H
