#ifndef CONFIGUREI2CWEBDIALOG_H
#define CONFIGUREI2CWEBDIALOG_H
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

#include <Common/common.h>
#include <Common/Web/webdialog.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WPanel.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WComboBox.h>
#include <Wt/WLabel.h>
#include <Wt/WPushButton.h>
#include <Wt/WGridLayout.h>
#include <Wt/WSpinBox.h>

namespace MRL {
/*!
 * \brief The ConfigureI2cWebDialog
 * configuration I2C using web interface
 */
class ConfigureI2cWebDialog : public WebDialogBase
{
    MRL::VariantPropertyTree &_configuration; // tree of configuration values

    // I2C settings
    Wt::WLineEdit * _i2cAddress;
    Wt::WComboBox * _i2cDeviceList; // the I2C address
    Wt::WComboBox * _i2cType;
    Wt::WSpinBox * _i2cPort;

public:
    ConfigureI2cWebDialog(VariantPropertyTree &c);
    void setup();
    void onOk();
    void onI2cTypeSet();
};
}
#endif // CONFIGUREI2CWEBDIALOG_H
