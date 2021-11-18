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
#include "bwebuimain.h"
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WLabel.h>
#include <Wt/WMessageBox.h>
#include "bwebnavigationtree.h"
#include "websystempropertiesdialog.h"
#include <Common/Web/webdialog.h>
#include <Common/Daq/daq.h>
#include <Common/Daq/daqcommon.h>
#include <Common/messageids.h>
#include <Wt/WFitLayout.h>
#include <Common/Web/webaliasconfigurationdialog.h>
#include <Common/Web/webuserconfigurationdialog.h>
/*!
    \brief BWebUiMain::BWebUiMain
    \param parent
*/
MRL::BWebUiMain::BWebUiMain() {
    //
    auto layout = std::make_unique<Wt::WVBoxLayout>();
    //
    _navigationBar =  layout->addWidget(std::make_unique<Wt::WNavigationBar>());
    //
    _navigationBar->setTitle(_TR("OPC Servant"));
    auto fitlayout = layout->addLayout(std::make_unique<Wt::WFitLayout>(),5);
    //
    // Add menu
    auto m = std::make_unique<Wt::WMenu>();
    //
    // Add menu items and connect signals
    //
    _logout = m->addItem(_TR("Logout"));
    _properties = m->addItem(_TR("System Properties"));
    _alias = m->addItem(_TR("Alias Setup"));
    _user = m->addItem(_TR("User Configuration"));

    _navigationBar->addMenu(std::move(m));
    //
    // add the tab widget
    auto st = fitlayout->addWidget(std::make_unique<Wt::WTabWidget>());
    //
    // Navigator tab
    //
    std::unique_ptr<BWebNavigationTree> bt((_tree = new BWebNavigationTree()));
    //
    // Create the tabs
    //auto cp = st->addTab(std::make_unique<Wt::WText>(), _TR("Configuration"));
    //auto mp = st->addTab(std::make_unique<Wt::WText>(), _TR("Measure"));
    //auto pp = st->addTab(std::make_unique<Wt::WText>(), _TR("Publish"));
    //
    //bt->setConfigurationPane(static_cast<Wt::WText *>(cp->contents()));
    //bt->setMeasurePane(static_cast<Wt::WText *>(mp->contents()));
    //bt->setPublishPane(static_cast<Wt::WText *>(pp->contents()));
    //
    Wt::WLength wd(100,Wt::LengthUnit::Percentage);
    bt->resize(wd,wd);
    st->addTab(std::move(bt), _TR("Navigator"));
    // add a status bar
    //
    auto toolb = layout->addLayout(std::make_unique<Wt::WHBoxLayout>(), 1);
    _editItem = toolb->addWidget(std::make_unique<Wt::WPushButton>(_TR("Options...")), 1);
    _restartButton = toolb->addWidget(std::make_unique<Wt::WPushButton>(_TR("Restart")), 1);
    _startButton = toolb->addWidget(std::make_unique<Wt::WPushButton>(_TR("Start")), 1);
    _stopButton = toolb->addWidget(std::make_unique<Wt::WPushButton>(_TR("Stop")), 1);
    auto sb =   toolb->addWidget(std::make_unique<Wt::WLabel>(), 8);
    sb->setText(_TR("Status OK"));
    //
    _logout->triggered().connect(this, &MRL::BWebUiMain::onLogout);
    _properties->triggered().connect(this, &MRL::BWebUiMain::onProperties);
    _alias->triggered().connect(this, &MRL::BWebUiMain::onAliasSetup);
    _editItem->clicked().connect(this, &MRL::BWebUiMain::onEditItem);
    _restartButton->clicked().connect(this, &MRL::BWebUiMain::onRestart);
    _startButton->clicked().connect(this, &MRL::BWebUiMain::onStart);
    _stopButton->clicked().connect(this, &MRL::BWebUiMain::onStop);
    _user->triggered().connect(this, &MRL::BWebUiMain::onUser);
    //
    setLayout(std::move(layout));
}

/*!
 * \brief MRL::BWebUiMain::onAliasSetup
 */
void MRL::BWebUiMain::onAliasSetup()
{
    WebDialogBase::showDialog<MRL::WebAliasConfigurationDialog>(this);
}

/*!
    \brief MRL::BWebUiMain::onRestart
*/
void MRL::BWebUiMain::onRestart() {
    MRL::Daq::instance()->post(MESSAGEID::Daq_Restart);
}
/*!
    \brief MRL::BWebUiMain::onStart
*/
void MRL::BWebUiMain::onStart() {
    MRL::Daq::instance()->post(MESSAGEID::Daq_Start);
}
/*!
    \brief MRL::BWebUiMain::onStop
*/
void MRL::BWebUiMain::onStop() {
    MRL::Daq::instance()->post(MESSAGEID::Daq_Stop);
}



/*!
    \brief MRL::BWebUiMain::onLogout
*/
void MRL::BWebUiMain::onLogout() {
    Wt::StandardButton result = Wt::WMessageBox::show(_TR("Confirm"), _TR("Logout?"),  Wt::StandardButton::Ok | Wt::StandardButton::Cancel);
    if (result == Wt::StandardButton::Ok) {
        // Do logout
        Wt::WApplication::instance()->quit();
    }
}

/*!
    \brief MRL::BWebUiMain::onProperties
*/
void MRL::BWebUiMain::onProperties() {
    WebDialogBase::showDialog<WebSystemPropertiesDialog>(this);
}

/*!
    \brief MRL::BWebUiMain::onEditItem
*/
void MRL::BWebUiMain::onEditItem() {
    if (_tree) _tree->doMenu();
    _editItem->setFocus(false);
}

void MRL::BWebUiMain::onUser()
{
    WebDialogBase::showDialog<WebUserConfigurationDialog>(this);

}
