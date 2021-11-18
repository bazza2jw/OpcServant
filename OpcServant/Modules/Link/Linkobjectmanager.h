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
#ifndef LinkOBJECTMANAGER_H
#define LinkOBJECTMANAGER_H
#include <Common/objectmanager.h>
#include <Common/Gui/SetupDialog.h>
#include <Common/Web/webpanel.h>
#include "Linkrt.h"

namespace MRL
{
/*!
     * \brief The LinkObjectManager class
     */
typedef ObjectManagerRT<LinkRT,SetupNetworkDialog,ConfigurationNetworkWebDialog> LinkObjectManagerBase;

class LinkObjectManager : public LinkObjectManagerBase
{
public:
    LinkObjectManager() : LinkObjectManagerBase("Link") {}
};
}
#endif // LinkOBJECTMANAGER_H
