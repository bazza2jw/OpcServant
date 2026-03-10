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
#ifndef OPCTHREAD_H
#define OPCTHREAD_H
#include <Common/Opc/opcserver.h>
#include <MrlLib/threadHelper.h>

namespace MRL {
/*!
     * \brief The OpcThread class
     */
    class OpcThread : public ThreadHelper {
            std::unique_ptr<OpcServer> _opc; //!< opc server
        public:
            OpcThread();
            virtual ~OpcThread();
            virtual void begin()
            {
                try
                {
                    if(_opc) _opc->start(); // runs the OPC loop in the thread
                }
                CATCH_DEF;
            }
            virtual void end()
            {
                if(_opc) _opc->stop();
            }
            virtual void stop()
            {
                try
                {
                    if(_opc) _opc->stop(); // mark to stop OPC
                    ThreadHelper::stop(); // stop thread and join
                }
                CATCH_DEF;
            }

    };
}
#endif // OPCTHREAD_H
