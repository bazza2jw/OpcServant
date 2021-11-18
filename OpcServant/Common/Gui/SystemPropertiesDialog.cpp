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
#include "SystemPropertiesDialog.h"
#include <Common/common.h>
#include <Common/Daq/daq.h>
#include <Common/Opc/opccommon.h>
#include <Common/Daq/localdb.h>
#include <Common/Gui/UserConfigurationDialog.h>
/*!
    \brief SystemPropertiesDialog::SystemPropertiesDialog
    \param parent
*/
SystemPropertiesDialog::SystemPropertiesDialog(wxWindow *parent)
    : SystemPropertiesDialogBase(parent) {
    setup();
    SetSize(0,0,500,400);
}

/*!
 * \brief SystemPropertiesDialog::setup
 */
void SystemPropertiesDialog::setup()
{
    _settings.load(MRL::Common::instance()->configFileName());
    MRL::PropertyPath p;
    p.push_back("System");
    //
    m_siteName->SetValue(_settings.getValue<std::string>(p, "SiteName"));
    m_enableGui->SetValue(_settings.getValue<bool>(p, "EnableGui"));
    m_enableOpc->SetValue(_settings.getValue<bool>(p, "EnableOpc"));
    m_enableWeb->SetValue(_settings.getValue<bool>(p, "EnableWeb"));
    m_enableVK->SetValue(_settings.getValue<bool>(p, "EnableVK"));
    m_screenLock->SetValue(_settings.getValue<bool>(p, "ScreenLock"));
    m_screenLockPin->SetValue(_settings.getValue<std::string>(p, "ScreenLockPin"));
    //
    m_mainTab->SetValue(_settings.getValue<std::string>(p, "MainTab"));
    m_opcPort->SetValue(_settings.getValue<std::string>(p, "OpcPort"));
    m_opcNamespace->SetValue(_settings.getValue<std::string>(p, "OpcNamespace"));
    m_loginRequired->SetValue(_settings.getValue<bool>(p, "LoginRequired"));
    m_opcUsername->SetValue(_settings.getValue<std::string>(p, "OpcUsername"));
    m_opcPassword->SetValue(_settings.getValue<std::string>(p, "OpcPassword"));
    m_autoPurge->SetValue(_settings.getValue<bool>(p, "AutoPurge"));

}
/*!
    \brief SystemPropertiesDialog::~SystemPropertiesDialog
*/
SystemPropertiesDialog::~SystemPropertiesDialog() {
}
/*!
    \brief SystemPropertiesDialog::OnOK
    \param event
*/
void SystemPropertiesDialog::OnOK(wxCommandEvent & /*event*/) {
    MRL::PropertyPath p;
    //
    p.push_back("System");
    _settings.setValue(p, "SiteName", m_siteName->GetValueAsString().ToStdString());
    //
    _settings.setValue(p, "EnableGui", m_enableGui->GetValue().GetBool());
    _settings.setValue(p, "EnableOpc", m_enableOpc->GetValue().GetBool());
    _settings.setValue(p, "EnableWeb", m_enableWeb->GetValue().GetBool());
    _settings.setValue(p, "EnableVK", m_enableVK->GetValue().GetBool());
    _settings.setValue(p, "AutoPurge", m_autoPurge->GetValue().GetBool());
    _settings.setValue(p, "ScreenLock", m_screenLock->GetValue().GetBool());
    _settings.setValue(p, "ScreenLockPin", m_screenLockPin->GetValueAsString().ToStdString());
    //
    _settings.setValue(p, "MainTab", m_mainTab->GetValueAsString().ToStdString());
    _settings.setValue(p, "OpcPort", m_opcPort->GetValueAsString().ToStdString());
    _settings.setValue(p, "OpcNamespace", m_opcNamespace->GetValueAsString().ToStdString());
    _settings.setValue(p, "LoginRequired", m_loginRequired->GetValue().GetBool());
    _settings.setValue(p, "OpcUsername", m_opcUsername->GetValueAsString().ToStdString());
    _settings.setValue(p, "OpcPassword", m_opcPassword->GetValueAsString().ToStdString());
    //
    _settings.save(MRL::Common::instance()->configFileName());
    EndModal(wxID_OK);
}

/*!
    \brief SystemPropertiesDialog::onFactoryReset
    \param event
*/
void SystemPropertiesDialog::onFactoryReset(wxCommandEvent &/* event*/) {
    wxMessageDialog dlg(this, _("Reset to FACTORY DEFAULTS - ARE YOU SURE? - CANNOT BE UNDONE"),
                        _("RESET TO FACTORY DEFAULTS"), wxYES_NO);
    // get confirm reset
    if (dlg.ShowModal() == wxID_YES) {

        // clears the config and sets up defaults - this is well hidden
        MRL::Common::opc().server().stop(); // get the OPC to stop
        wxThread::Sleep(1000);
        MRL::Daq::instance()->stop(); // stop data collection
        wxThread::Sleep(1000);
        // reset the database
        MRL::Common::configuration().resetDatabase();
        // clear the object tree
        MRL::Common::display().clearAll();
        //
        wxMessageBox(_("Factory Reset Done - Exit and Restart Required"), _("FACTORY RESET"));
    }
}
/*!
 * \brief SystemPropertiesDialog::onPurge
 * \param event
 */
void SystemPropertiesDialog::onPurge(wxCommandEvent& /*event*/)
{
    // purge all tables to 90 days
    MRL::Daq::instance()->localDb()->purgeAll();
}
/*!
 * \brief SystemPropertiesDialog::onUsers
 */
void SystemPropertiesDialog::onUsers(wxCommandEvent& /*event*/)
{
    UserConfigurationDialog dlg(this,_settings);
    if(dlg.ShowModal() != wxID_OK)
    {
        setup();
    }
}
