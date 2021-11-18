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
#include "readonlycontext.h"

/*!
 * \brief MRL::ReadOnlyContext::readValue
 */
void MRL::ReadOnlyContext::readValue(Open62541::Server &/*server*/,
                                     Open62541::NodeId &/*node*/,
                                     const UA_NumericRange * /*range*/,
                                     const UA_DataValue */*value*/) {
    // read a value from the Runtime data tree
    // Node Id must be (will be) numeric index to the value
}

