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
#ifndef MQTTSETUPDIALOG_H
#define MQTTSETUPDIALOG_H
#include "MQTTUI.h"
#include <Common/common.h>

/*!
 * \brief The MqttSetupDialog class
 */
class MqttSetupDialog : public MqttSetupDialogBase
{
    MRL::PropertyPath _path;
public:
    MqttSetupDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~MqttSetupDialog();
protected:
    virtual void OnOk(wxCommandEvent& event);
};
#endif // MQTTSETUPDIALOG_H
