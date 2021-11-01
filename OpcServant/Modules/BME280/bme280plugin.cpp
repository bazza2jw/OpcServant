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
#include "bme280plugin.h"
#include "bme280objectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "bme280typeobject.h"

static MRL::BME280 _instance;//!< register
/*!
 * \brief MRL::BME280::BME280
 */
MRL::BME280::BME280() :Plugin("BME280")
{
}


/*!
 * \brief MRL::BME280::~System
 */
MRL::BME280::~BME280()
{

}

//
// Global initialise
void MRL::BME280::initialise()
{
    // add object factory
    (void) new MRL::BME280ObjectManager("BME280"); // create the object factory / manager
}
// Initialise the GUI
void MRL::BME280::initialiseGui()
{

}
// Initialise OPC
void MRL::BME280::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<BME280TypeObject>("BME280");
}
// Initialise Web
void MRL::BME280::initialiseWeb()
{

}
// Clean up
void MRL::BME280::terminate()
{

}
