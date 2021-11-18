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
#include "OPCN3plugin.h"
#include "OPCN3objectmanager.h"
#include "OPCN3typeobject.h"
#include <Common/common.h>
#include <Common/Opc/opccommon.h>

static MRL::OPCN3Plugin _instance;//!< register
/*!
 * \brief MRL::OPCN3Plugin::OPCN3Plugin
 */
MRL::OPCN3Plugin::OPCN3Plugin() :Plugin("OPCN3")
{
}


/*!
 * \brief MRL::OPCN3Plugin::~System
 */
MRL::OPCN3Plugin::~OPCN3Plugin()
{

}

//
// Global initialise
void MRL::OPCN3Plugin::initialise()
{
    // add object factory
    (void) new MRL::OPCN3ObjectManager(); // create the object factory / manager
}
// Initialise the GUI
void MRL::OPCN3Plugin::initialiseGui()
{

}
// Initialise OPC
void MRL::OPCN3Plugin::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<OPCN3TypeObject>("OPCN3");
}
// Initialise Web
void MRL::OPCN3Plugin::initialiseWeb()
{

}
// Clean up
void MRL::OPCN3Plugin::terminate()
{

}
