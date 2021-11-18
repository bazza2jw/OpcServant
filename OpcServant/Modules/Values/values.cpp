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
#include "values.h"
#include "valueobjectmanager.h"
#include <Common/Opc/opccommon.h>
#include <Common/Opc/opcserver.h>
#include "valuetypeobject.h"

static MRL::Value _instance;//!< register
/*!
 * \brief MRL::Value::Value
 */
MRL::Value::Value() :Plugin("Value")
{
}
//
// Global initialise
void MRL::Value::initialise()
{
    // add object factory
    (void) new MRL::ValueObjectManager(); // create the object factory / manager
}
// Initialise OPC
void MRL::Value::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<ValueTypeObject>("Value");
}



