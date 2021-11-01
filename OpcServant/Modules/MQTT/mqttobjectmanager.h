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
#ifndef MQTTOBJECTMANAGER_H
#define MQTTOBJECTMANAGER_H
#include "mqtt_global.h"
#include <Common/objectmanager.h>
#include "mqttrt.h"
#include "MqttSetupDialog.h"
#include "mqttwebsetupdialog.h"

namespace MRL {
/*!
 * \brief The MQTTObjectManager class
*/
typedef ObjectManagerPublisher< MQTTRt, MqttSetupDialog, MqttWebSetupDialog> MQTTObjectManagerBase;

class MQTTObjectManager : public MQTTObjectManagerBase {
public:
    MQTTObjectManager() : MQTTObjectManagerBase("MQTT") {}
};
}
#endif // MQTTOBJECTMANAGER_H
