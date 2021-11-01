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
#ifndef ADDOBJECTDIALOG_H
#define ADDOBJECTDIALOG_H
#include "BourbonCommonUI.h"
#include <MrlLib/mrllib.h>
#include <Common/common.h>
class AddObjectDialog : public AddObjectDialogBase
{
    MRL::PropertyPath _path;
public:
    AddObjectDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~AddObjectDialog();
protected:
    virtual void OnEditChanged(wxCommandEvent& event);
    virtual void OnOk(wxCommandEvent& event);
};
#endif // ADDOBJECTDIALOG_H
