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
#include "icpcon7065objecttype.h"
#include "icpcon7065rt.h"
#include "icpcon7065nodecontext.h"

/*!
 * \brief MRL::ICPCON7065ObjectType::addChildren
 * \param parent
 * \return true on success
 */
bool MRL::ICPCON7065ObjectType::addChildren(const Open62541::NodeId &parent)
{
    for(int i = 0; i < NUMBER_7065_CHAN; i++)
    {
        Open62541::NodeId n;
        n.notNull();
        addObjectTypeVariable<bool>(ICPCON7065RT::_channelNames[i], parent, n); // current object value
        setMandatory(n);
    }
    //
    Open62541::NodeId ns;
    ns.notNull();
    addObjectTypeVariable<int>("Status", parent, ns); // value range
    setMandatory(ns);
    //
    return true;
}

/*!
 * \brief MRL::ICPCON7065ObjectType::addInstance
 * \param n
 * \param parent
 * \param nodeId
 * \param requestNodeId
 * \param context
 * \return true on success
 */
bool MRL::ICPCON7065ObjectType::addInstance(const std::string &n,
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
