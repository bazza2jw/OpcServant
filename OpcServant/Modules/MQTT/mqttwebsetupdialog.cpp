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
#include "mqttwebsetupdialog.h"
#include <Wt/WGridLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WCheckBox.h>
/*!
 * \brief MRL::MqttSetupDialog::setup
 */
void MRL::MqttWebSetupDialog::setup()
{
  setWindowTitle("MQTT Configuration");
  auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
  layout->addWidget(std::make_unique<Wt::WLabel>("Host"), 0, 0);
  _host = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
  layout->addWidget(std::make_unique<Wt::WLabel>("Username"), 1, 0);
  _username = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 1, 1);
  layout->addWidget(std::make_unique<Wt::WLabel>("Password"), 2, 0);
  _password = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 2, 1);

  layout->addWidget(std::make_unique<Wt::WLabel>("Ident"), 3, 0);
  _ident = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 3, 1);
  layout->addWidget(std::make_unique<Wt::WLabel>("Filter"), 4, 0);
  _filter = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 4, 1);
  layout->addWidget(std::make_unique<Wt::WLabel>("Port"), 5, 0);
  _port = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 5, 1);
  _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enabled"), 6, 1);
  //
  _port->setMaximum(10000);
  _port->setMinimum(2000);
  _port->setValue(1883);
  //
  Json::Value v;
  if (MRL::Common::configuration().getData(_path, v)) {
      // do not always have to convert to tree - esp if simple property list
      Json::Value &ht = v["Ident"];
      Json::Value &db = v["Filter"];
      Json::Value &pt = v["Port"];
      Json::Value &hs = v["Host"];
      Json::Value &us = v["Username"];
      Json::Value &ps = v["Password"];
      Json::Value &ei = v["Enabled"];
      //
      _host->setText(hs.asString());
      _username->setText(us.asString());
      _password->setText(ps.asString());
      _ident->setText(ht.asString());
      _filter->setText(db.asString());
      _port->setValue(pt.asInt());
      _enabled->setCheckState(ei.asBool()?Wt::CheckState::Checked:Wt::CheckState::Unchecked);
  }
  else
  {
      _ident->setText("OPCSERVANT");
      _filter->setText("*");
      _port->setValue(1883);
      _enabled->setCheckState(Wt::CheckState::Checked);
  }
  WebDialogBase::setup();
}
/*!
 * \brief MRL::MqttWebSetupDialog::onOk
 */
void MRL::MqttWebSetupDialog::onOk()
{
    Json::Value v;
    v["Ident"]       = _ident->text().toUTF8();
    v["Filter"]   = _filter->text().toUTF8();
    v["Port"]       = _port->value();
    v["Host"]       = _host->text().toUTF8();
    v["Username"]   = _username->text().toUTF8();
    v["Password"]   = _password->text().toUTF8();
    v["Enabled"]    = (_enabled->checkState() == Wt::CheckState::Checked);
    MRL::Common::configuration().updateData(_path, v);
}
