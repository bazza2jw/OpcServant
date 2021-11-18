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
#include "ValueConfigurationDialog.h"
#include "ValueSelectAliasDialog.h"
#include <Common/Daq/daqcommon.h>
#include <Common/Daq/rtobject.h>

/*!
    \brief ValueConfigurationDialog::ValueConfigurationDialog
    \param parent
*/
ValueConfigurationDialog::ValueConfigurationDialog(wxWindow *parent, const MRL::PropertyPath &path, MRL::VariantPropertyTree &c)
    : ValueConfigurationDialogBase(parent),_path(path),_configuration(c) {
    //
    // Get the alias list - the alias list maps to object / inputs
    MRL::StringMap &a = MRL::Common::aliasMap();
    for(auto i = a.begin(); i != a.end(); i++)
    {
        wxString s(i->first);
        GetObject()->Append(s); // add to list
    }
    //
    MRL::SetChoice(GetObject(), _configuration.getValue<std::string>(_path,"Object"));
    GetScale()->SetValue(_configuration.getValue<double>(_path,"Scale"));
    GetOffset()->SetValue(_configuration.getValue<double>(_path,"Offset"));
    GetHihi()->SetValue(_configuration.getValue<double>(_path,"HiHi"));
    GetHilo()->SetValue(_configuration.getValue<double>(_path,"HiLo"));
    GetLohi()->SetValue(_configuration.getValue<double>(_path,"LoHi"));
    GetLolo()->SetValue(_configuration.getValue<double>(_path,"LoLo"));
    GetHihiEnabled()->SetValue(_configuration.getValue<bool>(_path,"HiHiEnabled"));
    GetHiloEnabled()->SetValue(_configuration.getValue<bool>(_path,"HiLoEnabled"));
    GetLohiEnabled()->SetValue(_configuration.getValue<bool>(_path,"LoHiEnabled"));
    GetLoloEnabled()->SetValue(_configuration.getValue<bool>(_path,"LoLoEnabled"));
    //
}

/*!
    \brief ValueConfigurationDialog::~ValueConfigurationDialog
*/
ValueConfigurationDialog::~ValueConfigurationDialog() {
}
/*!
    \brief ValueConfigurationDialog::OnOk
    \param event
*/
void ValueConfigurationDialog::OnOk(wxCommandEvent &/*event*/) {
    //
    _configuration.setValue(_path,"Object",MRL::GetChoice(GetObject()));
    _configuration.setValue(_path,"HiHi",GetHihi()->GetValue());
    _configuration.setValue(_path,"HiLo",GetHilo()->GetValue());
    _configuration.setValue(_path,"LoHi",GetLohi()->GetValue());
    _configuration.setValue(_path,"LoLo",GetLolo()->GetValue());
    _configuration.setValue(_path,"HiHiEnabled", GetHihiEnabled()->GetValue());
    _configuration.setValue(_path,"HiLoEnabled", GetHiloEnabled()->GetValue());
    _configuration.setValue(_path,"LoHiEnabled", GetLohiEnabled()->GetValue());
    _configuration.setValue(_path,"LoLoEnabled", GetLoloEnabled()->GetValue());
    _configuration.setValue(_path,"Scale",GetScale()->GetValue());
    _configuration.setValue(_path,"Offset",GetOffset()->GetValue());

    //
    //
    EndModal(wxID_OK);
}
/*!
 * \brief ValueConfigurationDialog::onOk
 * \param event
 */
void ValueConfigurationDialog::onOk(wxCommandEvent& event)
{
    OnOk(event);
}
