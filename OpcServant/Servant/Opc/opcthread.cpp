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
#include "opcthread.h"
#include <Common/common.h>
#include <Common/Opc/opccommon.h>

/*!
 * \brief MRL::OpcThread::OpcThread
 */
MRL::OpcThread::OpcThread()
{
    MRL::PropertyPath p;
    p.push_back("System");

    std::string port = MRL::SETTINGS().getValue<std::string>(p, "OpcPort");
    _opc = std::make_unique<OpcServer>(std::atoi(port.c_str()));
    Common::opc().setServer(_opc.get());
}

/*!
 * \brief MRL::OpcThread::~OpcThread
 */
MRL::OpcThread::~OpcThread()
{
    Common::opc().setServer(nullptr);
}



