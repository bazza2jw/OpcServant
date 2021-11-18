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
#include "sps30typeobject.h"
#include "sps30rt.h"



/*!
 * \brief MRL::SPS30TypeObject::addChildren
 * \param parent
 * \return
 */
bool MRL::SPS30TypeObject::addChildren(const Open62541::NodeId &parent)
{
    Open62541::NodeId n;
    // Status nodes
    for(size_t i = 0; i < SPS30RT::_inputs.size(); i++)
    {
        n.notNull();
        addObjectTypeVariable<double>(SPS30RT::_inputs[i], parent, n); // current object value
        setMandatory(n);
    }
    return true;
}

/*!
 * \brief MRL::SPS30TypeObject::addInstance
 * \param n
 * \param parent
 * \param nodeId
 * \param requestNodeId
 * \param context
 * \return
 */
bool MRL::SPS30TypeObject::addInstance(const std::string &n,
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
