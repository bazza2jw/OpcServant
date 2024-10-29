#include "commsthread.h"

#include <Common/Daq/minp2pserial.h>
#include <MrlLib/modbusServer.h>
//
void MRL::CommsThread::process()
{
            // Drive the comms layers
           if(_runP2p) MRL::MinP2PSerial::pollAll(); // this is the Point to point serial networking for talking to microcontroller boards
           if(_runModbus) MRL::ModbusServer::pollAll(); // drive any Modbus servers
            this->sleep(5.0); // let other stuff run
}

