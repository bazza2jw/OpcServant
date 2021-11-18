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
#include "bobject.h"
#include <Common/messageids.h>

/*!
 * \brief MRL::BObject::processQueueItem
 * \param m
 * \return true if processed
 */
bool MRL::BObject::processQueueItem(const Message &m) {
    switch(m.id())
    {
        case MESSAGEID::ObjectStart: // start an object
        start();
        break;

        case MESSAGEID::ObjectStop:  // stop an object
        stop();
        break;

    default:
        return false;

    }

    return true; // return true if processed
}
