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
#ifndef ICPCON7017OBJECTMANAGER_H
#define ICPCON7017OBJECTMANAGER_H
#include "../icpcon_global.h"
#include <Common/objectmanager.h>
#include "icpcon7017rt.h"
#include "ICPCON7017SetupDialog.h"
#include "icpcon7017setupwebdialog.h"

namespace MRL {
/*!
     * \brief The ICPCON7017ObjectManager class
     */
typedef ObjectManagerRT<ICPCON7017RT,ICPCON7017SetupDialog,ICPCON7017SetupWebDialog> ICPCON7017ObjectManagerBase;

    class ICPCON7017ObjectManager  : public ICPCON7017ObjectManagerBase {
        public:
        ICPCON7017ObjectManager() : ICPCON7017ObjectManagerBase("ICPCON7017") {}
    };
}
#endif // ICPCON7017OBJECTMANAGER_H
