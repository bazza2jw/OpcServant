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
#include "buiwebapplication.h"
#include "bwebuimain.h"
#include <Wt/WEnvironment.h>
#include "bwebadminpane.h"
#include "buiwebuserpage.h"
#include <Common/Web/webreportpane.h>
#include <Common/Web/webdialog.h>
#include <Common/Web/reportselectweb.h>
#include <Common/Daq/daqcommon.h>
#include <Common/objectmanager.h>
#include <Common/Web/weblogindialog.h>
#include <Common/Web/weblogindialog.h>
/*!
   * \brief MRL::BuiWebApplication::BuiWebApplication
   * \param env
   */
MRL::BuiWebApplication::BuiWebApplication(UI_TYPE t, const Wt::WEnvironment& env) : Wt::WApplication(env), _type(t)
{
    setTitle(_TR("OPC Servant User Interface"));
    // Get the boot strap theme for the navigation bar
    _theme = std::make_shared<Wt::WBootstrapTheme>();
    _theme->setVersion(Wt::BootstrapVersion::v3);
    setTheme(_theme);
    //
    // Set up the model for the configuration
    const std::string * u = env.getParameter("user");
    const std::string * tok = env.getParameter("token");
    if(u && tok)
    {
        MRL::PropertyPath p;
        p.push_back("LOGIN");
        p.push_back(*u);
        if(RUNTIME().exists(p))
        {
            std::string rt = MRL::RUNTIME().getValue<std::string>(p);
            if(rt == *tok)
            {
                //
                switch(_type)
                {
                case UI_ADMIN:
                {
                    _topContainer = root()->addWidget(std::make_unique<MRL::BWebUiMain>());
                }
                break;
                case UI_USER:
                {
//                    const std::string * p = env.getParameter("id");
//                    if(p)
//                    {
//                        // get the ID
//                        unsigned id = stringToNumber<unsigned>(*p);
//                        MRL::ObjectManager *o = MRL::ObjectManager::findByObjectId(id);
//                        if(o)
//                        {
//                            _topContainer = o->createWebWindow(root(),id);
//                        }
//                    }
//                    else
//                    {
                        // select and make report only
                        _topContainer = root()->addWidget(std::make_unique<MRL::BuiWebUserPage>());
//                    }
                }
                break;
                case UI_REPORT:
                {
                    // get the parameter
//                    const std::string * p = env.getParameter("admin");
//                    if(p)
//                    {
                        _topContainer = root()->addWidget(std::make_unique<MRL::WebReportPane>());
//                    }
//                    else
//                    {
//                        // select and make report only
//                        _topContainer = root()->addWidget(std::make_unique<MRL::ReportSelectWeb>());
//                    }
                }
                break;
                case UI_DIAGNOSTIC:
                    break;
                default:
                    break;
                }
                //
                Wt::WLength wd(100,Wt::LengthUnit::Percentage);
                if(_topContainer) _topContainer->resize(wd,wd);
                //
            }
        }
    }
    else
    {
        Wt::WLength wd(30,Wt::LengthUnit::FontEm);
        Wt::WLength hd(10,Wt::LengthUnit::FontEm);
        MRL::WebLoginDialog * p = root()->addWidget(std::make_unique<MRL::WebLoginDialog>());
        p->resize(wd,hd);
    }
}

