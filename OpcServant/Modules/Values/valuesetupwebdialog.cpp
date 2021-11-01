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
#include "valuesetupwebdialog.h"
#include <Wt/WGridLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WCheckBox.h>
#include <Common/Daq/daqcommon.h>


/*!
    \brief SimulatorConfigurationWebForm
    \param path
    \return
*/

// item names for easy read/write
static const char *al[] = {"HiHi","HiLo","LoHi","LoLo"};
static const char *el[] = {"HiHiEnabled","HiLoEnabled","LoHiEnabled","LoLoEnabled"};


void MRL::ValueSetupWebDialog::setup() {

    try {
        Wt::WGridLayout *layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>());  // a bag grid layout
        setWindowTitle("Configure Value");
        layout->setColumnResizable(0, true, Wt::WLength(30, Wt::LengthUnit::Percentage));
        //
        // add the items to the grid
        //
        int row = 0;
        layout->addWidget(std::make_unique<Wt::WLabel>("Object"), row, 0);
        _object = layout->addWidget(std::make_unique<Wt::WComboBox>(), row++, 1);
        //
        layout->addWidget(std::make_unique<Wt::WLabel>("Scale"), row, 0);
        layout->addWidget(std::make_unique<Wt::WDoubleSpinBox>(),row++,1);
        _scale->setMinimum(-100000.0);
        _scale->setMaximum(100000.0);
        _scale->setValue(1.0);
        //
        layout->addWidget(std::make_unique<Wt::WLabel>("Offset"), row, 0);
        layout->addWidget(std::make_unique<Wt::WDoubleSpinBox>(),row++,1);
        _offset->setMinimum(-100000.0);
        _offset->setMaximum(100000.0);
        _offset->setValue(1.0);
        //
        layout->addWidget(std::make_unique<Wt::WLabel>("Alarm Limits"), row++, 0);
        //

        for (int i = 0; i < 4; i++,row++) {
            layout->addWidget(std::make_unique<Wt::WLabel>(al[i]), row, 0);
            _threshold[i] = layout->addWidget(std::make_unique<Wt::WDoubleSpinBox>(), row, 1);
            _threshold[i]->setMinimum(-100000.0);
            _threshold[i]->setMaximum(100000.0);
            _threshold[i]->setValue(0.0);
            _enables[i] = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enabled"), row, 2);
            _threshold[i]->setValue(_configuration.getValue<double>(_path,al[i]));
            _enables[i]->setChecked(_configuration.getValue<bool>(_path,el[i]));
        }
        //
        MRL::StringMap &a = MRL::Common::aliasMap();
        for(auto i = a.begin(); i != a.end(); i++)
        {
            _object->addItem(i->first);
        }
        //
        int j = _object->findText(_configuration.getValue<std::string>(_path,"Object"));
        _object->setCurrentIndex(j);
        //
        _scale->setValue(_configuration.getValue<double>(_path,"Scale"));
        _offset->setValue(_configuration.getValue<double>(_path,"Offset"));
        //
        //
        WebDialogBase::setup();
    }
    catch (...) {
        std::cerr << "Exception" << __FUNCTION__ << std::endl;
    }
}
/*!
 * \brief MRL::ValueSetupWebDialog::onOk
 */
void MRL::ValueSetupWebDialog::onOk() {
    //
    std::string s = _object->currentText().toUTF8();
    _configuration.setValue(_path,"Object",s);
    _configuration.setValue(_path,"Scale",_scale->value());
    _configuration.setValue(_path,"Offset",_offset->value());
    for(int i = 0; i < 4; i++)
    {
        _configuration.setValue(_path,al[i],_threshold[i]->value());
        _configuration.setValue(_path,el[i], _enables[i]->isChecked());
    }
    //
}


