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
#include "mqttcommand.h"
#include <Common/Daq/rtobject.h>
#include <Common/Daq/daqcommon.h>
//
// create static instances of each command handler
//
static class SetOutput : public MRL::MqttCommand
{
public:
    /*!
     * \brief SetOutput
     */
    SetOutput() : MqttCommand("setOutput") {} // register
    /*!
     * \brief action
     */
    void action(MRL::MqttConnection &/*connection*/, const std::string &/*topic*/, Json::Value &payload)
    {
        unsigned id = payload["objectId"].asUInt(); // get the object id
        if(id > 0)
        {
            // get the output name
            std::string out = payload["output"].asString();
            bool state = payload["state"].asBool();
            //
            // Now find the object and message it
            //
            MRL::RtObjectRef &r = MRL::Common::daq().objects()[id];
            if (r) {
                MRL::Message m(MESSAGEID::SetOutput);
                m.data().add(PARAMETERID::Index, out);
                m.data().add(PARAMETERID::Value, state);
                r->queueItem(m);
            }
        }
    }
} _setOuput;
