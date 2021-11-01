/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "EnviroPlusplugin.h"
#include "EnviroPlusobjectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "EnviroPlustypeobject.h"

static MRL::EnviroPlus _instance;//!< register
/*!
 * \brief MRL::EnviroPlus::EnviroPlus
 */
MRL::EnviroPlus::EnviroPlus() :Plugin("EnviroPlus")
{
}


/*!
 * \brief MRL::EnviroPlus::~System
 */
MRL::EnviroPlus::~EnviroPlus()
{

}

//
// Global initialise
void MRL::EnviroPlus::initialise()
{
    // add object factory
    (void) new MRL::EnviroPlusObjectManager(); // create the object factory / manager
}
// Initialise the GUI
void MRL::EnviroPlus::initialiseGui()
{

}
// Initialise OPC
void MRL::EnviroPlus::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<EnviroPlusTypeObject>("EnviroPlus");
}
// Initialise Web
void MRL::EnviroPlus::initialiseWeb()
{

}
// Clean up
void MRL::EnviroPlus::terminate()
{

}
