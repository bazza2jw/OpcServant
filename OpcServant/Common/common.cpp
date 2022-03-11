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
#include "common.h"
#include <Common/Daq/daqcommon.h>
#include <Common/Opc/opccommon.h>
#include <Common/objectmanager.h>
#include "Gui/BourbonCommonUI.h"
#include <MrlLib/luascript.h>
MRL::Common * MRL::Common::_instance = nullptr; //!< global instance
wxHtmlEasyPrinting  * MRL::Common::_htmlPrinter = nullptr; // printer interface
MRL::StringMap MRL::Common::_aliasMap; // maps path to alias
MRL::StringMap MRL::Common::_reverseAliasMap; // maps alias to path

/*!
 * \brief MRL::Common::~Common
 * Clean up.
 */
MRL::Common::~Common() {
    if(_daq) delete _daq;
    if(_opc) delete _opc;
    _instance = nullptr;
}

/*!
 * \brief MRL::Common::initialise
 * \param dir
 * \param settings
 * \return true on success
 */
bool MRL::Common::initialise(const std::string &dir, const std::string &fs )
{
    //
    try
    {
        if(!_instance)
        {
            _instance = new Common();
            _instance->_daq = new DaqCommon; // create the DAQ common object
            _instance->_opc = new OpcCommon;
        }
        if(!_htmlPrinter)
        {
            _htmlPrinter = new wxHtmlEasyPrinting(); //
        }
        _instance->_display = std::make_unique<DataItemTree>();
        //
        MRL::ObjectManager::initialise();
        //
        Common::setConfigFileName(dir + "/" + fs);
        if(settings().load(Common::configFileName()))
        {
            Common::configuration().setFile("CONFIG");
            Common::configuration().setDir(Common::baseDir() +"/data");
            Common::configuration().createDatabase();
            Common::configuration().loadFromDb();
            Common::configuration().listAlias(_aliasMap, _reverseAliasMap);
            //
            // Create the admin user if missing
            //
            if(!settings().exists("/Users/Admin"))
            {
                settings().setString("/Users/Admin/Password","password"); // to do encrypt password
                settings().setBool("/Users/Admin/Enabled",true);
                settings().setBool("/Users/Admin/Admin",true);
            }
            return true;
        }

    }
    CATCH_DEF
    return false;
}

/*!
 * \brief MRL::Common::clear
 */
void MRL::Common::clear()
{
    try
    {
        if(_instance)
        {
            _instance->_settings.clear();
            _instance->_globals.clear();
            _instance->_runtime.clear();
            _instance->_db.db().CloseConnection();
            _instance->_opc->clear();
            delete _instance;
            _instance = nullptr;
        }
    }
    CATCH_DEF
}

/*!
 * \brief MRL::Common::checkUser
 * \param u
 * \param pw
 * \return
 */
bool MRL::Common::checkUser(const std::string &u, const std::string &pw)
{
    // validate user name and password
    MRL::PropertyPath p;
    p.push_back("Users");
    p.push_back(u);
    if(SETTINGS().exists(p))
    {
        std::string s = SETTINGS().getValue<std::string>(p,"Password");
        // TO DO encrypt passwords
        if(pw == s)
        {
            return SETTINGS().getValue<bool>(p,"Enabled");
        }
    }
    return false;
}

/*!
 * \brief MRL::Common::checkUserAdmin
 * \param u
 * \return
 */
bool MRL::Common::checkUserAdmin(const std::string &u)
{
    // is the user an admin user
    MRL::PropertyPath p;
    p.push_back("Users");
    p.push_back(u);
    if(SETTINGS().exists(p))
    {
         return SETTINGS().getValue<bool>(p,"Enabled") && SETTINGS().getValue<bool>(p,"Admin");
    }
    return false;
}

/*!
 * \brief MRL::Common::setupLuaApi
 * \param state
 */
void MRL::Common::setupLuaApi(LUASCRIPTPTR &state)
{
    //
    // Create common interface to an LUA script
    //
    (*state)["Util"].SetObj(*this,
                           "Trace", &MRL::Common::trace
                          );
    //
    // Property tree interface
    //
    (*state)["Settings"].SetObj(_settings,
                             "GetBool", &VariantPropertyTree::getBool,
                             "GetInt", &VariantPropertyTree::getInt,
                             "GetDouble", &VariantPropertyTree::getDouble,
                             "GetString", &VariantPropertyTree::getString,
                             "SetBool", &VariantPropertyTree::setBool,
                             "SetInt", &VariantPropertyTree::setInt,
                             "SetDouble", &VariantPropertyTree::setDouble,
                             "SetString", &VariantPropertyTree::setString,
                             "Remove", &VariantPropertyTree::erase
                            );
    //
    (*state)["Runtime"].SetObj(_runtime,
                               "GetBool", &VariantPropertyTree::getBool,
                               "GetInt", &VariantPropertyTree::getInt,
                               "GetDouble", &VariantPropertyTree::getDouble,
                               "GetString", &VariantPropertyTree::getString,
                               "SetBool", &VariantPropertyTree::setBool,
                               "SetInt", &VariantPropertyTree::setInt,
                               "SetDouble", &VariantPropertyTree::setDouble,
                               "SetString", &VariantPropertyTree::setString,
                               "Remove", &VariantPropertyTree::erase
                            );
}

