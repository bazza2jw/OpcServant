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
#include "I2CADCplugin.h"
#include "I2CADCobjectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "I2CADCtypeobject.h"

static MRL::I2CADC _instance;//!< register
/*!
 * \brief MRL::I2CADC::I2CADC
 */
MRL::I2CADC::I2CADC() :Plugin("I2CADC")
{
}


/*!
 * \brief MRL::I2CADC::~System
 */
MRL::I2CADC::~I2CADC()
{

}

//
// Global initialise
void MRL::I2CADC::initialise()
{
    // add object factory
    (void) new MRL::I2CADCObjectManager(); // create the object factory / manager
}
// Initialise the GUI
void MRL::I2CADC::initialiseGui()
{

}
// Initialise OPC
void MRL::I2CADC::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<I2CADCTypeObject>("I2CADC");
}
// Initialise Web
void MRL::I2CADC::initialiseWeb()
{

}
// Clean up
void MRL::I2CADC::terminate()
{

}
