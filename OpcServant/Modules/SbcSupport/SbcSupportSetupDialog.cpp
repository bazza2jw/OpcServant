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
#include "SbcSupportSetupDialog.h"
/*!
 * \brief SbcSupportSetupDialog::SbcSupportSetupDialog
 * \param parent
 */
SbcSupportSetupDialog::SbcSupportSetupDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : SbcSupportSetupDialogBase(parent),_path(path)
{
    // get the configuration from database
    Json::Value v;
    if(MRL::Common::configuration().getData(path,v))
    {
        // do not always have to convert to tree - esp if simple property list
        Json::Value &ecc = v["EnableClockChange"];
        Json::Value &ehc = v["EnableHardwareClock"];
        Json::Value &efd = v["EnableFactoryDefaults"];
        Json::Value &ec = v["EnableConsole"];
        Json::Value &er = v["EnableRestart"];
        Json::Value &etv = v["EnableTabView"];
        Json::Value &ei = v["Enabled"];
        GetEnabled()->SetValue(ei.asBool());
        GetEnableClockChange()->SetValue(ecc.asBool());
        GetEnableHardwareClock()->SetValue(ehc.asBool());
        GetEnableFactoryDefaults()->SetValue(efd.asBool());
        GetEnableConsole()->SetValue(ec.asBool());
        GetEnableRestart()->SetValue(er.asBool());
        GetEnableTabView()->SetValue(etv.asBool());
    }
}
/*!
 * \brief SbcSupportSetupDialog::~SbcSupportSetupDialog
 */
SbcSupportSetupDialog::~SbcSupportSetupDialog()
{
}

/*!
 * \brief SbcSupportSetupDialog::OnOK
 * \param event
 */
void SbcSupportSetupDialog::OnOK(wxCommandEvent& /*event*/)
{
    Json::Value v;
    v["Enabled"] = GetEnabled()->GetValue();
    v["EnableClockChange"] = GetEnableClockChange()->GetValue();
    v["EnableHardwareClock"] = GetEnableHardwareClock()->GetValue();
    v["EnableFactoryDefaults"] = GetEnableFactoryDefaults()->GetValue();
    v["EnableConsole"] = GetEnableConsole()->GetValue();
    v["EnableRestart"] = GetEnableRestart()->GetValue();
    v["EnableTabView"] = GetEnableTabView()->GetValue();
    //
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
