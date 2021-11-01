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
#ifndef LWSAPEXTYPEOBJECT_H
#define LWSAPEXTYPEOBJECT_H
#include <open62541cpp/serverobjecttype.h>
namespace MRL
{
/*!
 * \brief The LwsApexTypeObject class
 */
class LwsApexTypeObject : public Open62541::ServerObjectType
{
public:
    LwsApexTypeObject(Open62541::Server &s, const std::string &n)
        : Open62541::ServerObjectType(s,n) {}
    bool addChildren(const Open62541::NodeId &parent) ;
};

}
#endif // LWSAPEXTYPEOBJECT_H
