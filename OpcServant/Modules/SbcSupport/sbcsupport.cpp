/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided with open62541.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "sbcsupport.h"
#include "sbcsupportobjectmanager.h"

/*!
 * \brief _instance
 */
static MRL::SbcSupport _instance; // self register
/*!
 * \brief MRL::SbcSupport::SbcSupport
 */
MRL::SbcSupport::SbcSupport() : Plugin("SbcSupport")
{
}
/*!
 * \brief MRL::SbcSupport::initialise
 */
void MRL::SbcSupport::initialise()
{
    (void) new MRL::SbcSupportObjectManager(); // SbcSupport publisher
}
