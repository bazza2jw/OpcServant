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
#include "icpcon.h"
#include "ICPCON7017/icpcon7017objectmanager.h"
#include "ICPCON7017/icpcon7017objecttype.h"
#include "ICPCON7065/icpcon7065objectmanager.h"
#include "ICPCON7065/icpcon7065objecttype.h"

static MRL::ICPCON _instance; // self register

/*!
 * \brief MRL::ICPCON::ICPCON
 */
MRL::ICPCON::ICPCON() : Plugin("ICPCON")
{

}

/*!
 * \brief MRL::ICPCON::initialise
 */
void MRL::ICPCON::initialise()
{
    // for each of the supported modules create an object manager
    (void) new ICPCON7017ObjectManager();
    (void) new ICPCON7065ObjectManager();
}

// Initialise OPC
/*!
 * \brief MRL::ICPCON::initialiseOpc
 */
void MRL::ICPCON::initialiseOpc()
{
    // Construct OPC type Object for each of the supported modules
    Common::opc().add<ICPCON7017ObjectType>("ICPCON7017");
    Common::opc().add<ICPCON7065ObjectType>("ICPCON7065");
}

