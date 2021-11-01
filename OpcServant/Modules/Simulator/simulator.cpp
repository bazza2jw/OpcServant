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
#include "simulator.h"
#include "simulatorobjectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "simulatortypeobject.h"

static MRL::Simulator _instance;//!< register
/*!
 * \brief MRL::Simulator::Simulator
 */
MRL::Simulator::Simulator() :Plugin("Simulator")
{
}


/*!
 * \brief MRL::Simulator::~System
 */
MRL::Simulator::~Simulator()
{

}

//
// Global initialise
void MRL::Simulator::initialise()
{
    // add object factory
    (void) new MRL::SimulatorObjectManager(); // create the object factory / manager
}
// Initialise the GUI
void MRL::Simulator::initialiseGui()
{

}
// Initialise OPC
void MRL::Simulator::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<SimulatorTypeObject>("Simulator");
}
// Initialise Web
void MRL::Simulator::initialiseWeb()
{

}
// Clean up
void MRL::Simulator::terminate()
{

}
