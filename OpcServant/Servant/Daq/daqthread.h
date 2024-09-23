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
#ifndef DAQTHREAD_H
#define DAQTHREAD_H
#include <MrlLib/mrllib.h>
#include <Common/Daq/daq.h>

namespace MRL {

// drive communications
class CommsThread : public wxThreadHelper {
         bool _stopThread = false;
    public:
         CommsThread() {}
        /*!
         * \brief Entry
         * \return exit code
         */
        wxThread::ExitCode Entry();
        /*!
         * \brief OnKill
         */
        void OnKill() { stop();}
        void stop() { _stopThread = true;}
        void start();

};



/*!
     * \brief The DaqThread class
     */
    class DaqThread : public wxThreadHelper {
             std::unique_ptr<Daq> _daq;
             std::unique_ptr<CommsThread> _comms;
             bool _stopThread = false;
        public:
            DaqThread();
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
#endif // DAQTHREAD_H
