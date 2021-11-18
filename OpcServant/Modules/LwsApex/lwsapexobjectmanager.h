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
#ifndef LWSAPEXOBJECTMANAGER_H
#define LWSAPEXOBJECTMANAGER_H

#include <Common/objectmanager.h>
#include "lwsapexrt.h"
#include "LwsApexConfigurationDialog.h"
#include "lwswebconfiguration.h"

namespace MRL
{
/*!
 * \brief The LwsApexObjectManager class
 */
typedef  ObjectManagerRT<LwsApexRT,LwsApexConfigurationDialog,LwsWebConfiguration> LwsApexObjectManagerBase;

class LwsApexObjectManager: public LwsApexObjectManagerBase
{
public:
    LwsApexObjectManager() : LwsApexObjectManagerBase("LwsApex") {}
    //
};
}
#endif // LWSAPEXOBJECTMANAGER_H
