#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <wx/object.h>
#include <MrlLib/mrllib.h>
#include <wx/socket.h>
#include <wx/event.h>
#include <functional>

namespace MRL {

/*!
 * \brief The TCPServer class
 */
class TCPServer :  public wxSocketServer
{
public:
    typedef std::function<void (wxSocketBase *)> HANDLER;
private:
    wxEvtHandler * _handler = nullptr;
    wxIPV4address _address;
    HANDLER _processInput;
    HANDLER _outputDone;
public:
    wxEvtHandler * handler() { return _handler;}
    /*!
     * \brief The Connection class
     */
    class Connection
    {
        TCPServer * _parent;
        wxIPV4address _peer;
        wxSocketBase * _socket;
    public:
        /*!
         * \brief Connection
         * \param parent
         * \param s
         */
        Connection(TCPServer * parent, wxSocketBase *s) : _parent(parent)
        {
            _socket = s; // take ownership
            _socket->SetEventHandler(*(_parent->handler()));
            _socket->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_OUTPUT_FLAG | wxSOCKET_LOST_FLAG);
            _socket->Notify(true);
            _socket->SetFlags(wxSOCKET_NOWAIT);
            _socket->GetPeer(_peer);

        }
        ~Connection()
        {
            if(_socket)
            {
                 _parent->remove(_socket);
                _socket->Close();
            }
        }
        /*!
         * \brief onSocketEvent
         * \param pEvent
         */
        virtual void onSocketEvent(wxSocketEvent& pEvent);
    };

    std::map<wxSocketBase *,Connection *> _connections;
    void setProcessInput(HANDLER &f) {_processInput = f;}
    void setOutputDone(HANDLER &f) { _outputDone = f;}


    /*!
     * \brief TCPServer
     * \param la
     */

    TCPServer(wxIPV4address &la ) : wxSocketServer(la,wxSOCKET_NOWAIT|wxSOCKET_REUSEADDR)
    {
        _address = la;
        if (!IsOk())
        {
            throw std::runtime_error("Cannot bind listening socket");
        }
    }

    /*!
     * \brief address
     * \return
     */
    wxIPV4address &address() { return _address;}
    /*!
     * \brief bind
     * \param handler
     */
    void bind(wxEvtHandler *h, int id = wxID_ANY)
    {
        _handler = h;
        SetEventHandler(*_handler,id);
        SetNotify(wxSOCKET_CONNECTION_FLAG);
        Notify(true);
    }
    /*!
     * \brief OnSocketEvent
     * \param pEvent
     */
    virtual void OnSocketEvent(wxSocketEvent& pEvent);
    /*!
     * \brief remove
     * \param c
     */
    void remove(wxSocketBase  *c)
    {
       _connections.erase(c);
    }
};
}
#endif // TCPSERVER_H
