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
#ifndef WEBSYSTEMPROPERTIESDIALOG_H
#define WEBSYSTEMPROPERTIESDIALOG_H

#include <Common/Web/webdialog.h>
#include <Wt/WLabel.h>
#include <Wt/WText.h>
#include <Wt/WCheckBox.h>
#include <Wt/WTextEdit.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>

namespace MRL
{
/*!
 * \brief The WebSystemPropertiesDialog class
 * Web version of basic system properties dialog
 *
 */
class WebSystemPropertiesDialog : public WebDialogBase
{
    //
    Wt::WLineEdit * _siteName = nullptr;
    Wt::WCheckBox * _enableGui = nullptr;
    Wt::WCheckBox * _enableWeb = nullptr;
    Wt::WCheckBox * _enableOpc = nullptr;
    Wt::WPushButton * _ok = nullptr;
    Wt::WPushButton * _cancel = nullptr;
    //
public:
    WebSystemPropertiesDialog();
    void setup();
    void onOk();

};
}
#endif // WEBSYSTEMPROPERTIESDIALOG_H
