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
#ifndef BWEBNAVIGATIONTREE_H
#define BWEBNAVIGATIONTREE_H
#include "buiwebapplication.h"
#include <Wt/WTreeView.h>
#include <Common/Web/webdatamodel.h>
#include <Common/displaypropertytree.h>
#include <Wt/WTimer.h>
#include <Common/bobject.h>
#include <Wt/WText.h>
#include <Wt/WPoint.h>
namespace MRL {
    class BWebNavigationTree;
    /*!
     * \brief The WebHandler class
     */
    class WebHandler : public BObject {
            BWebNavigationTree &_page;
        public:
            unsigned _currentId = 0;
            WebHandler(BWebNavigationTree &p) : _page(p) {
                BObject::setEnabled(true);
            }
            bool processQueueItem(const Message &);
    };

    /*!
        \brief The BWebNavigationTree class
    */
    class BWebNavigationTree : public Wt::WTreeView {
            PropertyPath _currentPath;
            Wt::WStandardItem *_currentItem = nullptr;
            Wt::WPoint _currentPosition;
            Wt::WTimer *_timer = nullptr;
            WebHandler _handler;
            //
            //
        public:
            BWebNavigationTree();
            //
            // accesors
            WebHandler & handler() {
                return _handler;
            }

            void doMenu();

    protected:
            void selected(Wt::WModelIndex i, Wt::WMouseEvent e);
            void selectedDoubleClicked(Wt::WModelIndex i, Wt::WMouseEvent e);
            void onTimer();
    };
}
#endif // BWEBNAVIGATIONTREE_H
