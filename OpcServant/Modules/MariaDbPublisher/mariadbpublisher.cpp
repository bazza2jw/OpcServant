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
#include "mariadbpublisher.h"
#include "mariadbpublisherobjectmanager.h"
#include "mariadbobjecttype.h"
static MRL::MariaDbPublisher _instance; // self register

MRL::MariaDbPublisher::MariaDbPublisher() : Plugin("MariaDbPublisher")
{

}

void MRL::MariaDbPublisher::initialise()
{
    (void) new MariaDbPublisherObjectManager();
}

// Initialise OPC
void MRL::MariaDbPublisher::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<MariaDbObjectType>("MariaDbPublisher");
}
