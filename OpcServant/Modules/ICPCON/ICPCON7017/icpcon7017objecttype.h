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
#ifndef ICPCON7017OBJECTTYPE_H
#define ICPCON7017OBJECTTYPE_H
#include "../icpcon_global.h"
#include <open62541cpp/serverobjecttype.h>
namespace MRL {
/*!
     * \brief The ICPCON7017ObjectType class
     */
    class ICPCON7017ObjectType : public Open62541::ServerObjectType {

        public:
            ICPCON7017ObjectType(Open62541::Server &s, const std::string &n)
                : Open62541::ServerObjectType(s, n) {}
            bool addChildren(const Open62541::NodeId &parent) ;
            virtual bool addInstance(const std::string &n,
                                     const Open62541::NodeId &parent,
                                     Open62541::NodeId &nodeId,
                                     const Open62541::NodeId &requestNodeId = Open62541::NodeId::Null,
                                     Open62541::NodeContext *context = nullptr);
    };
}
#endif // ICPCON7017OBJECTTYPE_H
