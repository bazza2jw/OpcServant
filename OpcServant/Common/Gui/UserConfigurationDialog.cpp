/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "UserConfigurationDialog.h"
#include <Common/common.h>

/*!
 * \brief UserConfigurationDialog::UserConfigurationDialog
 * \param parent
 */
UserConfigurationDialog::UserConfigurationDialog(wxWindow* parent,MRL::VariantPropertyTree &s)
    : UserConfigurationDialogBase(parent),_settings(s)
{
    wxArrayString  l;
    _settings.list("/Users",l);
    if(l.Count() > 0)
    {
        GetListUsers()->Append(l);
        GetListUsers()->SetSelection(0);
    }
    setup();
}

/*!
 * \brief UserConfigurationDialog::setup
 */
void UserConfigurationDialog::setup()
{

    wxString u = GetListUsers()->GetStringSelection();
    //
    // Look up the entry
    MRL::PropertyPath p;
    p.push_back("Users");
    p.push_back(u.ToStdString());
    //
    std::string pwd = _settings.getValue<std::string>(p,"Password");
    GetPassword()->SetValue(pwd);
    GetEnabled()->SetValue(_settings.getValue<bool>(p,"Enabled"));
    GetAdmin()->SetValue(_settings.getValue<bool>(p,"Admin"));

}

/*!
 * \brief UserConfigurationDialog::~UserConfigurationDialog
 */
UserConfigurationDialog::~UserConfigurationDialog()
{

}
/*!
 * \brief UserConfigurationDialog::onApply
 * \param event
 */
void UserConfigurationDialog::onApply(wxCommandEvent& /*event*/)
{
    if(GetListUsers()->GetCount() > 0)
    {
        wxString u = GetListUsers()->GetStringSelection();
        //
        MRL::PropertyPath p;
        p.push_back("Users");
        p.push_back(u.ToStdString());
        //
        _settings.setValue(p,"Password",GetPassword()->GetValue().ToStdString());
        _settings.setValue(p,"Enabled",GetEnabled()->GetValue());
        _settings.setValue(p,"Admin",GetEnabled()->GetValue());
        //
    }
}
/*!
 * \brief UserConfigurationDialog::onDelete
 * \param event
 */
void UserConfigurationDialog::onDelete(wxCommandEvent& /*event*/)
{
    if(GetListUsers()->GetCount() > 0)
    {
        wxString u = GetListUsers()->GetStringSelection();
        int n  =         GetListUsers()->GetSelection();
        //
        GetListUsers()->Delete(n);
        GetListUsers()->SetSelection(0);
        //
        MRL::PropertyPath p;
        p.push_back("Users");
        p.push_back(u.ToStdString());
        _settings.remove(p);
    }
}
/*!
 * \brief UserConfigurationDialog::onNew
 * \param event
 */
void UserConfigurationDialog::onNew(wxCommandEvent& event)
{
  wxTextEntryDialog dlg(this,_("New User Name"),_("New User"));
  if(dlg.ShowModal() == wxID_OK)
  {
      wxString u = dlg.GetValue();
      if(GetListUsers()->FindString(u) == wxNOT_FOUND)
      {
          GetListUsers()->AppendString(u);
          GetEnabled()->SetValue(false);
          GetAdmin()->SetValue(false);
          GetPassword()->SetValue("*******");
          GetListUsers()->SetSelection(GetListUsers()->FindString(u));
          onApply(event);
      }
  }
}

/*!
 * \brief UserConfigurationDialog::onSelection
 * \param event
 */
void UserConfigurationDialog::onSelection(wxCommandEvent& event)
{
    setup();
}
/*!
 * \brief UserConfigurationDialog::onOk
 * \param event
 */
void UserConfigurationDialog::onOk(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}
