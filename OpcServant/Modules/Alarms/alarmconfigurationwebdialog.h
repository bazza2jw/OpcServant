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
#ifndef ALARMCONFIGURATIONWEBDIALOG_H
#define ALARMCONFIGURATIONWEBDIALOG_H

/*!
 * \brief The AlarmConfigurationWebDialog class
 */
class AlarmConfigurationWebDialog {
    public:
        AlarmConfigurationWebDialog();
};


#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WSelectionBox.h>
#include "alarmrt.h"
namespace MRL {
    class AlarmConfigurationWebDialog : public WebDialogBase {
            PropertyPath _path;
        public:
            Wt::WSpinBox   *_nodeAddress = nullptr;
            Wt::WSpinBox   *_publishInterval = nullptr;
            Wt::WSpinBox   *_measureInterval = nullptr;
            Wt::WCheckBox *_enabled = nullptr;
            Wt::WSelectionBox *_groupList = nullptr;
            //
            // object references
            Wt::WComboBox   *_actionObject  = nullptr;
            Wt::WComboBox    *_alertObject  = nullptr;
            Wt::WComboBox    *_okObject  = nullptr;
            //
            // Outputs to use
            Wt::WComboBox    *_actionOutput = nullptr;
            Wt::WComboBox    *_alertOutput = nullptr;
            Wt::WComboBox    *_okOutput = nullptr;
            //
            AlarmConfigurationWebDialog(const PropertyPath &path) : _path(path) {}
            void setup();
            void onOk();
            void setOutputs(const std::string &s, Wt::WComboBox *c);
            void OnActionObject(int i);
            void OnAlertObject(int i);
            void OnOkObject(int i);
    };
}


#endif // ALARMCONFIGURATIONWEBDIALOG_H
