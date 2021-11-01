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
#include "sbcsupportobjectmanager.h"
#include "sbcsupportobjectmanager.h"
#include "SbcSupportSetupDialog.h"
#include "SbcSupportTabPanel.h"
//#include "SbcSupportconfigurationwebdialog.h"
#include <Common/Web/webdialog.h>
#include "sbcsupportrt.h"
#include <Common/Daq/daqcommon.h>
#include <Common/Opc/opcserver.h>

/*!
    \brief MRL::SbcSupportObjectManager::SbcSupportObjectManager
*/
MRL::SbcSupportObjectManager::SbcSupportObjectManager() : ObjectManager("SbcSupport") {
}

/*!
    \brief MRL::SbcSupportObjectManager::createRT
    \param path
    \return
*/
bool MRL::SbcSupportObjectManager::createRT(const PropertyPath &path) {
    if (ObjectManager::createRT(path)) {
        return true;
    }
    return false;
}

/*!
    \brief MRL::SbcSupportObjectManager::create
    \param path
    \return
*/
bool MRL::SbcSupportObjectManager::create(const PropertyPath &path) {
    if (ObjectManager::create(path)) {

        return true;
    }
    return false;
}

/*!
    \brief MRL::SbcSupportObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::SbcSupportObjectManager::edit(wxWindow *w, const PropertyPath &s) {
    SbcSupportSetupDialog dlg(w,s);
    return dlg.ShowModal();
}
/*!
    \brief MRL::SbcSupportObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::SbcSupportObjectManager::edit(Wt::WWidget *w, const PropertyPath &path) {
    //WebDialogBase::showDialog<SbcSupportConfigurationWebForm>(w, path);
    return true;
}

/*!
    \brief MRL::SbcSupportObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::SbcSupportObjectManager::view(wxWindow */*parent*/, const PropertyPath &/*s*/) {
    return false;
}
/*!
    \brief MRL::SbcSupportObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::SbcSupportObjectManager::view(Wt::WWidget */*parent*/, const PropertyPath &/*s*/) {
    return false;
}

/*!
    \brief MRL::SbcSupportObjectManager::update
    \param s
    \return
*/
bool MRL::SbcSupportObjectManager::update(const PropertyPath &/*s*/) { // updated as an object so includes all children
    return false;
}

/*!
 * \brief MRL::SbcSupportObjectManager::createRtObject
 * \param id
 * \param r
 * \return true on success
 */
bool MRL::SbcSupportObjectManager::createRtObject(unsigned id, RtObjectRef &r)
{
   wxLogDebug("SbcSupportObjectManager::createRtObject id %d",id);
   RtObjectRef o( new SbcSupportRT(id));
   r = std::move(o); // assign across
   return true;
}

/*!
* \brief createTabWindow
* \param id
* \return  window for tab - similar to UI but without navigator
*/
wxWindow * MRL::SbcSupportObjectManager::createTabWindow(wxWindow *parent, unsigned id)
{
   return new SbcSupportTabPanel(parent,id);
}

