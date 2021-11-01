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
#ifndef ALARMS_H
#define ALARMS_H
#include <Common/plugin.h>
#include "alarms_global.h"
namespace MRL {
/*!
     * \brief The Alarms class
     */
    class  Alarms : public Plugin {
        public:
            Alarms();
            // Global initialise
            virtual void initialise();
            virtual void initialiseOpc();
    };
}
#endif // ALARMS_H
