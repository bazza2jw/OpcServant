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
#include "OPCN3webconfiguration.h"
#include <Wt/WSpinBox.h>
#include <Wt/WCheckBox.h>

/*!
 * \brief MRL::LwsWebConfiguration::setup
 */
void MRL::OPCN3WebConfiguration::setup()
{
    setWindowTitle("OPCN3 Configuration");
    auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setHorizontalSpacing(10);
    //
    // add the items to the grid
    int row = 0;
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("SPI Device")), row, 0);
    _address = layout->addWidget(std::make_unique<Wt::WLineEdit>(),row++,1);
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Sample Interval(s)")),row , 0);
    _sampleInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), row++, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Wait Interval(min)")), row, 0);
    _purgeInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), row++, 1);
    _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>(_TR("Enabled")),row,0);
    _enableTabView = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enable Tab View"),row,1);

    //
    _sampleInterval->setMaximum(45);
    _sampleInterval->setMinimum(10);
    _sampleInterval->setValue(30);
    //
    _purgeInterval->setMaximum(10000);
    _purgeInterval->setMinimum(0);
    _purgeInterval->setValue(0);
    //
    Json::Value v;
    if(MRL::Common::configuration().getData(_path,v))
    {
        _configuration.fromJson(v); // load configuration
        //
        _purgeInterval->setValue(_configuration.getInt("WaitInterval"));
        _sampleInterval->setValue(_configuration.getInt("SampleInterval"));
        _address->setText(_configuration.getString("SPIDevice"));
        _enabled->setChecked(_configuration.getBool("Enabled"));
        _enableTabView->setChecked(_configuration.getBool("EnableTabView"));
    }

     WebDialogBase::setup();
}

/*!
 * \brief MRL::LwsWebConfiguration::onOk
 */
void  MRL::OPCN3WebConfiguration::onOk()
{
    _configuration.setInt("WaitInterval",_purgeInterval->value());
    _configuration.setInt("SampleInterval",_sampleInterval->value());
    _configuration.setString("SPIDevice",_address->text().toUTF8());
    _configuration.setBool("Enabled",_enabled->isChecked());
    _configuration.setBool("EnableTabView",_enableTabView->isChecked());

    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);

}
