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
#include "alarmtypeobject.h"

/*!
 * \brief MRL::AlarmTypeObject::addChildren
 * \param parent
 * \return
 */
bool MRL::AlarmTypeObject::addChildren(const Open62541::NodeId &parent)
{
    Open62541::NodeId n;
    // Status nodes
    n.notNull();
    addHistoricalObjectTypeVariable<int>("State", parent, n); // current object value
    setMandatory(n);
    return true;
}

