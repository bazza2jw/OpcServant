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
#ifndef I2CCONFIGURATIONWEBDIALOG_H
#define I2CCONFIGURATIONWEBDIALOG_H
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>
#include <Common/Web/configurationwebdialog.h>

namespace MRL
{
/*!
   * \brief The i2cConfigurationWebForm class
   */
  class i2cConfigurationWebForm : public ConfigurationWebForm
  {
      Wt::WPushButton * _configButton = nullptr;
  public:
      i2cConfigurationWebForm(const PropertyPath &path) : ConfigurationWebForm(path){}
      void configI2C();
      virtual int addFields(Wt::WGridLayout *, int row = 0);

  };

  // the web form dialog wrapper

}
#endif // I2CCONFIGURATIONWEBDIALOG_H
