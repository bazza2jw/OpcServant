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

#include <MrlLib/mrllib.h>
#include <Common/Opc/opcserver.h>
namespace MRL {
/*!
     * \brief The OpcThread class
     */
    class OpcThread : public wxThreadHelper {
            std::unique_ptr<OpcServer> _opc; //!< opc server
        public:
            OpcThread();
            ~OpcThread();

            /*!
             * \brief Entry
             * \return exit code
             */
            wxThread::ExitCode Entry();
            /*!
             * \brief OnKill
             */
            void OnKill();
            /*!
             * \brief start
             * Start the thread
             */
            void start();
            /*!
             * \brief stop
             * Stop the thread
             */
            void stop();
    };
}
#endif // OPCTHREAD_H
