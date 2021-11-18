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
#ifndef VALUESETUPWEBDIALOG_H
#define VALUESETUPWEBDIALOG_H
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>
#include <Wt/WDoubleSpinBox.h>
#include "valuert.h"
#include <MrlLib/stats.hpp>

namespace MRL {
/*!
     * \brief The ValueSetupWebDialog class
     */
    class ValueSetupWebDialog : public WebDialogBase
    {
        PropertyPath _path;

        Wt::WComboBox * _object = nullptr;
        //
        // Scale factors
        Wt::WDoubleSpinBox *_scale = nullptr;
        Wt::WDoubleSpinBox *_offset = nullptr;

        //
        Wt::WDoubleSpinBox * _threshold[StatisticsThreshold::NumberThresholds];
        // Offsets
        Wt::WCheckBox * _enables[StatisticsThreshold::NumberThresholds];
        //
        MRL::VariantPropertyTree &_configuration;
        //
    public:
        ValueSetupWebDialog( const PropertyPath &path,MRL::VariantPropertyTree &c) : _path(path),_configuration(c){}
        void setup();
        void onOk();
    };
}


#endif // VALUESETUPWEBDIALOG_H
