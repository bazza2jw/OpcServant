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
#include "Linkplugin.h"
#include "Linkobjectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "Linktypeobject.h"

static MRL::Link _instance;//!< register
/*!
 * \brief MRL::Link::Link
 */
MRL::Link::Link() :Plugin("Link")
{
}


/*!
 * \brief MRL::Link::~System
 */
MRL::Link::~Link()
{

}

//
// Global initialise
void MRL::Link::initialise()
{
    // add object factory
    (void) new MRL::LinkObjectManager(); // create the object factory / manager
}
// Initialise the GUI
void MRL::Link::initialiseGui()
{

}
// Initialise OPC
void MRL::Link::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<LinkTypeObject>("Link");
}
// Initialise Web
void MRL::Link::initialiseWeb()
{

}
// Clean up
void MRL::Link::terminate()
{

}
