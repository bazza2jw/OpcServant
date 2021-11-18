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
#include "MqttSetupDialog.h"

/*!
 * \brief MqttSetupDialog::MqttSetupDialog
 * \param parent
 * \param path
 */
MqttSetupDialog::MqttSetupDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : MqttSetupDialogBase(parent),_path(path)
{
    // get the configuration from database
    Json::Value v;
    if (MRL::Common::configuration().getData(path, v)) {
        // do not always have to convert to tree - esp if simple property list
        Json::Value &ht = v["Ident"];
        Json::Value &hs = v["Host"];
        Json::Value &db = v["Filter"];
        Json::Value &pt = v["Port"];
        Json::Value &ei = v["Enabled"];
        Json::Value &un = v["Username"];
        Json::Value &pw = v["Password"];
        Json::Value &cm = v["Command"];
        //
        GetUsername()->SetValue(un.asString());
        GetPassword()->SetValue(pw.asString());
        GetHost()->SetValue(hs.asString());
        GetIdent()->SetValue(ht.asString());
        GetFilter()->SetValue(db.asString());
        GetPort()->SetValue(pt.asInt());
        GetEnable()->SetValue(ei.asBool());
        GetCommandSub()->SetValue(cm.asString());
        //
    }
}

/*!
 * \brief MqttSetupDialog::~MqttSetupDialog
 */
MqttSetupDialog::~MqttSetupDialog()
{
}

/*!
 * \brief MqttSetupDialog::OnOk
 */
void MqttSetupDialog::OnOk(wxCommandEvent& /*event*/)
{
    Json::Value v;
    //
    v["Ident"] = GetIdent()->GetValue().ToStdString();
    v["Host"] = GetHost()->GetValue().ToStdString();
    v["Filter"] = GetFilter()->GetValue().ToStdString();
    v["Port"] = GetPort()->GetValue();
    v["Enabled"]  = GetEnable()->GetValue();
    v["Username"] = GetUsername()->GetValue().ToStdString();
    v["Password"] = GetPassword()->GetValue().ToStdString();
    v["Command"] = GetCommandSub()->GetValue().ToStdString();

    //
    MRL::Common::configuration().updateData(_path, v);
    //
    EndModal(wxID_OK);
}
