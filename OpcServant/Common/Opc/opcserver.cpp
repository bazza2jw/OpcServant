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
#include "opcserver.h"
#include <Common/plugin.h>
#include "opccommon.h"
#include "opcnodeids.h"
#include <Common/Daq/daq.h>
#include <Common/objectmanager.h>
#include <Common/Daq/daqcommon.h>
#include <Common/messageids.h>
/*!
    \brief MRL::OpcServer::OpcServer
    \param p
    \param certificate
*/
MRL::OpcServer::OpcServer(int port, wxThreadHelper *thread) :
    Open62541::Server(port),
    _handler(*this),
    _thread(thread),
    _historian(nullptr) { // not enabled
    MRL::PropertyPath p;
    p.push_back("System");
    addNamespace(MRL::SETTINGS().getValue<std::string>(p, "OpcNamespace"));

    if(MRL::SETTINGS().getValue<bool>(p, "LoginRequired"))
    {
        _username = MRL::SETTINGS().getValue<std::string>(p, "OpcUsername");
        _password = MRL::SETTINGS().getValue<std::string>(p, "OpcPassword");
        std::vector<UA_UsernamePasswordLogin> &li = logins();
        li.resize(1);
        li[0].username = Open62541::toUA_String(_username);
        li[0].password = Open62541::toUA_String(_password);
        enableSimpleLogin(false); // require login
    }
    else
    {
        //enableSimpleLogin();
    }
}

/*!
    \brief MRL::OpcServer::~OpcServer
*/
MRL::OpcServer::~OpcServer() {
}

/*!
    \brief MRL::OpcServer::initialise
*/
void MRL::OpcServer::initialise() {
    Open62541::Server::initialise();
    //
    //
    // Estimate the number of historian nodes required
    //
    int nNode = 0;
    //
    DataNodeIdMap &m = Common::configuration().idMap();
    for (auto i = m.begin(); i != m.end(); i++) {
        DataNode *d = i->second;
        if (d) {
            auto o = ObjectManager::find(d->data().type());
            if (o) {
                nNode += o->opcHistoryCount();
            }
        }
    }
    setHistorian(nNode + HistoryNodeSizeMargin, DefaultHistorySize); // set the historian
    // Set up the name space
    Common::instance()->opc().setNameSpaceIndex((_idx = addNamespace("urn:opcservant")));
    //
    // Set up the folder structure
    // The top level folders must be set up
    //
    _controlFolder = Open62541::NodeId(_idx, OPCSERVANTID::Control);
    _dataFolder = Open62541::NodeId(_idx, OPCSERVANTID::Data);
    //
    addFolder(Open62541::NodeId::Objects, "Control", _controlFolder, Open62541::NodeId::Null, _idx);
    addFolder(Open62541::NodeId::Objects, "Data", _dataFolder,  Open62541::NodeId::Null, _idx);
    //
    Plugin::initialiseAllOpc(); // create OPC objects and types
    //
    _handler.join(Daq::instance()); // connect to Daq thread for messages
}

/*!
    \brief MRL::OpcServer::process
*/
void MRL::OpcServer::process() {
    if (running()) {
        if (_thread && _thread->GetThread()->TestDestroy()) {
            stop();
        }
        else {
            _handler.process(); // process message queue
        }
    }
}

