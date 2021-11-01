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
#include "Mics6814plugin.h"
#include "Mics6814objectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "Mics6814typeobject.h"

static MRL::Mics6814 _instance;//!< register
/*!
 * \brief MRL::Mics6814::Mics6814
 */
MRL::Mics6814::Mics6814() :Plugin("Mics6814")
{
}


/*!
 * \brief MRL::Mics6814::~System
 */
MRL::Mics6814::~Mics6814()
{

}

//
// Global initialise
void MRL::Mics6814::initialise()
{
    // add object factory
    (void) new MRL::Mics6814ObjectManager(); // create the object factory / manager
}
// Initialise the GUI
void MRL::Mics6814::initialiseGui()
{

}
// Initialise OPC
void MRL::Mics6814::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<Mics6814TypeObject>("Mics6814");
}
// Initialise Web
void MRL::Mics6814::initialiseWeb()
{

}
// Clean up
void MRL::Mics6814::terminate()
{

}
