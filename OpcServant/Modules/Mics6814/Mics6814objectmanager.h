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
#ifndef Mics6814OBJECTMANAGER_H
#define Mics6814OBJECTMANAGER_H
#include <Common/objectmanager.h>
#include "Mics6814rt.h"

namespace MRL
{
/*!
     * \brief The Mics6814ObjectManager class
     */
typedef ObjectManagerI2cRT<Mics6814RT> Mics6814ObjectManagerBase;

class Mics6814ObjectManager : public Mics6814ObjectManagerBase
{
public:
    Mics6814ObjectManager() : Mics6814ObjectManagerBase("Mics6814") {}
};
}
#endif // Mics6814OBJECTMANAGER_H
