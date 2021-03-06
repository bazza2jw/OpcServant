/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#include "nodeflow.h"

/*!
 * \brief NODEFLOW::NodeFlow::_msgId
 */
int NODEFLOW::NodeFlow::_msgId = 0;

/*!
 * \brief NODEFLOW::NodeFlow::msgId
 * \return
 */
int NODEFLOW::NodeFlow::msgId()
{
    return _msgId++;
}

/*!
 * \brief NODEFLOW::NodeFlow::initialiseNodes
 */
void addFunctionNodes();
void addValueNodes();
void addOutputs();
void registerMQTTNodes();
void addControlNodes();
void addDataProcessing();
void NODEFLOW::NodeFlow::initialiseNodes()
{
    addFunctionNodes();
    addValueNodes();
    addOutputs();
    addControlNodes();
    addDataProcessing();
}
