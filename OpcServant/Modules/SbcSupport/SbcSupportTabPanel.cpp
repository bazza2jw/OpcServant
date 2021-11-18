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
#include "SbcSupportTabPanel.h"
#include "SetClock.h"
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
/*!
    \brief SbcSupportTabPanel::SbcSupportTabPanel
    \param parent
    \param id
*/
SbcSupportTabPanel::SbcSupportTabPanel(wxWindow *parent, unsigned id)
    : SbcSupportTabPanelBase(parent), _objectId(id) {
    //
    // get the configuration
    // enable buttons as per config
    // RTC needs careful handling - the RTC is syncd with NTP
    // if NTP is present usually then changing the RTC through the UI is not needed
    //
}

/*!
    \brief SbcSupportTabPanel::~SbcSupportTabPanel
*/
SbcSupportTabPanel::~SbcSupportTabPanel() {
}

/*!
    \brief SbcSupportTabPanel::onFactoryDefaults
    \param event
*/
void SbcSupportTabPanel::onFactoryDefaults(wxCommandEvent &/*event*/) {
    // get password
    wxPasswordEntryDialog pdlg(this, "Enter Admin Password");
    if (pdlg.ShowModal() == wxID_OK) {
        // check the password
        //if(pdlg.GetValue() == password)
        {
            //
            wxMessageDialog dlg(this, _("Reset to FACTORY DEFAULTS - ARE YOU SURE? - CANNOT BE UNDONE"), _("RESET TO FACTORY DEFAULTS"), wxYES_NO);
            // get confirm reset
            if (dlg.ShowModal() == wxID_YES) {
                //
                // stop everything
                // clear the config database table
                // restart if enabled otherwise exit
                //
            }
        }
    }
}
/*!
    \brief SbcSupportTabPanel::onOpenConsole
    \param event
*/
void SbcSupportTabPanel::onOpenConsole(wxCommandEvent &/*event*/) {
    // open kterm or similar - add to system config
    wxPasswordEntryDialog pdlg(this, "Enter Admin Password");
    if (pdlg.ShowModal() == wxID_OK) {
    }
}
/*!
    \brief SbcSupportTabPanel::onRestart
    \param event
*/
void SbcSupportTabPanel::onRestart(wxCommandEvent &/*event*/) {
    wxPasswordEntryDialog pdlg(this, "Enter Admin Password");
    if (pdlg.ShowModal() == wxID_OK) {
    }
}
/*!
    \brief SbcSupportTabPanel::onSetTime
    \param event
*/
void SbcSupportTabPanel::onSetTime(wxCommandEvent &/*event*/) {
    wxPasswordEntryDialog pdlg(this, "Enter Admin Password");
    if (pdlg.ShowModal() == wxID_OK) {
        // put up set clock dialog - set the hardware clock
        //
        SetClock dlg(this);
        dlg.ShowModal();
    }
}
