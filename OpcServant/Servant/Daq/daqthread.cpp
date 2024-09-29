/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "daqthread.h"
#include "../serverobject.h"
#include <Common/plugin.h>



MRL::DaqThread::DaqThread()
{
    _daq = std::make_unique<Daq>(); //!< create the daq object
    Plugin::initialiseAllDaq(); //!< create objects that connect to the Daq thread
}
/*!
 * \brief Entry
 * \return exit code
 */
wxThread::ExitCode MRL::DaqThread::Entry()
{
    try
    {
        if(_daq)
        {
            wxStopWatch timer;
            while( !GetThread()->TestDestroy() && !_stopThread)
            {
                timer.Start();
                _daq->process();
                while(timer.Time() < 20) // aim for a 20ms step
                {
                    wxThread::Sleep(5); // let other stuff run
                }
            }
            _daq->stop();
        }
    }
    catch(std::exception &e)
    {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
    wxLogDebug("Exit DAQ Thread");
    return wxThread::ExitCode(nullptr);
}

/*!
 * \brief OnKill
 */
void MRL::DaqThread::OnKill()
{
    stop();
}

/*!
 * \brief start
 * Start the thread
 */
void MRL::DaqThread::start()
{
    wxLogDebug("Start DAQ Thread");
    try
    {
        _stopThread = false;
        _daq->start();
        CreateThread();
        GetThread()->SetPriority(wxPRIORITY_DEFAULT / 2); // this thread is not event driven
        GetThread()->Run();
    }
    catch(std::exception &e)
    {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
}

/*!
 * \brief stop
 * Stop the thread
 */
void MRL::DaqThread::stop()
{
    wxLogDebug("Stop DAQ Thread");
    _stopThread = true;
}
