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
#include "simulatorstopmethod.h"

/*!
 * \brief SimulatorStopMethod::callback
 * \param server
 * \return UA_STATUSCODE_GOOD
 */
UA_StatusCode SimulatorStopMethod::callback(Open62541::Server &/*server*/,
                               const UA_NodeId */*objectId*/,
                               size_t /*inputSize*/,
                               const UA_Variant * /*input*/,
                               size_t /*outputSize*/,
                               UA_Variant * /*output*/)
{
    return UA_STATUSCODE_GOOD;
}
