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
#include <MrlLib/threadHelper.h>
#include <Common/Daq/daq.h>
#include <Common/plugin.h>
namespace MRL {



/*!
     * \brief The DaqThread class
     */
    class DaqThread : public ThreadHelper {
             std::unique_ptr<Daq> _daq;
        public:
            DaqThread()
            {
                _daq = std::make_unique<Daq>(); //!< create the daq object
                Plugin::initialiseAllDaq(); //!< create objects that connect to the Daq thread
            }

            //
            virtual void begin()
            {
                if(_daq) _daq->start();
            }
            virtual void end()
            {
                wxLogDebug("DaqThread end() Enter\n");
                if(_daq) _daq->stop();
                wxLogDebug("DaqThread end() Exit\n");
            }
            virtual void process(); // sleep

    };



}
#endif // DAQTHREAD_H
