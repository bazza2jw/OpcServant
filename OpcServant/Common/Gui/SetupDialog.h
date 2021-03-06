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
#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H
#include "BourbonCommonUI.h"
#include <Common/common.h>
#include <Common/Gui/I2cConfigurePanel.h>
#include <Common/Gui/SerialConfigurePanel.h>
#include <Common/Gui/DatabaseConfigurePanel.h>
#include <Common/Gui/NetworkConfigurePanel.h>

/*!
 * \brief The SetupDialog class
 * Standard configuration dialogues.
 */
class SetupDialog : public SetupDialogBase
{
    const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree _configuration; // tree of configuration values
    MRL::PropertyPath _dataPath; // usuall empty but can be a sub directory in the configuration tree
public:
    SetupDialog(wxWindow* parent, const MRL::PropertyPath &path, const MRL::PropertyPath &dataPath = MRL::PropertyPath() );
    virtual ~SetupDialog();
    virtual int addFields(wxFlexGridSizer *, int row = 0) { return row;}
    virtual void setFields(MRL::PropertyPath &p);
    virtual void getFields(MRL::PropertyPath &p);
    //
    const MRL::PropertyPath &path() { return  _path;} // path to object in configuration and elsewhere
    MRL::VariantPropertyTree &configuration() { return  _configuration;} // tree of configuration values
    MRL::PropertyPath & dataPath() { return _dataPath;} // usually empty but can be a sub directory in the configuration tree
    //
protected:
    virtual void onOk(wxCommandEvent& event);
};



/*!
 * \brief The SetupI2cDialog class
 */
template <typename T>
class SetupBaseDialog : public SetupDialog
{
    T * _panel = nullptr;
public:
    /*!
     * \brief SetupI2cDialog
     * \param parent
     * \param path
     */
    SetupBaseDialog(wxWindow* parent,
                    const MRL::PropertyPath &path,
                    const MRL::PropertyPath &data = MRL::PropertyPath())
        : SetupDialog(parent,path,data)
    {
        _panel = new T(GetNotebook());
        GetNotebook()->AddPage(_panel,_("I2C Setup"));
        _panel->read(configuration(), dataPath());
    }
    /*!
     * \brief setFields
     */
    virtual void setFields(MRL::PropertyPath &p)
    {
      _panel->read(configuration(), p);
      SetupDialog::setFields(p);
    }
    /*!
     * \brief getFields
     */
    virtual void getFields(MRL::PropertyPath &p)
    {
      _panel->write(configuration(),p);
      SetupDialog::getFields(p);
    }
    T * panel() const { return _panel;}
};


typedef SetupBaseDialog<I2cConfigurePanel> SetupI2cDialog;
typedef SetupBaseDialog<NetworkConfigurePanel> SetupNetworkDialog;
typedef SetupBaseDialog<DatabaseConfigurePanel> SetupDatabaseDialog;
typedef SetupBaseDialog<SerialConfigurePanel> SetupSerialDialog;
/*!
 * \brief The SetupNodeSerial class
 */
class SetupNodeSerial : public SetupSerialDialog
{
public:
    SetupNodeSerial(wxWindow* parent,
                    const MRL::PropertyPath &path,
                    const MRL::PropertyPath &dataPath = MRL::PropertyPath())
        : SetupSerialDialog(parent,path,dataPath)
    {
        panel()->showNodeAddress();
    }
};

#endif // SETUPDIALOG_H
