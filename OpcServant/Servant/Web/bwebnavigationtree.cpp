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
#include "bwebnavigationtree.h"
#include <Wt/WPopupMenu.h>
#include <Wt/WPopupMenuItem.h>
#include <Common/common.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Daq/daq.h>
#include <Common/messageids.h>
#include <Common/Daq/daqcommon.h>

/*!
    \brief MRL::BWebNavigationTree::BWebNavigationTree
*/
MRL::BWebNavigationTree::BWebNavigationTree() : Wt::WTreeView(), _handler(*this) {

    setAttributeValue("oncontextmenu",
                      "event.cancelBubble = true; event.returnValue = false; return false;");
    setModel(Common::display().webStore());  // set the model shared by all instances
    setSelectionMode(Wt::SelectionMode::Single); // single selection
    Wt::WFlags< Wt::EditTrigger > editTriggers;
    setEditTriggers(editTriggers);
    setRowHeight(34);
    // do the connections
    mouseWentUp().connect(this, &MRL::BWebNavigationTree::selected);
    clicked().connect(this, &MRL::BWebNavigationTree::selected);
    doubleClicked().connect(this, &MRL::BWebNavigationTree::selectedDoubleClicked);
    //
    _timer = addChild(std::make_unique<Wt::WTimer>());
    _timer->timeout().connect(this, &BWebNavigationTree::onTimer);
    _timer->setInterval(std::chrono::seconds(1));
    _timer->start();
    //
    _handler.join(Daq::instance()); // connect for DAQ message

}

/*!
    \brief MRL::BWebNavigationTree::selectedDoubleClicked
    \param i
    \param e
*/
void MRL::BWebNavigationTree::selectedDoubleClicked(Wt::WModelIndex i, Wt::WMouseEvent e) {
    _currentItem = Common::display().webStore()->itemFromIndex(i);
    _currentPath.clear();
    if (_currentItem) {
        _currentPosition = Wt::WPoint(e.document().x,e.document().y);
        doMenu();
    }
}
/*!
    \brief MRL::BWebNavigationTree::doMenu
*/
void MRL::BWebNavigationTree::doMenu() {

    if (_currentItem) {

        // now translate to path
        Common::display().findDataItemPath(_currentItem, _currentPath);
        unsigned t = MRL::Common::configuration().typeId(_currentPath); // get item type
        _handler._currentId = MRL::Common::configuration().find(_currentPath);

        auto o = MRL::ObjectManager::find(t);

        if (o) {
            Wt::WPopupMenu m;
            o->buildMenu(&m, _currentPath);
            Wt::WMenuItem *i = m.exec(_currentPosition);
            if (i) {
                union {
                    void *pv;
                    int a;
                };
                pv = i->data();
                switch (a) {
                case MRL::ObjectManagerAdd: {
                    // Add a child item to the selected item
                    unsigned t = MRL::Common::configuration().typeId(_currentPath);
                    auto o = MRL::ObjectManager::find(t); // find the handler by type id
                    if (o) {
                        o->add(this, _currentPath);
                    }
                }
                break;
                case MRL::ObjectManagerDelete: {
                    unsigned t = MRL::Common::configuration().typeId(_currentPath);
                    int id = MRL::Common::configuration().find(_currentPath);
                    //
                    auto o = MRL::ObjectManager::find(t); // find the handler by type id
                    if (o && (id > 0)) {
                        MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                        if (r) {
                            r.get()->stop();
                        }
                        o->remove(_currentPath);
                    }
                }
                break;
                case MRL::ObjectManagerProperties: {
                    unsigned t = MRL::Common::configuration().typeId(_currentPath);
                    auto o = MRL::ObjectManager::find(t); // find the handler by type id
                    if (o) {
                        if (o->edit(this, _currentPath)) {
                            int id = MRL::Common::configuration().find(_currentPath);
                            if (id > 0) {
                                MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
                                if (r) {
                                    r.get()->stop();
                                    r.get()->start();
                                }
                            }
                        }
                    }
                }
                break;
                case MRL::ObjectManagerView: {
                    unsigned t = MRL::Common::configuration().typeId(_currentPath);
                    auto o = MRL::ObjectManager::find(t); // find the handler by type id
                    if (o) {
                        o->view(this, _currentPath);
                    }
                }
                break;

                default:
                    break;
                }
            }
        }
    }
}

/*!
    \brief MRL::BWebNavigationTree::selected
    \param i
    \param e
*/
void MRL::BWebNavigationTree::selected(Wt::WModelIndex i, Wt::WMouseEvent e) {
    // get the path to the item
    if(e.button() == Wt::MouseButton::Right)
    {
        selectedDoubleClicked(i,e);
    }
    else
    {
        _currentItem = Common::display().webStore()->itemFromIndex(i);
        _currentPath.clear();
        //
        if (_currentItem) {
            _currentPosition = Wt::WPoint(e.document().x,e.document().y);
            // now translate to path
            Common::display().findDataItemPath(_currentItem, _currentPath);
            unsigned t = MRL::Common::configuration().typeId(_currentPath); // get item type
            _handler._currentId = MRL::Common::configuration().find(_currentPath);
            auto o = MRL::ObjectManager::find(t);
            if (o) {
                std::string cfg;
                o->writeConfigurationPage(_handler._currentId, cfg);
            }
        }
    }
}




/*!
    \brief MRL::BWebNavigationTree::OnTimer
*/
void MRL::BWebNavigationTree::onTimer() {
    _handler.process();
    MRL::Common::display().processWeb(); // drive web form updates in the correct context
}

/*!
    \brief MRL::WebHandler::processQueueItem
    \return
*/
bool MRL::WebHandler::processQueueItem(const Message &msg) {
    if (!MRL::BObject::processQueueItem(msg)) {
        // notifications from DAQ thread
        switch (msg.id()) {
        case MESSAGEID::Publish_Object:
            break;
        case MESSAGEID::Update_Object: // Object has been updated (measured) in DAQ space - OPC and GUI/Web need to sync
            break;
        case MESSAGEID::Log_Event: // Object has notified of event - typically in and out of fault
            break;
        case MESSAGEID::Diagnostic_Event: // an object has emitted a diagnostic (trace)

            break;
        case MESSAGEID::Audit_Event: // some security related event has happend - login/out or config update

            break;
        case MESSAGEID::Add_Object: // model changes
            break;
        case MESSAGEID::Remove_Object: // model changes
            break;
        default:
            break;
        }
    }
    return true;
}



