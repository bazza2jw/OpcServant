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
#include "VisualCalcplugin.h"
#include "VisualCalcobjectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "VisualCalctypeobject.h"

static MRL::VisualCalc _instance;//!< register
/*!
 * \brief MRL::VisualCalc::VisualCalc
 */
MRL::VisualCalc::VisualCalc() :Plugin("VisualCalc")
{
}


/*!
 * \brief MRL::VisualCalc::~System
 */
MRL::VisualCalc::~VisualCalc()
{

}

//
// Global initialise
void MRL::VisualCalc::initialise()
{
    // add object factory
    (void) new MRL::VisualCalcObjectManager(); // create the object factory / manager
}
// Initialise the GUI
void MRL::VisualCalc::initialiseGui()
{

}
// Initialise OPC
void MRL::VisualCalc::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<VisualCalcTypeObject>("VisualCalc");
}
// Initialise Web
void MRL::VisualCalc::initialiseWeb()
{

}
// Clean up
void MRL::VisualCalc::terminate()
{

}
