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
#include "i2cconfigurationwebdialog.h"
#include "configurei2cwebdialog.h"
#include <Wt/WGridLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WCheckBox.h>


/*!
 * \brief MRL::i2cConfigurationWebForm::addFields
 * \param layout
 * \return next row to use
 */
int MRL::i2cConfigurationWebForm::addFields(Wt::WGridLayout *layout, int row)
{
    row = ConfigurationWebForm::addFields(layout,row);
    _configButton = layout->addWidget(std::make_unique<Wt::WPushButton>("I2C Config ..."),row++,0);
    _configButton->clicked().connect(this,&MRL::i2cConfigurationWebForm::configI2C);
    return row;
}

/*!
 * \brief MRL::i2cConfigurationWebForm::configI2C
 */
void MRL::i2cConfigurationWebForm::configI2C()
{
    ConfigureI2cWebDialog dlg(configuration());
    dlg.doExec();
}


