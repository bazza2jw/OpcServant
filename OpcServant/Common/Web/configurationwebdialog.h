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
#ifndef CONFIGURATIONWEBDIALOG_H
#define CONFIGURATIONWEBDIALOG_H
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>

namespace MRL
{
  /*!
   * \brief The ConfigurationWebForm class
   * Web based configuration  base class
   */
  class ConfigurationWebForm : public WebDialogBase
  {
      PropertyPath _path;
      MRL::VariantPropertyTree _configuration; // tree of configuration values
  public:
      Wt::WSpinBox  * _publishInterval = nullptr;
      Wt::WSpinBox  * _measureInterval = nullptr;
      Wt::WCheckBox * _enabled = nullptr;
      Wt::WCheckBox * _enableTabView = nullptr;
      ConfigurationWebForm(const PropertyPath &path) : _path(path){}
      void setup();// initialise form
      void onOk(); // handles OK button
      MRL::VariantPropertyTree &configuration() {return  _configuration;} // tree of configuration values
      virtual int addFields(Wt::WGridLayout *, int row = 0);
      virtual void setFields(); // configuration -> fields
      virtual void getFields(); // fields -> configuration
  };

  // the web form dialog wrapper

}
#endif // CONFIGURATIONWEBDIALOG_H
