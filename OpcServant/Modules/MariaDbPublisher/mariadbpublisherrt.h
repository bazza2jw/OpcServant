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
#ifndef MARIADBPUBLISHERRT_H
#define MARIADBPUBLISHERRT_H
#include "mariadbpublisher.h"
#include <Common/Daq/rtobject.h>
#include <Common/storagedatabase.h>
#include <queue>

namespace MRL
{
//
// This is a publisher to a maria or mysql database
//
    constexpr int MariaDbPubProcessInterval = 10; // seconds between buffer flushes
    constexpr int MariaDbPubMaxBufferLength = 1000; // maximum items allowed in queue
/*!
     * \brief The MariaDbPublisherRt class
     */
    class MariaDbPublisherRt  : public RTObject
    {
        StorageDatabase _db; // the database interface with all the queries prepared
        Open62541::NodeId _statusNode;
        std::string _lastStatus; // the status
    public:
        /*!
         * \brief MariaDbPublisherRt
         * \param id
         */
        MariaDbPublisherRt(int id);
        /*!
            \brief start
        */
        virtual void start();
        /*!
            \brief stop
        */
        virtual void stop();
        /*!
            \brief publish
        */
        virtual void publish();
        /*!
            \brief measure
        */
        virtual void measure();
        /*!
         * \brief publishQueueItem
         * \param msg
         * \return
         */
        void publishQueueItem(const Message &msg);
        //
        // event messages
        void handleMessage(const MRL::LogMessage &);
        void handleAudit(const MRL::LogMessage &);
        void handleDiagnostic(const MRL::LogMessage &);
        /*!
         * \brief updateOpc
         * \param server
         * \param objectNode
         */
        virtual void updateOpc(Open62541::Server &server, Open62541::NodeId &objectNode);
        /*!
         * \brief initialiseOpc
         * \param server
         * \param objectNode
         */
        virtual void initialiseOpc(Open62541::Server &server, Open62541::NodeId &objectNode);

    };
}
#endif // MARIADBPUBLISHERRT_H
