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
#include "EnviroPlusobjectmanager.h"
#include <Common/Web/webdialog.h>
#include "EnviroPlusrt.h"
#include <Common/Daq/daqcommon.h>
#include <Common/Opc/opcserver.h>
#include <Common/Web/configurei2cwebdialog.h>
#include <Common/Web/i2cconfigurationwebdialog.h>
#include <Common/Gui/i2csetupdialog.h>

/*!
    \brief MRL::EnviroPlusObjectManager::EnviroPlusObjectManager
*/
MRL::EnviroPlusObjectManager::EnviroPlusObjectManager() : ObjectManager("EnviroPlus") {
}

/*!
    \brief MRL::EnviroPlusObjectManager::createRT
    \param path
    \return
*/
bool MRL::EnviroPlusObjectManager::createRT(const PropertyPath &path) {
    if (ObjectManager::createRT(path)) {
        return true;
    }
    return false;
}

/*!
    \brief MRL::EnviroPlusObjectManager::create
    \param path
    \return
*/
bool MRL::EnviroPlusObjectManager::create(const PropertyPath &path) {
    if (ObjectManager::create(path)) {

        return true;
    }
    return false;
}

/*!
    \brief MRL::EnviroPlusObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::EnviroPlusObjectManager::edit(wxWindow *w, const PropertyPath &s) {
    I2cSetupDialog dlg(w,s);
    return dlg.ShowModal() == wxID_OK;
}
/*!
    \brief MRL::EnviroPlusObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::EnviroPlusObjectManager::edit(Wt::WWidget *w, const PropertyPath &path) {
    WebDialogBase::showDialog<i2cConfigurationWebForm>(w, path);
    return true;
}

/*!
    \brief MRL::EnviroPlusObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::EnviroPlusObjectManager::view(wxWindow */*parent*/, const PropertyPath &/*s*/) {
    return false;
}
/*!
    \brief MRL::EnviroPlusObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::EnviroPlusObjectManager::view(Wt::WWidget */*parent*/, const PropertyPath &/*s*/) {
    return false;
}

/*!
    \brief MRL::EnviroPlusObjectManager::update
    \param s
    \return
*/
bool MRL::EnviroPlusObjectManager::update(const PropertyPath &/*s*/) { // updated as an object so includes all children
    return false;
}

/*!
 * \brief MRL::EnviroPlusObjectManager::createRtObject
 * \param id
 * \param r
 * \return true on success
 */
bool MRL::EnviroPlusObjectManager::createRtObject(unsigned id, RtObjectRef &r)
{
   wxLogDebug("EnviroPlusObjectManager::createRtObject id %d",id);
   RtObjectRef o( new EnviroPlusRT(id));
   r = std::move(o); // assign across
   return true;
}





