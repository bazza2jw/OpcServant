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
#include "LinkSetupDialog.h"

/*!
 * \brief LinkSetupDialog::LinkSetupDialog
 * \param parent
 * \param path
 */
LinkSetupDialog::LinkSetupDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : LinkSetupDialogBase(parent),_path(path)
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
        //
        GetHost()->SetValue(hs.asString());
        GetIdent()->SetValue(ht.asString());
        GetFilter()->SetValue(db.asString());
        GetPort()->SetValue(pt.asInt());
        GetEnable()->SetValue(ei.asBool());
    }
}

/*!
 * \brief LinkSetupDialog::~LinkSetupDialog
 */
LinkSetupDialog::~LinkSetupDialog()
{
}

/*!
 * \brief LinkSetupDialog::OnOk
 */
void LinkSetupDialog::OnOk(wxCommandEvent& /*event*/)
{
    Json::Value v;
    //
    v["Ident"] = GetIdent()->GetValue().ToStdString();
    v["Host"] = GetHost()->GetValue().ToStdString();
    v["Filter"] = GetFilter()->GetValue().ToStdString();
    v["Port"] = GetPort()->GetValue();
    v["Enabled"]  = GetEnable()->GetValue();
    //
    MRL::Common::configuration().updateData(_path, v);
    //
    EndModal(wxID_OK);
}
