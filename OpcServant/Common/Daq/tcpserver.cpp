#include <Common/Daq/tcpserver.h>

/*!
 * \brief MRL::TcpConnectionBase::process
 */

void MRL::TcpConnectionBase::process()
{
    if(socket()->IsConnected())
    {
        uint8_t b[256]; // read a chunk
        socket()->Read(b,(sizeof(b) - 1));
        wxUint32 n = socket()->LastReadCount();
        if(n > 0)
        {
            b[n] = 0;
            dataReady(b, uint32_t(n));
        }
    }
    else
    {
        if(! _disconnected)
        {
            _disconnected = true;
            disconnect(); // call the disconnect event handler
            parent()->remove(socket()); // queue delete the handler
        }
    }
}
