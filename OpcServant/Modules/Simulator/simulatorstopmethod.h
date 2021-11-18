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
#ifndef SIMULATORSTOPMETHOD_H
#define SIMULATORSTOPMETHOD_H

#include <open62541cpp/servermethod.h>
/*!
 * \brief The SimulatorStopMethod class
 */
class SimulatorStopMethod : public Open62541::ServerMethod
{
    Open62541::Argument inputArgument1; // argument definitions must persist
    Open62541::Argument outputArguments;
public:
    SimulatorStopMethod() : Open62541::ServerMethod("Stop",0,0)
    {

    }

    virtual UA_StatusCode callback(Open62541::Server &/*server*/,
                                   const UA_NodeId */*objectId*/,
                                   size_t /*inputSize*/,
                                   const UA_Variant * /*input*/,
                                   size_t /*outputSize*/,
                                   UA_Variant * /*output*/);

};

#endif // SIMULATORSTOPMETHOD_H
