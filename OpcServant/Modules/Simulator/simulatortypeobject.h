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
#ifndef SIMULATORTYPEOBJECT_H
#define SIMULATORTYPEOBJECT_H
#include <open62541cpp/serverobjecttype.h>
namespace MRL
{
/*!
     * \brief The SimulatorTypeObject class
     */
    class SimulatorTypeObject : public Open62541::ServerObjectType
    {
    public:
        SimulatorTypeObject(Open62541::Server &s, const std::string &n)
            : Open62541::ServerObjectType(s,n) {}
        bool addChildren(const Open62541::NodeId &parent) ;
        virtual bool addInstance(const std::string &n,
                                 const Open62541::NodeId &parent,
                                 Open62541::NodeId &nodeId,
                                 const Open62541::NodeId &requestNodeId = Open62541::NodeId::Null,
                                 Open62541::NodeContext *context = nullptr);
    };
}
#endif // SIMULATORTYPEOBJECT_H
