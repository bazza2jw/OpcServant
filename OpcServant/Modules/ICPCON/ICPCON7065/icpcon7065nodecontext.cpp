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
#include "icpcon7065nodecontext.h"
#include <Common/common.h>
#include <MrlLib/mrllib.h>
#include <open62541cpp/open62541server.h>
#include <Common/messageids.h>
#include <Common/Daq/daqcommon.h>
#include "icpcon7065rt.h"


/*!
    \brief readData
    \param node
    \param range
    \param value
    \return
*/
bool MRL::ICPCON7065NodeContext::readData(Open62541::Server &/*server*/,
                                     Open62541::NodeId &/*node*/, const UA_NumericRange * /*range*/,
                                     UA_DataValue &value) {
    // get the value to update
    // find the instance
    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[_rtId];
    if(r)
    {
        // seek the output value
        // This must point to a ICPCON7065RT object
        MRL::ICPCON7065RT *p = static_cast<MRL::ICPCON7065RT *>(r.get());
        UA_Boolean v = p->output(_output)?UA_TRUE:UA_FALSE;
        UA_Variant_setScalarCopy(&value.value, &v,&UA_TYPES[UA_TYPES_BOOLEAN]); // set the value
    }
    return true;
}

/*!
    \brief writeData
    \param server
    \param node
    \param range
    \param value
    \return
*/
bool MRL::ICPCON7065NodeContext::writeData(Open62541::Server &/*server*/,  Open62541::NodeId &node,
                                      const UA_NumericRange * /*range*/, const UA_DataValue &value) {
    // get the value to update
    if (node.identifierType() == UA_NODEIDTYPE_NUMERIC) {
        if (value.hasValue) {
            if (value.value.type == &UA_TYPES[UA_TYPES_BOOLEAN]) {
                // find the DAQ object
                MRL::RtObjectRef &r =  MRL::Common::daq().objects()[_rtId];
                if(r)
                {
                    UA_Boolean *b = (UA_Boolean *)(value.value.data);
                    bool v = *b == UA_TRUE;
                    MRL::ICPCON7065RT *p = static_cast<MRL::ICPCON7065RT *>(r.get());
                    p->setOutput(_output,v);

                }
            }
        }
    }
    return true;
}

