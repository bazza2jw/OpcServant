#ifndef COMMSTHREAD_H
#define COMMSTHREAD_H

#include <MrlLib/mrllib.h>
#include <Common/Daq/daq.h>

namespace MRL
{
// drive communications
class CommsThread : public wxThreadHelper {
         static CommsThread * _thread;
         bool _runP2p = false; // do we run these interfaces
         bool _runModbus = false;
         bool _runThread = false;
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
        void OnKill();
        static void stop();
        static void start(bool runP2P = false, bool runModbus = false);

};

}

#endif // COMMSTHREAD_H
