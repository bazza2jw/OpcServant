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
#include "simulatorconfigurationwebdialog.h"
#include <Wt/WGridLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WPushButton.h>
/*!
    \brief SimulatorConfigurationWebForm
    \param path
    \return
*/
void MRL::SimulatorConfigurationWebForm::setup() {

    setWindowTitle("Simulator Configuration");
    auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    layout->setHorizontalSpacing(10);

    // add the items to the grid
    layout->addWidget(std::make_unique<Wt::WLabel>("Measure Interval(s)"), 0, 0);
    _measureInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 0, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Publish Interval(s)"), 1, 0);
    _publishInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 1, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Type"), 2, 0);
    _type = layout->addWidget(std::make_unique<Wt::WComboBox>(), 2, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Range"), 3, 0);
    _range = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 3, 1);

    _type->addItem("Random");
    _type->addItem("Ramp");
    //
    _range->setMaximum(100);
    _range->setMinimum(10);
    _range->setValue(30);
    //
    Json::Value v;
    if (MRL::Common::configuration().getData(_path, v)) {
        // do not always have to convert to tree - esp if simple property list
        Json::Value &tv = v["Type"];
        Json::Value &rv = v["Range"];
        Json::Value &mi = v["MeasureInterval"];
        Json::Value &pi = v["PublishInterval"];
        //
        _type->setCurrentIndex((tv.asString() == "Ramp") ? 1 : 0);
        _range->setValue(rv.asInt());
        _measureInterval->setValue(mi.asInt());
        _publishInterval->setValue(pi.asInt());
    }
    WebDialogBase::setup();
}

/*!
    \brief MRL::SimulatorConfigurationWebForm::OnOk
*/
void MRL::SimulatorConfigurationWebForm::onOk() {
    Json::Value v;
    v["Type"] = _type->currentText().toUTF8();
    v["Range"] = _range->value();
    v["MeasureInterval"] = _measureInterval->value();
    v["PublishInterval"] = _publishInterval->value();
    MRL::Common::configuration().updateData(_path, v);
}

