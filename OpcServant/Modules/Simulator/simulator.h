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
#ifndef SIMULATOR_H
#define SIMULATOR_H
#include <Common/plugin.h>
#include <Common/objectmanager.h>
#include "simulator_global.h"
namespace MRL
{
/*!
     * \brief The Simulator class
     */
    class  Simulator : public Plugin
    {
    public:
        Simulator();
        ~Simulator();
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
#endif // SIMULATOR_H
