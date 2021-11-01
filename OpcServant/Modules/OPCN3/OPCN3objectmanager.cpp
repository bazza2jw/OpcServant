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
#include "OPCN3objectmanager.h"
#include "OPCN3webpanel.h"
#include "OPCN3Panel.h"


/*!
 * \brief MRL::OPCN3ObjectManager::createTabWindow
 * \param parent
 * \param id
 * \return
 */
wxWindow * MRL::OPCN3ObjectManager::createTabWindow(wxWindow *parent, unsigned id)
{
    return new OPCN3Panel(parent,id);

}
/*!
 * \brief MRL::OPCN3ObjectManager::createTabWindow
 */
void MRL::OPCN3ObjectManager::createTabWindow(Wt::WTabWidget *tab,unsigned id)
{
    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
    if (r) {
        tab->addTab(std::make_unique<MRL::OPCN3WebPanel>(id),r->path().back(), Wt::ContentLoading::Eager);
    }

}
/*!
 * \brief MRL::OPCN3ObjectManager::createWebWindow
 * \return
 */
Wt::WContainerWidget * MRL::OPCN3ObjectManager::createWebWindow( Wt::WContainerWidget *root, unsigned id)
{
    return root->addWidget(std::make_unique<OPCN3WebPanel>(id));
}



