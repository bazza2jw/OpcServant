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
#ifndef LWSAPEXPANEL_H
#define LWSAPEXPANEL_H
#include "LwsApexUI.h"

#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/bobject.h>
/*!
 * \brief The LwsApexPanel class
 */
class LwsApexPanel : public LwsApexPanelBase, public MRL::BObject
{
public:
    LwsApexPanel(wxWindow* parent, unsigned id);
    virtual ~LwsApexPanel();
    bool processQueueItem(const MRL::Message &msg);

};
#endif // LWSAPEXPANEL_H
