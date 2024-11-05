#ifndef COMMSTHREAD_H
#define COMMSTHREAD_H

#include <MrlLib/threadHelper.h>
#include <Common/Daq/daq.h>
#include <Common/Daq/minp2pserial.h>
#include <Common/Daq/modbusServer.h>


namespace MRL
{


// drive P2P communication
class CommsThread : public ThreadHelper {
public:
    CommsThread()  {}
    virtual void process()
    {
        try
        {
            MRL::MinP2PSerial::pollAll();
            this->sleep(5.0);
        }
        CATCH_DEF

    }
};

// drive modbus comms
class ModbusThread : public ThreadHelper {
public:
    ModbusThread() {}
    virtual void process()
    {
        try
        {
            MRL::ModbusServer::pollAll();
        }
        CATCH_DEF

    }
};




}

#endif // COMMSTHREAD_H
