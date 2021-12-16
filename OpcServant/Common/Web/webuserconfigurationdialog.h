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
#ifndef WEBUSERCONFIGURATIONDIALOG_H
#define WEBUSERCONFIGURATIONDIALOG_H
#include <Common/Web/webdialog.h>

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WGridLayout.h>
#include <Wt/WToolBar.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTree.h>
#include <Wt/WIconPair.h>
#include <Wt/WTreeNode.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WPushButton.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WDateEdit.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WDialog.h>
#include <Wt/WLink.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WComboBox.h>
#include <Wt/WCheckBox.h>
#include <Common/Web/webdatamodel.h>
#include <Common/Web/webaddobjectdialog.h>


namespace MRL
{
    class WebUserConfigurationDialog : public WebDialogBase
{
    Wt::WComboBox * _list = nullptr;
    Wt::WLineEdit * _password = nullptr;
    Wt::WCheckBox * _admin = nullptr;
    Wt::WCheckBox * _enabled = nullptr;
    //
    Wt::WPushButton * _add = nullptr;
    Wt::WPushButton * _edit = nullptr;
    Wt::WPushButton * _remove = nullptr;
    Wt::WToolBar *  _toolBar = nullptr;



    MRL::VariantPropertyTree _settings;
public:
    WebUserConfigurationDialog();
    virtual void setup() ;
    virtual void onOk();
    void add();
    void apply();
    void remove();
    void changed();
};
}
#endif // WEBUSERCONFIGURATIONDIALOG_H
