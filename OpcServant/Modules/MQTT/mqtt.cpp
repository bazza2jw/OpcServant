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
#include "mqtt.h"
#include <mosquittopp.h>
#include "mqttobjectmanager.h"
#include "mqttrt.h"
/*!
 * \brief _instance
 */
static MRL::MQTT _instance; // self register
/*!
 * \brief MRL::MQTT::MQTT
 */
MRL::MQTT::MQTT() : Plugin("MQTT")
{
}
/*!
 * \brief MRL::MQTT::initialise
 */
void MRL::MQTT::initialise()
{
    mosqpp::lib_init();
    (void) new MQTTObjectManager(); // MQTT publisher
    //
    // Add the MQTT command processors
    //
}

/*!
 * \brief MRL::MQTT::terminate
 */
void MRL::MQTT::terminate()
{
    mosqpp::lib_cleanup();
}
