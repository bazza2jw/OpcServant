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
#ifndef SBCSUPPORTRT_H
#define SBCSUPPORTRT_H


#include "sbcsupport_global.h"
#include <Common/Daq/rtobject.h>
namespace MRL {
    // runtime SbcSupport object
    // little more than a place holder
    // when the RT object is created the corresponding UI is expect to be created
    // this allows for periodic SBC type functions such as watchdog servicing and UPS monitoring - TODO
/*!
     * \brief The SbcSupportRT class
     */
    class SbcSupportRT : public RTObject
    {
    public:
        SbcSupportRT(int id) : RTObject(id)
        {

        }
        bool createTabView() { return true;} // force for now
    };
 }
#endif // SBCSUPPORTRT_H
