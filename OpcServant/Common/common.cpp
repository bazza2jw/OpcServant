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
#include "common.h"
#include <Common/Daq/daqcommon.h>
#include <Common/Opc/opccommon.h>
#include <Common/objectmanager.h>
#include "Gui/BourbonCommonUI.h"
MRL::Common * MRL::Common::_instance = nullptr; //!< global instance
wxHtmlEasyPrinting  * MRL::Common::_htmlPrinter = nullptr;
MRL::StringMap MRL::Common::_aliasMap;
MRL::StringMap MRL::Common::_reverseAliasMap;


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
        Common::setConfigFileName(dir + "/" + fs);
        if(settings().load(Common::configFileName()))
        {
            Common::configuration().setFile("CONFIG");
            Common::configuration().setDir(Common::baseDir() +"/data");
            Common::configuration().createDatabase();
            Common::configuration().loadFromDb();
            Common::configuration().listAlias(_aliasMap, _reverseAliasMap);
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

