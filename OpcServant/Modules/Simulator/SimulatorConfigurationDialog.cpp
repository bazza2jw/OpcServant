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
#include "SimulatorConfigurationDialog.h"
#include <MrlLib/mrllib.h>
/*!
 * \brief SimulatorConfigurationDialog::SimulatorConfigurationDialog
 * \param parent
 * \param path
 */
SimulatorConfigurationDialog::SimulatorConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : SimulatorConfigurationDialogBase(parent),_path(path)
{
    // get the configuration from database
    Json::Value v;
    if(MRL::Common::configuration().getData(path,v))
    {
        // do not always have to convert to tree - esp if simple property list
        Json::Value &tv = v["Type"];
        Json::Value &rv = v["Range"];
        Json::Value &mi = v["MeasureInterval"];
        Json::Value &pi = v["PublishInterval"];
        Json::Value &ei = v["Enabled"];
        //
        MRL::SetChoice(GetType(),tv.asString());
        GetRange()->SetValue(rv.asInt());
        GetMeasureInterval()->SetValue(mi.asInt());
        GetPublishInterval()->SetValue(pi.asInt());
        GetEnabled()->SetValue(ei.asBool());
        //
    }
}

/*!
 * \brief SimulatorConfigurationDialog::~SimulatorConfigurationDialog
 */
SimulatorConfigurationDialog::~SimulatorConfigurationDialog()
{

}
/*!
 * \brief SimulatorConfigurationDialog::OnOK
 * \param event
 */
void SimulatorConfigurationDialog::OnOK(wxCommandEvent& /*event*/)
{
    Json::Value v;
    v["Type"] = MRL::GetChoice(GetType());
    v["Range"] = GetRange()->GetValue();
    v["MeasureInterval"] = GetMeasureInterval()->GetValue();
    v["PublishInterval"] = GetPublishInterval()->GetValue();
    v["Enabled"] = GetEnabled()->GetValue();
    MRL::Common::configuration().updateData(_path,v);
    //
    // TO DO Post update notification to DAQ thread to sync config
    //
    EndModal(wxID_OK);
}
