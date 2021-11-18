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
#ifndef ICPCON7017SETUPDIALOG_H
#define ICPCON7017SETUPDIALOG_H
#include <Common/common.h>
#include <Common/Gui/SetupDialog.h>
#include <Common/Gui/ScaleOffsetConfigurePanel.h>
/*!
 * \brief The ICPCON7017SetupDialog class
 */
class ICPCON7017SetupDialog :public SetupNodeSerial
{
    ScaleOffsetConfigurePanel * _panel = nullptr;
public:
    ICPCON7017SetupDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~ICPCON7017SetupDialog(){}
    /*!
     * \brief setFields
     * \param p
     */
    virtual void setFields(MRL::PropertyPath &p)
    {
        SetupNodeSerial::setFields(p);
        p.push_back("ScaleOffset");
        _panel->map().read(configuration(),p);
    }
    /*!
     * \brief getFields
     * \param p
     */
    virtual void getFields(MRL::PropertyPath &p)
    {
        SetupNodeSerial::getFields(p);
        _panel->map().write(configuration(),p);
    }
};
#endif // ICPCON7017SETUPDIALOG_H
