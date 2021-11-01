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
#include "buiwebuserpage.h"
#include <Wt/WFitLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTextArea.h>
#include <Common/common.h>
#include <Common/Daq/daq.h>
#include <Common/Daq/rtobject.h>
#include <Common/Daq/daqcommon.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>
/*!
 * \brief MRL::BuiWebUserPage::BuiWebUserPage
 */
MRL::BuiWebUserPage::BuiWebUserPage()
{
     auto boxLayout = std::make_unique<Wt::WVBoxLayout>();
     //
     _toolBar = boxLayout->addWidget(std::make_unique<Wt::WToolBar>()); // add the tool bar
     auto layout = boxLayout->addLayout(std::make_unique<Wt::WFitLayout>(),2);
     //
     // add the tool buttons
     //
     createColorButton("btn-primary", "Options");
     //
     // Now add the tab widget pane
     //
     _tab = layout->addWidget(std::make_unique<Wt::WTabWidget>());
     //
     // Create the panes
     //
     //
     RtObjectMap &rd = MRL::Common::daq().objects();
     for(auto i = rd.begin(); i != rd.end(); i++)
     {
         MRL::RtObjectRef &r = i->second;
         // Scan the objects and find out which ones can provide pages
         if (r) {

             unsigned t = MRL::Common::configuration().typeId(r->path()); // get item type
             auto o = MRL::ObjectManager::find(t);
             if (o) {
                 o->createTabWindow(_tab, i->first);
             }
         }
     }
     _tab->setStyleClass("tabwidget");
     _tab->addTab(std::make_unique<Wt::WTextArea>(__DATE__ " - " __TIME__), _TR("Messages"), Wt::ContentLoading::Eager);
     //
      setLayout(std::move(boxLayout));
}

/*!
 * \brief MRL::BuiWebUserPage::~BuiWebUserPage
 */
MRL::BuiWebUserPage::~BuiWebUserPage()
{

}
