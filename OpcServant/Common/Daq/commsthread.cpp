#include "commsthread.h"

#include <Common/Daq/minp2pserial.h>
#include <MrlLib/modbusServer.h>
//
MRL::CommsThread * MRL::CommsThread::_thread = nullptr;
wxThread::ExitCode MRL::CommsThread::Entry()
{
    try
    {
        while( !GetThread()->TestDestroy() && _runThread)
        {
            // Drive the comms layers
           if(_runP2p) MRL::MinP2PSerial::pollAll(); // this is the Point to point serial networking for talking to microcontroller boards
           if(_runModbus) MRL::ModbusServer::pollAll(); // drive any Modbus servers
            wxThread::Sleep(5); // let other stuff run
        }
    }
    catch(std::exception &e)
    {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }

    wxLogDebug("Exit Comms Thread");
    return wxThread::ExitCode(nullptr);
}

/*!
 * \brief MRL::CommsThread::start
 */
void MRL::CommsThread::start(bool runP2P, bool runModbus)
{
    wxLogDebug("Start Comms Thread");
    try
    {
        if(runModbus || runP2P)
        {

            _thread = new MRL::CommsThread();
            _thread->_runThread = true;
            _thread->_runModbus = runModbus;
            _thread->_runP2p = runP2P;
            _thread->CreateThread();
            _thread->GetThread()->Run();
        }
    }
    catch(std::exception &e)
    {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
}

void MRL::CommsThread::OnKill()
{
    stop();
}


/*!
 * \brief MRL::CommsThread::stop
 */
void MRL::CommsThread::stop()
{
    if(_thread)
    {
        //
        _thread->_runThread = false;
        _thread->_runP2p = false;
        _thread->_runModbus = false;
        //
        wxThread::Sleep(100); // let other stuff run
    }

}
