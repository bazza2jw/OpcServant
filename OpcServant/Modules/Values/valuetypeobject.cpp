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
#include "valuetypeobject.h"


/*!
 * \brief MRL::ValueTypeObject::addChildren
 * \param parent
 * \return
 */
bool MRL::ValueTypeObject::addChildren(const Open62541::NodeId &parent)
{
    Open62541::NodeId n;
    // Status nodes -
    n.notNull();
    addObjectTypeVariable<std::string>("Status", parent, n); // alarm status
    setMandatory(n);
    return true;
}

/*!
 * \brief MRL::ValueTypeObject::addInstance
 * \param n
 * \param parent
 * \param nodeId
 * \param requestNodeId
 * \param context
 * \return
 */
bool MRL::ValueTypeObject::addInstance(const std::string &n,
                         const Open62541::NodeId &parent,
                         Open62541::NodeId &nodeId,
                         const Open62541::NodeId &requestNodeId,
                         Open62541::NodeContext *context)
{
    if(Open62541::ServerObjectType::addInstance(n,parent,nodeId,requestNodeId,context))
    {

        return true;
    }
    return false;
}
