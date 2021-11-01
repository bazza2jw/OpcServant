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
#include "EnviroPlustypeobject.h"



/*!
 * \brief MRL::EnviroPlusTypeObject::addChildren
 * \param parent
 * \return
 */
bool MRL::EnviroPlusTypeObject::addChildren(Open62541::NodeId &parent)
{
    Open62541::NodeId n;
    // values
    n.notNull();
    addObjectTypeVariable<double>("Temperature", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addObjectTypeVariable<double>("Pressure", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addObjectTypeVariable<double>("Humidity", parent, n); // current object value
    setMandatory(n);
    return true;
}

/*!
 * \brief MRL::EnviroPlusTypeObject::addInstance
 * \param n
 * \param parent
 * \param nodeId
 * \param requestNodeId
 * \param context
 * \return
 */
bool MRL::EnviroPlusTypeObject::addInstance(const std::string &n,
                         Open62541::NodeId &parent,
                         Open62541::NodeId &nodeId,
                         Open62541::NodeId &requestNodeId,
                         Open62541::NodeContext *context)
{
    if(Open62541::ServerObjectType::addInstance(n,parent,nodeId,requestNodeId,context))
    {

        return true;
    }
    return false;
}
