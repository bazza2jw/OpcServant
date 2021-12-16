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
#include "webuserconfigurationdialog.h"

/*!
 * \brief MRL::WebUserConfigurationDialog::WebUserConfigurationDialog
 */
MRL::WebUserConfigurationDialog::WebUserConfigurationDialog()
{
    _settings.load(MRL::Common::instance()->configFileName());
}
/*!
 * \brief MRL::WebUserConfigurationDialog::setup
 */
void MRL::WebUserConfigurationDialog::setup()
{
    MRL::WebDialogBase::setup();
    auto boxLayout = std::make_unique<Wt::WVBoxLayout>();
    _toolBar = boxLayout->addWidget(std::make_unique<Wt::WToolBar>()); // add the tool bar
    auto layout = boxLayout->addLayout(std::make_unique<Wt::WGridLayout>());
    //
    // add the buttons to the tool bar
    //
    (_add = createColorButton(_toolBar, "btn-primary", _TR("Add")))->clicked().connect(this,&MRL::WebUserConfigurationDialog::add);
    (_edit = createColorButton(_toolBar, "btn-primary", _TR("Apply")))->clicked().connect(this,&MRL::WebUserConfigurationDialog::apply);
    (_remove = createColorButton(_toolBar, "btn-primary", _TR("Remove")))->clicked().connect(this,&MRL::WebUserConfigurationDialog::remove);
    //
    // Add the list of aliases
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Name")), 0, 0);
    _list = layout->addWidget(std::make_unique<Wt::WComboBox>(),0,1);
    //
    StringList l;
    _settings.listChildren("/Users", l);
    for(auto i = l.begin(); i != l.end(); i++) _list->addItem(*i);
    _list->setCurrentIndex(0);
    _list->activated().connect(this,&MRL::WebUserConfigurationDialog::changed);
    //
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Password")), 1, 0);
    _password = layout->addWidget(std::make_unique<Wt::WLineEdit>(),1,1);
    _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>(_TR("Enabled")),2,0);
    _admin = layout->addWidget(std::make_unique<Wt::WCheckBox>(_TR("Admin")),3,0);
    //
    changed();
    contents()->setLayout(std::move(boxLayout));
}

/*!
 * \brief MRL::WebUserConfigurationDialog::onOk
 */
void MRL::WebUserConfigurationDialog::onOk()
{
    _settings.save(MRL::Common::instance()->configFileName());
}

/*!
 * \brief MRL::WebUserConfigurationDialog::newUser
 */
void MRL::WebUserConfigurationDialog::add()
{
    if(_list->count() > 0)
    {
        Wt::WString s;
        if(WebGetInput::getInput(this,_TR("New User"),s))
        {
            PropertyPath p;
            p.push_back("Users");
            p.push_back(s.toUTF8());
            if(!_settings.exists(p))
            {
                _list->addItem(s);
                _settings.setValue(p,"Password","*********");
                _settings.setValue(p,"Enabled",false);
                _settings.setValue(p,"Admin",false);
                changed();
            }
        }
    }
}
/*!
 * \brief MRL::WebUserConfigurationDialog::apply
 */
void MRL::WebUserConfigurationDialog::apply()
{
    if(_list->count() > 0)
    {
        std::string u = _list->currentText().toUTF8();
        //
        MRL::PropertyPath p;
        p.push_back("Users");
        p.push_back(u);
        //
        _settings.setValue(p,"Password",_password->text().toUTF8());
        _settings.setValue(p,"Enabled",_enabled->isChecked());
        _settings.setValue(p,"Admin",_admin->isChecked());
    }
}
/*!
 * \brief MRL::WebUserConfigurationDialog::deleteUser
 */
void MRL::WebUserConfigurationDialog::remove()
{
    if(_list->count() > 0)
    {
        std::string u = _list->currentText().toUTF8();
        //
        _list->removeItem(_list->currentIndex());
        _list->setCurrentIndex(0);
        //
        MRL::PropertyPath p;
        p.push_back("Users");
        p.push_back(u);
        _settings.remove(p);
    }
}

/*!
 * \brief MRL::WebUserConfigurationDialog::changed
 */
void MRL::WebUserConfigurationDialog::changed()
{
    std::string u  = _list->currentText().toUTF8();
    //
    // Look up the entry
    MRL::PropertyPath p;
    p.push_back("Users");
    p.push_back(u);
    //
    std::string pwd = _settings.getValue<std::string>(p,"Password");
    _password->setText(pwd);
    _enabled->setChecked(_settings.getValue<bool>(p,"Enabled"));
    _admin->setChecked(_settings.getValue<bool>(p,"Admin"));
}
