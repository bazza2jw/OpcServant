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
#ifndef MQTT_H
#define MQTT_H

#include "mqtt_global.h"
#include <Common/plugin.h>
#include <Common/objectmanager.h>
namespace MRL
{
/*!
     * \brief The MQTT class
     */
    class  MQTT : public Plugin {
    public:
        MQTT();
        //
        // Global initialise - create the object manager
        virtual void initialise();
        virtual void terminate();
        //
    };
}
#endif // MQTT_H
