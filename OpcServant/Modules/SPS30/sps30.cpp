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
#include "sps30.h"
#include "sps30objectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "sps30typeobject.h"

static MRL::SPS30 _instance;//!< register
/*!
 * \brief MRL::SPS30::Simulator
 */
MRL::SPS30::SPS30() :Plugin("SPS30")
{
}


/*!
 * \brief MRL::SPS30::~System
 */
MRL::SPS30::~SPS30()
{

}

//
// Global initialise
void MRL::SPS30::initialise()
{
    // add object factory
    (void) new MRL::SPS30ObjectManager("SPS30"); // create the object factory / manager
}
// Initialise the GUI
void MRL::SPS30::initialiseGui()
{

}
// Initialise OPC
void MRL::SPS30::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<SPS30TypeObject>("SPS30");
}
// Initialise Web
void MRL::SPS30::initialiseWeb()
{

}
// Clean up
void MRL::SPS30::terminate()
{

}
