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
#include "weblogindialog.h"
// use the system password for authentication - simple to manage
#include <pwd.h>
#include <shadow.h>
#include <sys/types.h>
#include <Common/common.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <wx/url.h>

/*!
 * \brief MRL::WebLoginDialog::WebLoginDialog
 */
MRL::WebLoginDialog::WebLoginDialog()
{
    auto boxLayout = std::make_unique<Wt::WVBoxLayout>();
    auto layout =  boxLayout->addLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
    auto buttons = boxLayout->addLayout(std::make_unique<Wt::WHBoxLayout>());
    addStyleClass("form-group");
    //
    Wt::WLength wd(20,Wt::LengthUnit::FontEm);
    Wt::WLength hd(2,Wt::LengthUnit::FontEm);
    Wt::WLength lw(10,Wt::LengthUnit::FontEm);
    //
    Wt::WLabel * l = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Name")), 0, 0);
    l->setMinimumSize(lw,hd);
    _name = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
    _name->setMaximumSize(wd,hd);
    l = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Password")), 1, 0);
    l->setMinimumSize(lw,hd);
    _password = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 1, 1);
    _password->setMaximumSize(wd,hd);
    //
    Wt::WPushButton * ok = buttons->addWidget(std::make_unique<Wt::WPushButton>("OK"));
    Wt::WPushButton * cancel = buttons->addWidget(std::make_unique<Wt::WPushButton>("Cancel"));
    //
    ok->addStyleClass("btn-primary");
    ok->setWidth(lw);
    cancel->addStyleClass("btn-primary");
    cancel->setWidth(lw);

    ok->clicked().connect(this, &MRL::WebLoginDialog::onOk);
    cancel->clicked().connect(this, &MRL::WebLoginDialog::onCancel);
    //
    setLayout(std::move(boxLayout));
}

/*!
 * \brief MRL::WebLoginDialog::onOk
 */
void MRL::WebLoginDialog::onOk()
{
    std::string u = _name->text().toUTF8();
    std::string pw = _password->text().toUTF8();
    //
    _result = Common::checkUser(u,pw);
    if(_result)
    {
        MRL::PropertyPath p;
        p.push_back("LOGIN");
        //
        std::string t = std::to_string(std::time(nullptr));
        MRL::RUNTIME().setValue(p,u,t); // set the login token
        //
        std::string url = Wt::WApplication::instance()->url();
        //
        int n = url.find('?');
        if(n != std::string::npos)
        {
            url = url.substr(0,n +1); // strip out the
        }
        else
        {
            url += "?";
        }
        //
        url += "user=";
        url += u;
        url += "&token=";
        url += t;
        //
        Wt::WApplication::instance()->redirect(url);
    }
    else
    {
        Wt::WApplication::instance()->refresh();
    }
}

/*!
 * \brief MRL::WebLoginDialog::onCancel
 */
void MRL::WebLoginDialog::onCancel()
{
    Wt::WApplication::instance()->quit();
}
