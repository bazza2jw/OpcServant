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
#ifndef BME280_H
#define BME280_H
#include <Common/plugin.h>
#include <Common/objectmanager.h>
namespace MRL
{
/*!
     * \brief The BME280 class
     */
    class  BME280 : public Plugin
    {
    public:
        BME280();
        ~BME280();
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
#endif // BME280_H
