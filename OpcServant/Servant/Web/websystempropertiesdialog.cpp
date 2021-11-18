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
#include "websystempropertiesdialog.h"
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Wt/WGridLayout.h>
#include <Wt/WWidget.h>

/*!
 * \brief WebSystemPropertiesDialog::WebSystemPropertiesDialog
 * \param o
 */
MRL::WebSystemPropertiesDialog::WebSystemPropertiesDialog()
{
}
/*!
 * \brief MRL::WebSystemPropertiesDialog::setup
 */
void MRL::WebSystemPropertiesDialog::setup()
{
    //
    //
    resize(500,400);
    //
    auto layout = std::make_unique<Wt::WGridLayout>();
    layout->addWidget(std::make_unique<Wt::WLabel>("Site Name"),0,0);
    _siteName  = layout->addWidget(std::make_unique<Wt::WLineEdit>("SITE"),0,1);
    _enableGui = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enable GUI"),1,0);
    _enableWeb = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enable Web"),2,0);
    _enableOpc = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enable OPC"),3,0);
    //
    //
    MRL::PropertyPath p;
    p.push_back("System");
    _siteName->setText(MRL::SETTINGS().getValue<std::string>(p,"SiteName"));
    _enableGui->setChecked(MRL::SETTINGS().getValue<bool>(p,"EnableGui"));
    _enableOpc->setChecked(MRL::SETTINGS().getValue<bool>(p,"EnableOpc"));
    _enableWeb->setChecked(MRL::SETTINGS().getValue<bool>(p,"EnableWeb"));
    //
    //
    contents()->setLayout(std::move(layout));
    //
    _siteName->setFocus();
}
/*!
 * \brief WebSystemPropertiesDialog::onOk
 */
void MRL::WebSystemPropertiesDialog::onOk()
{
    MRL::PropertyPath p;
    p.push_back("System");
    MRL::SETTINGS().setValue(p,"SiteName",_siteName->text().toUTF8());
    MRL::SETTINGS().setValue(p,"EnableGui",_enableGui->isChecked());
    MRL::SETTINGS().setValue(p,"EnableOpc",_enableOpc->isChecked());
    MRL::SETTINGS().setValue(p,"EnableWeb",_enableWeb->isChecked());
    //
    MRL::SETTINGS().dump();
    MRL::SETTINGS().save(MRL::Common::instance()->configFileName());
}




