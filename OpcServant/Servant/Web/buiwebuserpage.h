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
#ifndef BUIWEBUSERPAGE_H
#define BUIWEBUSERPAGE_H

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

namespace MRL
{
//
// The user tab display
// query each object for a tab display and add to the tab.
//
/*!
 * \brief The BuiWebUserPage class
 */
class BuiWebUserPage  : public Wt::WContainerWidget
{
    Wt::WTabWidget * _tab = nullptr;
    Wt::WToolBar *  _toolBar = nullptr;
public:
    BuiWebUserPage();
    ~BuiWebUserPage();
    //
    // accessors
    //
    Wt::WTabWidget * tab() {
        return _tab;
    }
    Wt::WToolBar *  toolBar() {
        return _toolBar;
    }

    /*!
     * \brief createColorButton
     * \param className
     * \param text
     * \return created button
     */

    void createColorButton(const char *className, const Wt::WString& text)
    {
        auto button = std::make_unique<Wt::WPushButton>();
        button->setTextFormat(Wt::TextFormat::XHTML);
        button->setText(text);
        button->addStyleClass(className);
        _toolBar->addButton(std::move(button));
    }

};
}
#endif // BUIWEBUSERPAGE_H
