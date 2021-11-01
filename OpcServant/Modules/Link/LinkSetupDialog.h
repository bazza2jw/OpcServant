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
#ifndef LinkSETUPDIALOG_H
#define LinkSETUPDIALOG_H
#include "LinkUI.h"
#include <Common/common.h>

/*!
 * \brief The LinkSetupDialog class
 */
class LinkSetupDialog : public LinkSetupDialogBase
{
    MRL::PropertyPath _path;
public:
    LinkSetupDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~LinkSetupDialog();
protected:
    virtual void OnOk(wxCommandEvent& event);
};
#endif // LinkSETUPDIALOG_H
