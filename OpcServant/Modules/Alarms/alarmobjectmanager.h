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
#ifndef ALARMOBJECTMANAGER_H
#define ALARMOBJECTMANAGER_H
#include <Common/objectmanager.h>
#include "alarms_global.h"
#include "AlarmConfigurationDialog.h"
#include "alarmconfigurationwebdialog.h"
#include <Common/Web/webdialog.h>
#include "alarmrt.h"

namespace MRL
{
    typedef ObjectManagerRT<AlarmRT,AlarmConfigurationDialog,AlarmConfigurationWebDialog> AlarmObjectManagerBase;
    class AlarmObjectManager : public AlarmObjectManagerBase
    {
    public:
        AlarmObjectManager() : AlarmObjectManagerBase("Alarm") {}
     };
}

#endif // ALARMOBJECTMANAGER_H
