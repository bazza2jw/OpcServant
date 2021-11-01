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
#ifndef ALARMCONFIGURATIONDIALOG_H
#define ALARMCONFIGURATIONDIALOG_H
#include "AlarmsUI.h"
#include "alarms.h"
class AlarmConfigurationDialog : public AlarmConfigurationDialogBase
{
    MRL::PropertyPath _path;
public:
    AlarmConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~AlarmConfigurationDialog();
protected:
    virtual void OnActionObject(wxCommandEvent& event);
    virtual void OnAlertObject(wxCommandEvent& event);
    virtual void OnOkObject(wxCommandEvent& event);
    virtual void OnOk(wxCommandEvent& event);
    void setOutputs(const std::string &outObject, wxChoice *c);
};
#endif // ALARMCONFIGURATIONDIALOG_H
