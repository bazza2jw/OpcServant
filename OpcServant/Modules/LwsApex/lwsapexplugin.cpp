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
#include "lwsapexplugin.h"
#include "lwsapexobjectmanager.h"
#include "lwsapextypeobject.h"
#include <Common/common.h>
#include <Common/Opc/opccommon.h>

static MRL::LwsApexPlugin _instance;//!< register
/*!
 * \brief MRL::LwsApexPlugin::LwsApexPlugin
 */
MRL::LwsApexPlugin::LwsApexPlugin() :Plugin("LwsApex")
{
}


/*!
 * \brief MRL::LwsApexPlugin::~System
 */
MRL::LwsApexPlugin::~LwsApexPlugin()
{

}

//
// Global initialise
void MRL::LwsApexPlugin::initialise()
{
    // add object factory
    (void) new MRL::LwsApexObjectManager(); // create the object factory / manager
}
// Initialise the GUI
void MRL::LwsApexPlugin::initialiseGui()
{

}
// Initialise OPC
void MRL::LwsApexPlugin::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<LwsApexTypeObject>("LwsApex");
}
// Initialise Web
void MRL::LwsApexPlugin::initialiseWeb()
{

}
// Clean up
void MRL::LwsApexPlugin::terminate()
{

}
