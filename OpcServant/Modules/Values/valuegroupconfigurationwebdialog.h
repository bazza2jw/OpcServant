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
#ifndef VALUEGROUPCONFIGURATIONWEBDIALOG_H
#define VALUEGROUPCONFIGURATIONWEBDIALOG_H

#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/Web/webdialog.h>
#include <Wt/WDoubleSpinBox.h>
#include "valuert.h"
#include <MrlLib/stats.hpp>
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


namespace MRL {
/*!
 * \brief The ValueGroupConfigurationWebDialog class
 */
class ValueGroupConfigurationWebDialog : public WebDialogBase
{
    PropertyPath _path;
    unsigned _id = 0;
    MRL::VariantPropertyTree _configuration; // tree of configuration values

    Wt::WToolBar *  _toolBar = nullptr;


    Wt::WSelectionBox * _itemList = nullptr;
    Wt::WSpinBox  * _publishInterval = nullptr;
    Wt::WSpinBox  * _measureInterval = nullptr;
    Wt::WCheckBox * _enabled = nullptr;
    //
    Wt::WPushButton * _add    = nullptr;
    Wt::WPushButton * _edit   = nullptr;
    Wt::WPushButton * _remove = nullptr;
    Wt::WPushButton * _alias  = nullptr;
    Wt::WCheckBox * _enableTabView = nullptr;

    Wt::WPushButton * createColorButton(const char *className, const Wt::WString& text)
    {
        Wt::WPushButton *b = nullptr;
        auto button = std::make_unique<Wt::WPushButton>();
        button->setTextFormat(Wt::TextFormat::XHTML);
        button->setText(text);
        button->addStyleClass(className);
        b = button.get();
        _toolBar->addButton(std::move(button));
        return b;
    }


public:
    ValueGroupConfigurationWebDialog(const PropertyPath &path);
    void setup();
    void onOk();


    void onAdd();
    void onEdit();
    void onRemove();
    void onAlias();

};
}
#endif // VALUEGROUPCONFIGURATIONWEBDIALOG_H
