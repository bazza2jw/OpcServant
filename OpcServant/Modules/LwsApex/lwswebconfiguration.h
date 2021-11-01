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
#ifndef LWSWEBCONFIGURATION_H
#define LWSWEBCONFIGURATION_H

#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>

namespace MRL
{
/*!
     * \brief The LwsWebConfiguration class
     */
    class LwsWebConfiguration : public WebDialogBase
    {
        PropertyPath _path;
        MRL::VariantPropertyTree _configuration; // tree of configuration values

        Wt::WLineEdit * _address;
        Wt::WSpinBox  * _purgeInterval = nullptr;
        Wt::WSpinBox  * _sampleInterval = nullptr;
        Wt::WCheckBox * _enabled = nullptr;
    public:
        LwsWebConfiguration(const PropertyPath &path) : _path(path){}
        void setup();
        void onOk();

    };

}
#endif // LWSWEBCONFIGURATION_H
