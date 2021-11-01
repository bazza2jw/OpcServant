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
#include "MariaDbSetupDialog.h"
#include "mariadbpublisher.h"
#include <Common/Daq/rtobject.h>
#include <Common/storagedatabase.h>
#include "mysql++/mysql++.h"

/*!
    \brief MariaDbSetupDialog::MariaDbSetupDialog
    \param parent
    \param id
*/
MariaDbSetupDialog::MariaDbSetupDialog(wxWindow *parent, const MRL::PropertyPath &path)
    : MariaDbSetupDialogBase(parent), _path(path) {
    // get the configuration from database
    Json::Value v;
    if (MRL::Common::configuration().getData(path, v)) {
        // do not always have to convert to tree - esp if simple property list
        Json::Value &ht = v["Host"];
        Json::Value &db = v["Database"];
        Json::Value &pt = v["Port"];
        Json::Value &us = v["Username"];
        Json::Value &ps = v["Password"];
        Json::Value &ei = v["Enabled"];
        //
        GetHost()->SetValue(ht.asString());
        GetDatabase()->SetValue(db.asString());
        GetPort()->SetValue(pt.asInt());
        GetUsername()->SetValue(us.asString());
        GetPassword()->SetValue(ps.asString());
        GetEnable()->SetValue(ei.asBool());
        //
    }
}
/*!
    \brief MariaDbSetupDialog::~MariaDbSetupDialog
*/
MariaDbSetupDialog::~MariaDbSetupDialog() {
}
/*!
    \brief MariaDbSetupDialog::OnOK
    \param event
*/
void MariaDbSetupDialog::OnOK(wxCommandEvent &/*event*/) {
    Json::Value v;
    //
    v["Host"] = GetHost()->GetValue().ToStdString();
    v["Database"] = GetDatabase()->GetValue().ToStdString();
    v["Port"] = GetPort()->GetValue();
    v["Username"] = GetUsername()->GetValue().ToStdString();
    v["Password"] = GetPassword()->GetValue().ToStdString();
    v["Enabled"]  = GetEnable()->GetValue();
    //
    MRL::Common::configuration().updateData(_path, v);
    //
    //
    EndModal(wxID_OK);
}
/*!
    \brief MariaDbSetupDialog::OnTest
    \param event
*/
void MariaDbSetupDialog::OnTest(wxCommandEvent & /*event*/) {
    mysqlpp::Connection _db;
    try {

        std::string h = GetHost()->GetValue().ToStdString();
        std::string d = GetDatabase()->GetValue().ToStdString();
        int p = GetPort()->GetValue();
        std::string u = GetUsername()->GetValue().ToStdString();
        std::string pw = GetPassword()->GetValue().ToStdString();

        if (_db.connect(d.c_str(), h.c_str(), u.c_str(), pw.c_str(),p)) {
            wxMessageBox("Connect Success");
            _db.disconnect();
        }
        else {
            wxMessageBox("Connect Fail");
        }
    }
    catch (mysqlpp::Exception e) {
        wxLogDebug("Database Exception in %s: %s", __FUNCTION__, e.what());
         wxMessageBox(e.what());
    }
}
