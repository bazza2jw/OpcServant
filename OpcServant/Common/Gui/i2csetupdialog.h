#ifndef I2CSETUPDIALOG_H
#define I2CSETUPDIALOG_H

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

#include <Common/Gui/SetupDialog.h>

/*!
 * \brief The I2cSetupDialog class
 */
class I2cSetupDialog : public SetupDialog
{
public:
    I2cSetupDialog(wxWindow* parent, const MRL::PropertyPath & _path);
protected:
    virtual void onConfigure(wxCommandEvent& event);
    virtual int addFields(wxFlexGridSizer *, int row);

};

#endif // I2CSETUPDIALOG_H
