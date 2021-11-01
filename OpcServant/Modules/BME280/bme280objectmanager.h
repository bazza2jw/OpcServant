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
#ifndef BME280OBJECTMANAGER_H
#define BME280OBJECTMANAGER_H
#include <Common/objectmanager.h>
#include "bme280rt.h"
namespace MRL
{
/*!
     * \brief The BME280ObjectManager class
     */
    typedef  ObjectManagerI2cRT<BME280RT> BME280ObjectManager;
}
#endif // BME280OBJECTMANAGER_H
