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
#include "AlarmConfigurationDialog.h"
#include <MrlLib/mrllib.h>
#include <Common/Daq/daqcommon.h>
#include "alarmobjectmanager.h"

/*!
    \brief AlarmConfigurationDialog::AlarmConfigurationDialog
    \param parent
    \param path
*/
AlarmConfigurationDialog::AlarmConfigurationDialog(wxWindow *parent, const MRL::PropertyPath &path)
    : AlarmConfigurationDialogBase(parent), _path(path) {



    for (auto i = MRL::Common::daq().objects().begin(); i != MRL::Common::daq().objects().end(); i++) {
        MRL::RtObjectRef &r = i->second;
        if (r) {
            if (r->hasOutputs()) {
                std::string s;
                r->path().toString(s);
                GetActionObject()->AppendString(s); // add the
                GetAlertObject()->AppendString(s); // add the
                GetOkObject()->AppendString(s); // add the
            }
        }
    }

    MRL::StringList valueList; // list of Value objects - TO DO other value types - maybe a isValue() flag is needed
    auto o = MRL::ObjectManager::find("Value");
    GetGroupList()->Clear();
    if (o) {
        //
        for (auto i = MRL::Common::daq().objects().begin(); i != MRL::Common::daq().objects().end(); i++) {
            MRL::RtObjectRef &r = i->second;
            if (r) {
                std::string s;
                r->path().toString(s);
                if (MRL::Common::configuration().typeId(r->path()) == o->type()) {
                    valueList.push_back(s);
                }
            }
        }
    }




    Json::Value v;
    if (MRL::Common::configuration().getData(path, v)) {
        //
        // get the selection list as object ids
        std::string sa = v["Selection"].asString();
        wxArrayString sl =  wxStringTokenize(sa, ",");
        std::vector<unsigned> si(sl.Count() + 1);
        for (size_t i = 0; i < sl.Count(); i++) {
            unsigned id = 0;
            sl[i].ToULong((unsigned long *)&id);
            si[i] = id;
        }
        //
        GetGroupList()->Clear();
        int n = 0;
        for (auto k = valueList.begin(); k != valueList.end(); k++,n++) {
            MRL::PropertyPath p;
            p.toList(*k);
            GetGroupList()->AppendString(*k);
            unsigned id = MRL::Common::configuration().find(p);
            for (size_t j = 0; j < sl.Count(); j++) { // is it in the checked list
                if (id == si[j]) {
                    GetGroupList()->Check(n);
                    break;
                }
            }
        }
        //
        GetActionObject()->SetSelection(0); // add the
        GetAlertObject()->SetSelection(0); // add the
        GetOkObject()->SetSelection(0); // add the
        //
        // get the configuration from database
        // do not always have to convert to tree - esp if simple property list
        Json::Value &mi = v["MeasureInterval"];
        Json::Value &pi = v["PublishInterval"];
        Json::Value &ei = v["Enabled"];
        //
        GetMeasureInterval()->SetValue(mi.asInt());
        GetPublishInterval()->SetValue(pi.asInt());
        GetEnable()->SetValue(ei.asBool());
        //
        MRL::SetChoice(GetActionObject(), v["ActionObject"].asString());
        MRL::SetChoice(GetAlertObject(), v["AlertObject"].asString());
        MRL::SetChoice(GetOkObject(), v["OkObject"].asString());
    }
    //
    setOutputs(MRL::GetChoice(GetActionObject()), GetActionOutput());
    setOutputs(MRL::GetChoice(GetAlertObject()), GetAlertOutput());
    setOutputs(MRL::GetChoice(GetOkObject()), GetOkOutput());
    //
    MRL::SetChoice(GetActionOutput(),v["ActionOutput"].asString());
    MRL::SetChoice(GetAlertOutput(),v["AlertOutput"].asString());
    MRL::SetChoice(GetOkOutput(),v["OkOutput"].asString());
    //
}
/*!
    \brief AlarmConfigurationDialog::~AlarmConfigurationDialog
*/
AlarmConfigurationDialog::~AlarmConfigurationDialog() {
}
/*!
    \brief AlarmConfigurationDialog::OnOk
    \param event
*/
void AlarmConfigurationDialog::OnOk(wxCommandEvent &/*event*/) {
    Json::Value v;
    v["MeasureInterval"] = GetMeasureInterval()->GetValue();
    v["PublishInterval"] = GetPublishInterval()->GetValue();
    v["Enabled"] = GetEnable()->GetValue();
    MRL::Common::configuration().updateData(_path, v);
    //
    v["ActionObject"] = MRL::GetChoice(GetActionObject());
    v["ActionOutput"] = MRL::GetChoice(GetActionOutput());
    v["AlertObject"] = MRL::GetChoice(GetAlertObject());
    v["AlertOutput"] = MRL::GetChoice(GetAlertOutput());
    v["OkObject"] = MRL::GetChoice(GetOkObject());
    v["OkOutput"] = MRL::GetChoice(GetOkOutput());
    //
    // get the selected items
    wxArrayInt checkedItems;
    unsigned ns = GetGroupList()->GetCheckedItems(checkedItems);
    if (ns > 0) {
        std::stringstream ss;

        // set the selection array
        for (unsigned i = 0; i < ns; i++) {
            std::string s = GetGroupList()->GetString(checkedItems[i]).ToStdString();
            // find the object id
            MRL::PropertyPath p;
            p.toList(s);
            unsigned id = MRL::Common::configuration().find(p);
            if(id > 0)
            {
                ss << id << ",";
            }
        }
        v["Selection"] = ss.str();
    }
    else {
        v["Selection"] = std::string("");
    }
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
/*!
    \brief AlarmConfigurationDialog::OnActionObject
    \param event
*/
void AlarmConfigurationDialog::OnActionObject(wxCommandEvent &/*event*/) {
    setOutputs(MRL::GetChoice(GetActionObject()), GetActionOutput());
}
/*!
    \brief AlarmConfigurationDialog::OnAlertObject
    \param event
*/
void AlarmConfigurationDialog::OnAlertObject(wxCommandEvent &/*event*/) {
    setOutputs(MRL::GetChoice(GetAlertObject()), GetAlertOutput());
}
/*!
    \brief AlarmConfigurationDialog::OnOkObject
    \param event
*/
void AlarmConfigurationDialog::OnOkObject(wxCommandEvent &/*event*/) {
    setOutputs(MRL::GetChoice(GetOkObject()), GetOkOutput());
}
/*!
    \brief AlarmConfigurationDialog::setOutputs
    \param s
    \param c
*/
void AlarmConfigurationDialog::setOutputs(const std::string &s, wxChoice *c) {
    MRL::PropertyPath p;
    p.toList(s); // convert to rpath
    // find object from path
    unsigned id = MRL::Common::configuration().find(p); // find the object id
    // locate the object
    MRL::RtObjectRef &r = MRL::Common::daq().objects()[id];
    // subtle consideration - an RT objects input names are not necessaraly fixed to object type
    c->Clear();
    if (r) {
        MRL::StringVector &in = r->outputs();
        for (size_t i = 0; i < in.size(); i++) {
            c->AppendString(in[i]);
        }
        c->SetSelection(0);
    }
}


