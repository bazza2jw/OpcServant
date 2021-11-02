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
#include "SystemPropertiesDialog.h"
#include <Common/common.h>
#include <Common/Daq/daq.h>
#include <Common/Opc/opccommon.h>
#include <Common/Daq/localdb.h>
/*!
    \brief SystemPropertiesDialog::SystemPropertiesDialog
    \param parent
*/
SystemPropertiesDialog::SystemPropertiesDialog(wxWindow *parent)
    : SystemPropertiesDialogBase(parent) {
    MRL::PropertyPath p;
    p.push_back("System");
    //
    m_siteName->SetValue(MRL::SETTINGS().getValue<std::string>(p, "SiteName"));
    m_sitePassword->SetValue(MRL::SETTINGS().getValue<std::string>(p, "SitePassword"));
    m_enableGui->SetValue(MRL::SETTINGS().getValue<bool>(p, "EnableGui"));
    m_enableOpc->SetValue(MRL::SETTINGS().getValue<bool>(p, "EnableOpc"));
    m_enableWeb->SetValue(MRL::SETTINGS().getValue<bool>(p, "EnableWeb"));
    m_enableVK->SetValue(MRL::SETTINGS().getValue<bool>(p, "EnableVK"));

    m_mainTab->SetValue(MRL::SETTINGS().getValue<std::string>(p, "MainTab"));
    m_opcPort->SetValue(MRL::SETTINGS().getValue<std::string>(p, "OpcPort"));
    m_opcNamespace->SetValue(MRL::SETTINGS().getValue<std::string>(p, "OpcNamespace"));
    m_loginRequired->SetValue(MRL::SETTINGS().getValue<bool>(p, "LoginRequired"));
    m_opcUsername->SetValue(MRL::SETTINGS().getValue<std::string>(p, "OpcUsername"));
    m_opcPassword->SetValue(MRL::SETTINGS().getValue<std::string>(p, "OpcPassword"));
    m_autoPurge->SetValue(MRL::SETTINGS().getValue<bool>(p, "AutoPurge"));

    SetSize(0,0,500,400);

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
    MRL::SETTINGS().setValue(p, "SiteName", m_siteName->GetValueAsString().ToStdString());
    MRL::SETTINGS().setValue(p, "SitePassword", m_sitePassword->GetValueAsString().ToStdString());
    //
    MRL::SETTINGS().setValue(p, "EnableGui", m_enableGui->GetValue().GetBool());
    MRL::SETTINGS().setValue(p, "EnableOpc", m_enableOpc->GetValue().GetBool());
    MRL::SETTINGS().setValue(p, "EnableWeb", m_enableWeb->GetValue().GetBool());
    MRL::SETTINGS().setValue(p, "EnableVK", m_enableVK->GetValue().GetBool());
    MRL::SETTINGS().setValue(p, "AutoPurge", m_autoPurge->GetValue().GetBool());


    //
    MRL::SETTINGS().setValue(p, "MainTab", m_mainTab->GetValueAsString().ToStdString());
    MRL::SETTINGS().setValue(p, "OpcPort", m_opcPort->GetValueAsString().ToStdString());
    MRL::SETTINGS().setValue(p, "OpcNamespace", m_opcNamespace->GetValueAsString().ToStdString());
    MRL::SETTINGS().setValue(p, "LoginRequired", m_loginRequired->GetValue().GetBool());
    MRL::SETTINGS().setValue(p, "OpcUsername", m_opcUsername->GetValueAsString().ToStdString());
    MRL::SETTINGS().setValue(p, "OpcPassword", m_opcPassword->GetValueAsString().ToStdString());
    //
    MRL::SETTINGS().save(MRL::Common::instance()->configFileName());
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

}
