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
#include "I2CADCobjectmanager.h"
#include <Common/Web/webdialog.h>
#include "I2CADCrt.h"
#include <Common/Daq/daqcommon.h>
#include <Common/Opc/opcserver.h>
#include <Common/Web/configurei2cwebdialog.h>
#include <Common/Web/i2cconfigurationwebdialog.h>
#include <Common/Gui/i2csetupdialog.h>

/*!
    \brief MRL::I2CADCObjectManager::I2CADCObjectManager
*/
MRL::I2CADCObjectManager::I2CADCObjectManager() : ObjectManager("I2CADC") {
}

/*!
    \brief MRL::I2CADCObjectManager::createRT
    \param path
    \return
*/
bool MRL::I2CADCObjectManager::createRT(const PropertyPath &path) {
    if (ObjectManager::createRT(path)) {
        return true;
    }
    return false;
}

/*!
    \brief MRL::I2CADCObjectManager::create
    \param path
    \return
*/
bool MRL::I2CADCObjectManager::create(const PropertyPath &path) {
    if (ObjectManager::create(path)) {

        return true;
    }
    return false;
}

/*!
    \brief MRL::I2CADCObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::I2CADCObjectManager::edit(wxWindow *w, const PropertyPath &s) {
    I2cSetupDialog dlg(w,s);
    return dlg.ShowModal() == wxID_OK;
}
/*!
    \brief MRL::I2CADCObjectManager::edit
    \param w
    \param s
    \return
*/
bool MRL::I2CADCObjectManager::edit(Wt::WWidget *w, const PropertyPath &path) {
    WebDialogBase::showDialog<i2cConfigurationWebForm>(w, path);
    return true;
}

/*!
    \brief MRL::I2CADCObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::I2CADCObjectManager::view(wxWindow */*parent*/, const PropertyPath &/*s*/) {
    return false;
}
/*!
    \brief MRL::I2CADCObjectManager::view
    \param parent
    \param s
    \return
*/
bool MRL::I2CADCObjectManager::view(Wt::WWidget */*parent*/, const PropertyPath &/*s*/) {
    return false;
}

/*!
    \brief MRL::I2CADCObjectManager::update
    \param s
    \return
*/
bool MRL::I2CADCObjectManager::update(const PropertyPath &/*s*/) { // updated as an object so includes all children
    return false;
}

/*!
 * \brief MRL::I2CADCObjectManager::createRtObject
 * \param id
 * \param r
 * \return true on success
 */
bool MRL::I2CADCObjectManager::createRtObject(unsigned id, RtObjectRef &r)
{
   wxLogDebug("I2CADCObjectManager::createRtObject id %d",id);
   RtObjectRef o( new I2CADCRT(id));
   r = std::move(o); // assign across
   return true;
}





