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
#ifndef OPCN3PLUGIN_H
#define OPCN3PLUGIN_H

#include <Common/plugin.h>
#include <Common/objectmanager.h>
namespace MRL
{
/*!
 * \brief The OPCN3Plugin class
 */
class OPCN3Plugin : public MRL::Plugin
{
public:
    OPCN3Plugin();
    ~OPCN3Plugin();
    //
    // Global initialise
    virtual void initialise();
    //
    // Initialise the GUI
    virtual void initialiseGui();
    // Initialise OPC
    virtual void initialiseOpc();
    // Initialise Web
    virtual void initialiseWeb();
    // Clean up
    virtual void terminate();
};
}

#endif // OPCN3PLUGIN_H
