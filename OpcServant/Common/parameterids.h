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
#ifndef PARAMETERIDS_H
#define PARAMETERIDS_H
// IDs for stock message parameters.
namespace PARAMETERID {
    //
    // Parameter ids
    enum {
        ObjectId = 1,
        Status = 0x100, // status of object
        ActionAlertLevel, // alarm / warning status
        Timestamp, // timestamp
        Value, // current value for display
        Record, // a record - a set of values probably
        Tag, // tag for value - add to path
        Minimum, // value statisitics parameters
        Maximum,
        Average,
        StdDev,
        Count,
        Path, // path to an item
        // For event messages
        MessageType,
        Message,
        Source,
        //
        Index, // an index to value
        // some sources / sinks have multiple inputs and outputs
        // Allows for rather big spectra
        Input0 = 0x1000, // input values are based on Input0
        Ouput0 = 0x2000, // output values are based on Output0
        //
        UserFirst = 0x8000
    };

}
#endif // PARAMETERIDS_H
