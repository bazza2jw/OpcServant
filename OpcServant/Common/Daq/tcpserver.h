#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <MrlLib/mrllib.h>
#include <MrlLib/threadHelper.h>
#include <iostream>
#include <functional>
#include <wx/socket.h>
#include <queue>

// Based on github.com/alejandrofsevilla/boost-tcp-server-client
namespace MRL {

class TcpServer;
// A connection
class TcpConnectionBase  {
    wxSocketBase *_sock = nullptr;
    TcpServer * _parent = nullptr;
    bool _disconnected = false;
 public:

  TcpConnectionBase( TcpServer *parent = nullptr, wxSocketBase *s  = nullptr )
      : _parent(parent), _sock(s)
  {

  }
  virtual ~ TcpConnectionBase() { if(_sock) _sock->Destroy();}

  wxSocketBase *socket() const { return _sock;}

  TcpServer * parent() const { return _parent;}

  wxUint32 write(void *data, wxUint32 len)
  {
      wxUint32 ret = 0;
      if(socket())
      {
          socket()->Write(data,len);
          if(!socket()->Error())
          {
              ret = socket()->GetLastIOWriteSize();
          }
      }
      return ret;
  }
  void close()
  {
      socket()->Close();
  }
  virtual void dataReady(uint8_t */*b*/, uint32_t /*n*/) {} // handle data received
  virtual void disconnect() {} // disconnect event
  virtual void process();


};


// TCP Server class

class TcpServer  : public ThreadHelper {

 typedef std::unique_ptr<TcpConnectionBase> TCPCONNECTIONPTR;

 std::queue<wxSocketBase *> _eraseList;
 std::map<wxSocketBase *, TCPCONNECTIONPTR> _connections; // set of connections
 wxIPV4address _address;
 std::unique_ptr<wxSocketServer> _server;

public:
  TcpServer() = default;
  TcpServer(wxIPV4address &a) { _address = a;}
  TcpServer(int port, const wxString &host = wxString() )
  {
      _address.Service(port);
      if(host.IsNull())
      {
          _address.AnyAddress();
      }
      else
      {
          _address.Hostname(host);
      }
  }
  TcpServer(const wxString &port, const wxString &host = wxString() )
  {
      _address.Service(port);
      if(host.IsNull())
      {
          _address.AnyAddress();
      }
      else
      {
          _address.Hostname(host);
      }
  }

  virtual ~TcpServer()
  {
    _connections.clear();
  }

  void start()
  {
      _server.reset(0);
      _server = std::make_unique<wxSocketServer>(_address);
      if(!_server->IsOk()) throw std::runtime_error("Failed to create TCP server");
      ThreadHelper::start();
  }
  void stop()
  {
      if(_server)
      {
          for(auto const &i : _connections)
          {
              const TCPCONNECTIONPTR &p = i.second;
              if(p)
              {
                p->close();
              }
          }
      }

      ThreadHelper::stop();
  }

  virtual TcpConnectionBase * createConnection(wxSocketBase *s)
  {
      return new TcpConnectionBase(this,s);
  }


  void process()
  {
      // is there anything to accept
      wxSocketBase *s = _server->Accept(false); // do not block
      if(s)
      {
        s->SetFlags(wxSOCKET_NOWAIT_READ);  // do not block on read
        TCPCONNECTIONPTR ptr(createConnection(s));
        _connections[s] = std::move(ptr);
      }

      for(auto const &i : _connections)
      {
          const TCPCONNECTIONPTR &p = i.second;
          if(p)
          {
            p->process();
          }
      }
      // delete any pending closed connections
      while(!_eraseList.empty())
      {
          wxSocketBase *s = _eraseList.front();
          _connections.erase(s);
          _eraseList.pop();
      }

  }

  void remove(wxSocketBase *s)
  {
      _eraseList.push(s);
  }
};



}


#endif // TCPSERVER_H
