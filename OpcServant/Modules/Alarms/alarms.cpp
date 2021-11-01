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
#include "alarms.h"
#include "alarmobjectmanager.h"
#include "alarmtypeobject.h"

static MRL::Alarms _instance;//!< register
/*!
 * \brief MRL::Alarm::Alarm
 */
MRL::Alarms::Alarms() :Plugin("Alarm")
{
}

//
// Global initialise
void MRL::Alarms::initialise()
{
    // add object factory
    (void) new MRL::AlarmObjectManager(); // create the object factory / manager
}

// Initialise OPC
void MRL::Alarms::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<AlarmTypeObject>("Alarm");
}

