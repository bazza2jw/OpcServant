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
#include "ValueGroupConfigurationDialog.h"
#include "ValueConfigurationDialog.h"
#include <Common/Gui/AliasConfigurationDialog.h>
#include <MrlLib/mrllib.h>
#include <Common/Daq/daqcommon.h>


/*!
 * \brief ValueGroupConfigurationDialog::ValueGroupConfigurationDialog
 * \param parent
 * \param path
 */
ValueGroupConfigurationDialog::ValueGroupConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : ValueGroupConfigurationDialogBase(parent),_path(path)
{
    // get the list of configured values - these are aliases
    // A value has alarm / warning limits
    Json::Value v;
    if (MRL::Common::configuration().getData(path, v)) {
        _configuration.fromJson(v); // load configuration
        //
        MRL::StringList l ;
        _configuration.listChildren("Values",l);
        // Populate the list box
        for(auto i = l.begin(); i != l.end(); i++)
        {
            GetListValues()->Append(*i);
        }

        if(l.size() > 0)
        {
            GetListValues()->SetSelection(0);
        }
        //
        GetOkOutput()->AppendString("None"); // add the
        GetAlertOutput()->AppendString("None"); // add the
        GetActionOutput()->AppendString("None"); // add the
        GetFailureOutput()->AppendString("None"); // add the
        //
        // build the output list
        //
        for (auto i = MRL::Common::daq().objects().begin(); i != MRL::Common::daq().objects().end(); i++) {
            MRL::RtObjectRef &r = i->second;
            if (r) {
                if (r->hasOutputs()) {
                    std::string pt;
                    r->path().toString(pt);
                    MRL::StringVector &l = r->outputs();
                    for(int j = 0; j < l.size(); j++)
                    {
                        std::string s = pt + TAG_SEPERATOR + l[j];
                        GetOkOutput()->AppendString(s); // add the
                        GetAlertOutput()->AppendString(s); // add the
                        GetActionOutput()->AppendString(s); // add the
                        GetFailureOutput()->AppendString(s); // add the
                    }
                }
            }
        }
        //
        MRL::SetChoice(GetOkOutput(), _configuration.getString("/OkOutput") );
        MRL::SetChoice(GetAlertOutput(), _configuration.getString("/AlertOutput") );
        MRL::SetChoice(GetActionOutput(), _configuration.getString("/ActionOutput") );
        MRL::SetChoice(GetFailureOutput(), _configuration.getString("/FailureOutput") );
        //

        GetMeasureInterval()->SetValue(_configuration.getInt("/MeasureInterval"));
        GetPublishInterval()->SetValue(_configuration.getInt("/PublishInterval"));
        GetEnabled()->SetValue(_configuration.getBool("/Enabled"));
        GetEnableTabView()->SetValue(_configuration.getBool("/EnableTabView"));
    }
}
/*!
 * \brief ValueGroupConfigurationDialog::~ValueGroupConfigurationDialog
 */
ValueGroupConfigurationDialog::~ValueGroupConfigurationDialog()
{
}
/*!
 * \brief ValueGroupConfigurationDialog::onAdd
 * \param event
 */
void ValueGroupConfigurationDialog::onAdd(wxCommandEvent& event)
{
    // get the name
    wxTextEntryDialog dlg(this,"Enter New Value Name","New Value Name");
    if(dlg.ShowModal() == wxID_OK)
    {
        wxString s = dlg.GetValue();
        if(GetListValues()->FindString(s,true) < 0)
        {
            // add to list
            GetListValues()->Append(s);
            GetListValues()->SetSelection(GetListValues()->FindString(s,true));
            // add to config
            MRL::PropertyPath p;
            p.push_back("Values");
            p.push_back(s.ToStdString());
            _configuration.setValue("HiHiEnabled",false);
            _configuration.setValue("HiLoEnabled",false);
            _configuration.setValue("LoHiEnabled",false);
            _configuration.setValue("LoLoEnabled",false);
            _configuration.setValue("HiHi",0.0);
            _configuration.setValue("HiLo",0.0);
            _configuration.setValue("LoHi",0.0);
            _configuration.setValue("LoLo",0.0);
            _configuration.setValue("Scale",1.0);
            _configuration.setValue("Offset",0.0);
            _configuration.setValue("Object",std::string("None"));

            // open the editor
            onEdit(event);
        }
        else
        {
            wxMessageBox("Duplicate Value");
        }
    }

}
/*!
 * \brief ValueGroupConfigurationDialog::onEdit
 * \param event
 */
void ValueGroupConfigurationDialog::onEdit(wxCommandEvent& event)
{
    // get current name
    std::string name = GetListValues()->GetStringSelection().ToStdString();
    MRL::PropertyPath p;
    p.push_back("Values");
    p.push_back(name);
    ValueConfigurationDialog dlg(this,p,_configuration);
    dlg.ShowModal();
}
/*!
 * \brief ValueGroupConfigurationDialog::onRemove
 */
void ValueGroupConfigurationDialog::onRemove(wxCommandEvent& /*event*/)
{
    // delete from config
    // delete from list
    if(GetListValues()->GetCount() > 0)
    {
        std::string name = GetListValues()->GetStringSelection().ToStdString();
        MRL::PropertyPath p;
        p.push_back("Values");
        p.push_back(name);
        GetListValues()->Delete(GetListValues()->GetSelection());
        _configuration.remove(p);
        if(GetListValues()->GetCount() > 0)GetListValues()->SetSelection(0);
    }
}
/*!
 * \brief ValueGroupConfigurationDialog::onOk
 */
void ValueGroupConfigurationDialog::onOk(wxCommandEvent& /*event*/)
{
    // write the configuration to database
    _configuration.setValue("/Enabled",GetEnabled()->GetValue());
    _configuration.setValue("/MeasureInterval", GetMeasureInterval()->GetValue());
    _configuration.setValue("/PublishInterval",GetPublishInterval()->GetValue());
    _configuration.setBool("/EnableTabView",GetEnableTabView()->GetValue());
    //
    _configuration.setString("/OkOutput",MRL::GetChoice(GetOkOutput()));
    _configuration.setString("/AlertOutput",MRL::GetChoice(GetAlertOutput()));
    _configuration.setString("/ActionOutput",MRL::GetChoice(GetActionOutput()));
    _configuration.setString("/FailureOutput",MRL::GetChoice(GetFailureOutput()));
    //
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
/*!
 * \brief ValueGroupConfigurationDialog::onAlias
 */
void ValueGroupConfigurationDialog::onAlias(wxCommandEvent& /*event*/)
{
    AliasConfigurationDialog dlg(this);
    dlg.ShowModal();
}
