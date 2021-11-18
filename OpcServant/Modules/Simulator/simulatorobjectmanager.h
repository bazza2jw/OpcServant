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
#ifndef SIMULATOROBJECTMANAGER_H
#define SIMULATOROBJECTMANAGER_H
#include <Common/objectmanager.h>
#include "simulator_global.h"
#include "SimulatorConfigurationDialog.h"
#include "simulatorconfigurationwebdialog.h"
#include "simulatorrt.h"


namespace MRL
{
/*!
     * \brief The SimulatorObjectManager class
     */
    typedef ObjectManagerRT<SimulatorRT,SimulatorConfigurationDialog,SimulatorConfigurationWebForm> SimulatorObjectManagerBase;
    class SimulatorObjectManager : public SimulatorObjectManagerBase
    {
    public:
        SimulatorObjectManager() : SimulatorObjectManagerBase("Simulator") { }
        size_t opcHistoryCount() const { return 1;}
     };
}
#endif // SIMULATOROBJECTMANAGER_H
