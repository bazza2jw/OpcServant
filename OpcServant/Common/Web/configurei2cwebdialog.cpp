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
#include "configurei2cwebdialog.h"
/*!
 * \brief MRL::ConfigureI2cWebDialog::ConfigureI2cWebDialog
 * \param c
 */
MRL::ConfigureI2cWebDialog::ConfigureI2cWebDialog( MRL::VariantPropertyTree &c) : _configuration(c)
{

}

/*!
 * \brief MRL::ConfigureI2cWebDialog::setup
 */
void MRL::ConfigureI2cWebDialog::setup()
{
    setWindowTitle("I2C Configuration");
    auto layout = contents()->setLayout(std::make_unique<Wt::WVBoxLayout>());
    //
    std::unique_ptr<Wt::WGridLayout> grid = std::make_unique<Wt::WGridLayout>();
    Wt::WLength w(20,Wt::LengthUnit::FontEm); // combo box size
    Wt::WLength sw(10,Wt::LengthUnit::FontEm); // spin box size
    //
    int row = 0;
    //
    grid->addWidget(std::make_unique<Wt::WLabel>("I2C Type"),row,0);
    _i2cType = grid->addWidget(std::make_unique<Wt::WComboBox>(),row++,1);
    _i2cType->setMaximumSize(w,_i2cType->maximumHeight());
    //
    grid->addWidget(std::make_unique<Wt::WLabel>("I2C Address"),row,0);
    _i2cAddress = grid->addWidget(std::make_unique<Wt::WLineEdit>(),row++,1);
    _i2cDeviceList = grid->addWidget(std::make_unique<Wt::WComboBox>(),row++,1);
    //
    grid->addWidget(std::make_unique<Wt::WLabel>("I2C Port"),row,0);
    _i2cPort = grid->addWidget(std::make_unique<Wt::WSpinBox>(),row++,1);
    _i2cPort->setMaximumSize(sw,_i2cPort->maximumHeight());
    _i2cPort->setMaximum(32000);
    _i2cPort->setMinimum(2000);
    _i2cPort->setValue(5000);
    //
    layout->addLayout(std::move(grid));
        // now load the fields
        // get the configuration
        _i2cType->setCurrentIndex(_configuration.getInt("/I2CType"));
        _i2cAddress->setText(_configuration.getString("/I2CAddress"));
        _i2cPort->setValue(_configuration.getInt("/I2CPort"));

    WebDialogBase::setup();
}

/*!
 * \brief MRL::ConfigureI2cWebDialog::onI2cTypeSet
 */
void MRL::ConfigureI2cWebDialog::onI2cTypeSet()
{
        // Local
        _i2cDeviceList->clear();
        // look for i2c
        wxDir d("/dev");
        if(d.IsOpened())
        {
            wxString f;
            if(d.GetFirst (&f, "*i2c*", wxDIR_FILES ))
            {
                do
                {
                    std::string s = f.ToStdString();
                    _i2cDeviceList->addItem(s);
                } while(d.GetNext(&f));
            }
        }
}

/*!
 * \brief MRL::ConfigureI2cWebDialog::onOk
 */
void MRL::ConfigureI2cWebDialog::onOk()
{
    _configuration.setInt("/I2CType",_i2cType->currentIndex());
    _configuration.setString("/I2CAddress",_i2cAddress->text().toUTF8());
    _configuration.setInt("/I2CPort",_i2cPort->value());
}

