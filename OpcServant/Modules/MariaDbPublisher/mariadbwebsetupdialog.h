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
#ifndef MARIADBWEBSETUPDIALOG_H
#define MARIADBWEBSETUPDIALOG_H
//
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>
//
namespace MRL
{
/*!
     * \brief The MariaDbWebSetupDialog class
     */
    class MariaDbWebSetupDialog  : public WebDialogBase
    {
         PropertyPath _path;
         //
         // the controls
         Wt::WLineEdit * _host = nullptr;
         Wt::WLineEdit * _database = nullptr;
         Wt::WSpinBox * _port = nullptr;
         Wt::WLineEdit * _username = nullptr;
         Wt::WLineEdit * _password = nullptr;
         Wt::WCheckBox * _enabled = nullptr;
         //
    public:
        MariaDbWebSetupDialog(const PropertyPath &path) : _path(path){}
        void setup();
        void onOk();
    };
}
#endif // MARIADBWEBSETUPDIALOG_H
