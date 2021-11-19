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
#include "OPCN3typeobject.h"

/*!
 * \brief MRL::SimulatorTypeObject::addChildren
 * \param parent
 * \return true on success
 */
bool MRL::OPCN3TypeObject::addChildren(const Open62541::NodeId &parent)
{
    Open62541::NodeId n;
    // Status nodes
    n.notNull();
    addHistoricalObjectTypeVariable<bool>("Status", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addHistoricalObjectTypeVariable<double>("PM1", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addHistoricalObjectTypeVariable<double>("PM2.5", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addHistoricalObjectTypeVariable<double>("PM10", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addHistoricalObjectTypeVariable<double>("Temperature", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addHistoricalObjectTypeVariable<double>("Humidity", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addHistoricalObjectTypeVariable<int>("LaserStatus", parent, n); // current object value
    setMandatory(n);
    n.notNull();
    addHistoricalObjectTypeVariable<int>("FanSpeed", parent, n); // current object value
    setMandatory(n);

    return true;
}
