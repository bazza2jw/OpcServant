#ifndef CONFIGUREI2CDIALOG_H
#define CONFIGUREI2CDIALOG_H
/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#include "BourbonCommonUI.h"
#include <Common/common.h>
/*!
 * \brief The ConfigureI2cDialog class
 * Setup I2C configuration
 */
class ConfigureI2cDialog : public ConfigureI2cDialogBase
{
    MRL::VariantPropertyTree &_configuration; // tree of configuration values

public:
    ConfigureI2cDialog(wxWindow* parent, MRL::VariantPropertyTree &c);
    virtual ~ConfigureI2cDialog();
protected:
    virtual void onTypeChanged(wxCommandEvent& event);
    virtual void onOK(wxCommandEvent& event);
    void onI2cTypeSet();
};
#endif // CONFIGUREI2CDIALOG_H
