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
#include "valueobjectmanager.h"
#include "valuert.h"
#include "ValueGroupPanel.h"
#include "valuegroupwebpanel.h"


/*!
 * \brief MRL::ValueObjectManager::createTabWindow
 * \param parent
 * \param id
 * \return
 */
wxWindow * MRL::ValueObjectManager::createTabWindow(wxWindow *parent, unsigned id)
{
    return new ValueGroupPanel(parent,id);
}

/*!
 * \brief MRL::ValueObjectManager::createTabWindow
 * \param tab
 * \param id
 */
void MRL::ValueObjectManager::createTabWindow(Wt::WTabWidget *tab,unsigned id)
{
    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
    if (r) {
        tab->addTab(std::make_unique<ValueGroupWebPanel>(id),r->path().back(), Wt::ContentLoading::Eager);
    }
}


/*!
 * \brief MRL::ValueObjectManager::createWebWindow
 * \param root
 * \param id
 * \return widget
 */
Wt::WContainerWidget * MRL::ValueObjectManager::createWebWindow( Wt::WContainerWidget *root, unsigned id)
{
    return root->addWidget(std::make_unique<ValueGroupWebPanel>(id));
}


