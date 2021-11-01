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
#include "LwsApexConfigurationDialog.h"
#include <MrlLib/mrllib.h>

/*!
 * \brief LwsApexConfigurationDialog::LwsApexConfigurationDialog
 * \param parent
 * \param path
 */
LwsApexConfigurationDialog::LwsApexConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : LwsApexConfigurationDialogBase(parent), _path(path)
{
    Json::Value v;
    if(MRL::Common::configuration().getData(path,v))
    {
        _configuration.fromJson(v); // load configuration
        GetPurgeTime()->SetValue(_configuration.getInt("PurgeTime"));
        GetSampleTime()->SetValue(_configuration.getInt("SampleTime"));
        GetAddress()->SetValue(_configuration.getAsWxString("Address"));
        GetEnabled()->SetValue(_configuration.getBool("Enabled"));
    }
}

/*!
 * \brief LwsApexConfigurationDialog::~LwsApexConfigurationDialog
 */
LwsApexConfigurationDialog::~LwsApexConfigurationDialog()
{
}

/*!
 * \brief LwsApexConfigurationDialog::onOk
 * \param event
 */
void LwsApexConfigurationDialog::onOk(wxCommandEvent& /*event*/)
{
    _configuration.setInt("PurgeTime",GetPurgeTime()->GetValue());
    _configuration.setInt("SampleTime",GetSampleTime()->GetValue());
    _configuration.setString("Address",GetAddress()->GetValue().ToStdString());
    _configuration.setBool("Enabled",GetEnabled()->GetValue());
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
