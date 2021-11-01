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
#ifndef EnviroPlusTYPEOBJECT_H
#define EnviroPlusTYPEOBJECT_H
#include <open62541cpp/serverobjecttype.h>
namespace MRL
{
/*!
     * \brief The EnviroPlusTypeObject class
     */
    class EnviroPlusTypeObject : public Open62541::ServerObjectType
    {
    public:
        EnviroPlusTypeObject(Open62541::Server &s, const std::string &n)
            : Open62541::ServerObjectType(s,n) {}
        bool addChildren(const Open62541::NodeId &parent) ;
        virtual bool addInstance(const std::string &n,
                                 Open62541::NodeId &parent,
                                 Open62541::NodeId &nodeId,
                                 Open62541::NodeId &requestNodeId = Open62541::NodeId::Null,
                                 Open62541::NodeContext *context = nullptr);
    };
}
#endif // EnviroPlusTYPEOBJECT_H
