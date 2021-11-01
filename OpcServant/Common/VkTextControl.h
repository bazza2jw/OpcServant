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
#ifndef VKTEXTCONTROL_H
#define VKTEXTCONTROL_H
#include "VirtualKeypadUI.h"

class VkTextControl : public VkTextControlBase
{
public:
    VkTextControl(wxWindow* parent);
    virtual ~VkTextControl();
protected:
    virtual void onOpen(wxCommandEvent& event);
};
#endif // VKTEXTCONTROL_H
