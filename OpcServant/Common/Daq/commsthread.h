#ifndef COMMSTHREAD_H
#define COMMSTHREAD_H

#include <MrlLib/threadHelper.h>
#include <Common/Daq/daq.h>


namespace MRL
{
// drive communications
class CommsThread : public ThreadHelper {
    bool _runP2p = false; // do we run these interfaces
    bool _runModbus = false;
public:
    CommsThread(bool runP2P = false, bool runModbus = false) : _runP2p(runP2P),_runModbus(runModbus) {}
    virtual void process();
};

}

#endif // COMMSTHREAD_H
