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
#include "readwritecontext.h"

/*!
    \brief writeValue
    The (assumed) HMI has changed an assumed control value. Generate a notification message
    \param node
*/
void MRL::ReadWriteContext::writeValue(Open62541::Server &/*server*/,
                                       Open62541::NodeId &/*node*/,
                                       const UA_NumericRange * /*range*/,
                                       const UA_DataValue &/*value*/) {
    // update the runtime tree and propogate a notification to the object
    // post update notification
}
