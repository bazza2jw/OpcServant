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
#include "ICPCON7017SetupDialog.h"
#include "icpcon7017objectmanager.h"

/*!
 * \brief ICPCON7017SetupDialog::ICPCON7017SetupDialog
 * \param parent
 * \param path
 */
ICPCON7017SetupDialog::ICPCON7017SetupDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : SetupNodeSerial(parent,path)
{
    _panel = new ScaleOffsetConfigurePanel(GetNotebook(),7);
    // initialise the map
    _panel->map()["IN0"] = MRL::ScaleOffset(1.0,0.0);
    _panel->map()["IN1"] = MRL::ScaleOffset(1.0,0.0);
    _panel->map()["IN2"] = MRL::ScaleOffset(1.0,0.0);
    _panel->map()["IN3"] = MRL::ScaleOffset(1.0,0.0);
    _panel->map()["IN4"] = MRL::ScaleOffset(1.0,0.0);
    _panel->map()["IN5"] = MRL::ScaleOffset(1.0,0.0);
    _panel->map()["IN6"] = MRL::ScaleOffset(1.0,0.0);
    GetNotebook()->AddPage(_panel,_("Scale Offset"));
}


