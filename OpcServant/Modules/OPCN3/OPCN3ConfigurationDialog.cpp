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
#include "OPCN3ConfigurationDialog.h"
#include <MrlLib/mrllib.h>

/*!
 * \brief OPCN3ConfigurationDialog::OPCN3ConfigurationDialog
 * \param parent
 * \param path
 */
OPCN3ConfigurationDialog::OPCN3ConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : OPCN3ConfigurationDialogBase(parent), _path(path)
{
    Json::Value v;
    if(MRL::Common::configuration().getData(path,v))
    {
        _configuration.fromJson(v); // load configuration
        GetWaitInterval()->SetValue(_configuration.getInt("/WaitInterval"));
        GetSampleInterval()->SetValue(_configuration.getInt("/SampleInterval"));
        GetAddress()->SetValue(_configuration.getAsWxString("/SPIDevice"));
        GetEnabled()->SetValue(_configuration.getBool("/Enabled"));
        GetEnableTabView()->SetValue(_configuration.getBool("/EnableTabView"));

    }
}

/*!
 * \brief OPCN3ConfigurationDialog::~OPCN3ConfigurationDialog
 */
OPCN3ConfigurationDialog::~OPCN3ConfigurationDialog()
{
}

/*!
 * \brief OPCN3ConfigurationDialog::onOk
 * \param event
 */
void OPCN3ConfigurationDialog::onOk(wxCommandEvent& /*event*/)
{
    _configuration.setInt("/WaitInterval",GetWaitInterval()->GetValue());
    _configuration.setInt("/SampleInterval",GetSampleInterval()->GetValue());
    _configuration.setString("/SPIDevice",GetAddress()->GetValue().ToStdString());
    _configuration.setBool("/Enabled",GetEnabled()->GetValue());
    _configuration.setBool("/EnableTabView",GetEnableTabView()->GetValue());

    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