/*!
    \brief MRL::OpcServerHandler::addObject
    \param id
*/
void MRL::OpcServerHandler::addObject(unsigned id) {
    DataNode *d = Common::configuration().idMap()[id];
    //
    if (d && d->parent()) { // recursively add if necessary
        TRC_LINE("Parent Id " << d->parent()->data().id() << " " << d->parent()->name())
        if (!isMapped(d->parent()->data().id())) {
            TRC_LINE("Add Parent " << d->parent()->data().id())
            addObject(d->parent()->data().id());
        }
    }
    //
    if (d) {
        auto o = ObjectManager::find(d->data().type());
        if (o) {
            // folders are created as folders and not objects
            if (o->isTopLevel()) {
                // parent id is the data folder if the parent is null (ROOT) (ie top level)
                Open62541::NodeId parent(_server.applicationNamespace(), OPCSERVANTID::Data);
                Open62541::NodeId requestNodeId(_server.applicationNamespace(), id);
                //
                if (!_server.addFolder(parent, d->name(),
                                       requestNodeId,
                                       Open62541::NodeId::Null,
                                       _server.applicationNamespace())) {
                    UAPRINTLASTERROR(_server.lastError());
                }
                setNode(id);
                TRC("Parent "  << Open62541::toString(parent.get())
                    << " Folder Node Id " << Open62541::toString(requestNodeId.get()))

            }
            else if (o->name() == FOLDER_TYPE) {
                // parent id is the data folder if the parent is null (ie top level)
                unsigned pid = (d->parent()) ? (d->parent()->data().id()) : OPCSERVANTID::Data;
                Open62541::NodeId parent(_server.applicationNamespace(), pid);
                Open62541::NodeId requestNodeId(_server.applicationNamespace(), id);
                TRC("Add Folder " << d->name())
                //
                if (!_server.addFolder(parent, d->name(),
                                       requestNodeId,
                                       Open62541::NodeId::Null,
                                       _server.applicationNamespace())) {
                    UAPRINTLASTERROR(_server.lastError());
                }
                setNode(id);
                TRC("Parent "  << Open62541::toString(parent.get())
                    << " Folder Node Id " << Open62541::toString(requestNodeId.get()))
            }
            else {
                removeObject(id); // remove any existing object
                // find the type object
                auto to = Common::opc().type(o->name());

                if (to) {
                    // get path to object
                    Open62541::NodeId parent(_server.applicationNamespace(), d->parent()->data().id());
                    Open62541::NodeId requestNodeId(_server.applicationNamespace(), id);
                    //
                    if (to->addInstance(d->name(), parent, Open62541::NodeId::Null,  requestNodeId, nullptr)) { // add object and fix up any write callbacks
                        TRC("Parent "  << Open62541::toString(parent.get())
                            << " Object Node Id " << Open62541::toString(requestNodeId.get()))
                        //
                        // set up the instance with the DAQ data
                        o->setupOPCInstance(_server, id, requestNodeId);
                        //
                        updateObject(id); // load current values if any
                        setNode(id);
                    }
                }
            }
        }
    }
}
/*!
    \brief MRL::OpcServerHandler::removeObject
    \param id
*/
void MRL::OpcServerHandler::removeObject(unsigned id) {
    Open62541::NodeId nd(_server.applicationNamespace(), id);
    _server.deleteNode(nd, false);
    _nodeMap.erase(id);
}
/*!
    \brief MRL::OpcServerHandler::updateObject
    \param id
*/
void MRL::OpcServerHandler::updateObject(unsigned id) {
    if (isMapped(id)) {
        Open62541::NodeId nd(_server.applicationNamespace(), id); // form the node id
        // find the DAQ object to do the update
        RtObjectRef &r = Common::daq().objects()[id]; // find the object
        if (r.get()) r.get()->updateOpc(_server, nd); // action update
    }
}
/*!
    \brief MRL::OpcServerHandler::processQueueItem
    \return true if processed
*/
bool MRL::OpcServerHandler::processQueueItem(const Message &msg) {
    if (!BObject::processQueueItem(msg)) {
        // work out what to do with the message
        Message &m = const_cast<Message &>(msg);
        //
        switch (m.id()) {
        case MESSAGEID::Update_Object: {
            unsigned id;
            m.data().get(PARAMETERID::ObjectId, id);
            updateObject(id);
        }
        break;

        case MESSAGEID::Opc_Clear: { // remove the non-top level items
            for (auto i = _nodeMap.begin(); i != _nodeMap.end(); i++) {
                DataNode *d = Common::configuration().idMap()[i->first];
                if (d) {
                    auto o = ObjectManager::find(d->data().type());
                    if (o) {
                        if (!o->isTopLevel()) {
                            Open62541::NodeId nd(_server.applicationNamespace(), i->first);
                            _server.deleteNode(nd, false);
                            _nodeMap.erase(i);
                        }
                    }
                }
            }
        }
        break;

        case MESSAGEID::Add_Object: {
            //
            unsigned id;
            m.data().get(PARAMETERID::ObjectId, id);
            addObject(id);
        }
        break;
        case MESSAGEID::Remove_Object: {
            unsigned id;
            m.data().get(PARAMETERID::ObjectId, id);
            removeObject(id);
        }
        break;
        default:
            break;
        }
    }
    return true;
}

