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
#ifndef DATASOURCE_H
#define DATASOURCE_H
#include <MrlLib/mrllib.h>
namespace MRL
{
/*!
     * \brief The DataSource class
     * A data source is subscription
     * Data source can include
     * OPC UA
     * MQTT
     * MODBUS/TCP
     */
    class DataSource
    {
        std::string _host = "localhost"; // the host name
        int         _port = 1883; // port to connect on
    public:
        typedef enum
        {
            NONE, OPC, MQTT, MODBUS
        } Type;

        DataSource(std::string &h = "localhost", int p = 1883) : _host(h), _port(p) {}
        virtual ~DataSource() {}
        virtual void start() {} // start the subscription
        virtual void stop() {} // stop the subscription
        virtual void process(){} // drive the message processing loop
        virtual Type type() const { return NONE; } // the source type
    };

    typedef std::unique_ptr<DataSource> DataSourceRef; // smart pointer to data source
    typedef std::map<std::string, DataSourceRef> DataSourceMap; // map of data sources
}
#endif // DATASOURCE_H
