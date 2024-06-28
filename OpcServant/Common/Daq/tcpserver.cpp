#include "tcpserver.h"

#include <MrlLib/mrllib.h>
#include "wx/wx.h"
#include "wx/socket.h"
#include "wx/event.h"

const char *GetSocketErrorMsg(int pSockError)
{
    switch(pSockError)
    {
    case wxSOCKET_NOERROR:
        return "wxSOCKET_NOERROR";

    case wxSOCKET_INVOP:
        return "wxSOCKET_INVOP";

    case wxSOCKET_IOERR:
        return "wxSOCKET_IOERR";

    case wxSOCKET_INVADDR:
        return "wxSOCKET_INVADDR";

    case wxSOCKET_NOHOST:
        return "wxSOCKET_NOHOST";

    case wxSOCKET_INVPORT:
        return "wxSOCKET_INVPORT";

    case wxSOCKET_WOULDBLOCK:
        return "wxSOCKET_WOULDBLOCK";

    case wxSOCKET_TIMEDOUT:
        return "wxSOCKET_TIMEDOUT";

    case wxSOCKET_MEMERR:
        return "wxSOCKET_MEMERR";

    default:
        return "Unknown";
    }
}

/*!
 * \brief MRL::TCPServer::OnSocketEvent
 * \param pEvent
 */
void MRL::TCPServer::OnSocketEvent(wxSocketEvent& pEvent)
{
    if(pEvent.GetSocket() == this)
    {
        switch(pEvent.GetSocketEvent())
        {
        case wxSOCKET_INPUT:
            wxLogError("Unexpected wxSOCKET_INPUT in wxSocketServer");
            break;
        case wxSOCKET_OUTPUT:
            wxLogError("Unexpected wxSOCKET_OUTPUT in wxSocketServer");
            break;
        case wxSOCKET_CONNECTION:
        {
            wxSocketBase* sock = Accept();
            wxIPV4address addr;
            if (sock && sock->GetPeer(addr))
            {
                wxLogMessage("Got connection from %s:%d",addr.IPAddress(), addr.Service());
                _connections[sock] = new Connection(this,sock);
            }
            else
            {
                wxLogMessage("Connect Socket Error");
            }
        }
        break;
        case wxSOCKET_LOST:
            wxLogError("Unexpected wxSOCKET_LOST in wxSocketServer");
            break;
        }
    }
    else
    {
        Connection *c = _connections[pEvent.GetSocket()];
        if(c)
        {
            c->onSocketEvent(pEvent);
        }
    }
}

/*!
 * \brief MRL::TCPServer::OnConnectionEvent
 * \param pEvent
 */
void MRL::TCPServer::Connection::onSocketEvent(wxSocketEvent& pEvent)
{
    switch(pEvent.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        break;
    case wxSOCKET_OUTPUT:
        break;
    case wxSOCKET_CONNECTION:
        break;
    case wxSOCKET_LOST:
        // request delete
        break;
    }
}


