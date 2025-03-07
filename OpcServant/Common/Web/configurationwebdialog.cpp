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
#include "configurationwebdialog.h"
#include "configurei2cwebdialog.h"
#include <Wt/WGridLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WCheckBox.h>
/*!
 * \brief MRL::ConfigurationWebForm::setup
 * Basic config form for an i2c based device
 * Assume the I2C address is fixed as is timing etc
 */
void MRL::ConfigurationWebForm::setup() {

    setWindowTitle("Configuration");
    auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setHorizontalSpacing(10);
    //
    addFields(layout);
    // add the items to the grid

    Json::Value v;
    if(MRL::Common::configuration().getData(_path,v))
    {
        _configuration.fromJson(v); // load configuration
        setFields();
    }
    WebDialogBase::setup();
}

/*!
 * \brief MRL::ConfigurationWebForm::onOk
 */
void MRL::ConfigurationWebForm::onOk() {
    Json::Value v;
    getFields();
    MRL::Common::configuration().updateData(_path, v);
}

/*!
 * \brief MRL::ConfigurationWebForm::addFields
 * \param layout
 * \return next row to use
 */
int MRL::ConfigurationWebForm::addFields(Wt::WGridLayout *layout, int row)
{
    layout->addWidget(std::make_unique<Wt::WLabel>("Measure Interval(s)"), row, 0);
    _measureInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), row++, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Publish Interval(s)"), row, 0);
    _publishInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), row++, 1);
    _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>(_TR("Enabled")),row++,0);
    _enableTabView = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enable Tab View"),row++,0);
    return row;
}

/*!
 * \brief MRL::ConfigurationWebForm::setFields
 */
void MRL::ConfigurationWebForm::setFields()
{
    _measureInterval->setValue(_configuration.getInt("/MeasureInterval"));
    _publishInterval->setValue(_configuration.getInt("/PublishInterval"));
    _enabled->setChecked(_configuration.getBool("/Enabled"));
    _enableTabView->setChecked(_configuration.getBool("/EnableTabView"));
}

/*!
 * \brief MRL::ConfigurationWebForm::getFields
 * save fields to configuration
 */
void MRL::ConfigurationWebForm::getFields()
{
    _configuration.setInt("/MeasureInterval",_measureInterval->value());
    _configuration.setInt("/PublishInterval",_publishInterval->value());
    _configuration.setBool("/Enabled",_enabled->isChecked());
    _configuration.setBool("/EnableTabView",_enabled->isChecked());

}



