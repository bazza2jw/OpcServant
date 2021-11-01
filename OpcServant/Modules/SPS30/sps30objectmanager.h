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
#ifndef SPS30OBJECTMANAGER_H
#define SPS30OBJECTMANAGER_H
#include <Common/objectmanager.h>
#include "sps30rt.h"
namespace MRL
{
/*!
     * \brief The SPS30ObjectManager class
     */
    typedef ObjectManagerI2cRT<SPS30RT> SPS30ObjectManager;
}
#endif // SPS30OBJECTMANAGER_H
