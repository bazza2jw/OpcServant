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
#ifndef SIMULATORCONFIGURATIONWEBDIALOG_H
#define SIMULATORCONFIGURATIONWEBDIALOG_H
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>

namespace MRL
{
  // entry form
/*!
   * \brief The SimulatorConfigurationWebForm class
   */
  class SimulatorConfigurationWebForm : public WebDialogBase
  {
      PropertyPath _path;
  public:
      Wt::WComboBox * _type = nullptr;
      Wt::WSpinBox  * _range = nullptr;
      Wt::WSpinBox  * _publishInterval = nullptr;
      Wt::WSpinBox  * _measureInterval = nullptr;

      SimulatorConfigurationWebForm(const PropertyPath &path) : _path(path){}
      void setup();
      void onOk();
  };

  // the web form dialog wrapper

}
#endif // SIMULATORCONFIGURATIONWEBDIALOG_H
