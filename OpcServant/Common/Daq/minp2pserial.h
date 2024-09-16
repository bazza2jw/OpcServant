#ifndef MINP2PSERIAL_H
#define MINP2PSERIAL_H
//
// The MIN serial peer to peer protocol
// this wraps the min C functions
//
#include <MrlLib/mrllib.h>
#include <wx/stopwatch.h>
#include "serial.hpp"
#include "min.h"
#include <functional>
#include <Common/bobject.h>


namespace MRL
{
typedef std::function<void (uint8_t const *, uint8_t)> RECEIVEFN;
class MinP2PSerial : public MRL::SerialConnect, public BObject
{
    min_context _context;
    static bool _timerInit;
    std::map<uint8_t,RECEIVEFN> _callbacks;
    mutable ReadWriteMutex _mutex;

public:
    MinP2PSerial(const std::string &port = "");
    virtual ~MinP2PSerial();
    //
    bool hasCallback(uint8_t b) { return _callbacks.find(b) != _callbacks.end();}
    void addCallback(uint8_t b, const RECEIVEFN &f) { if(!hasCallback(b)) _callbacks[b] = f;}
    void removeCallback(uint8_t b) { _callbacks.erase(b);}
    ReadWriteMutex & mutex() { return  _mutex;}

    //
    min_context *context() { return &_context;}

    // space in output buffer
    virtual uint16_t tx_space();
    // CALLBACK. Send a byte on the given line.
    virtual void tx_byte(uint8_t byte)
    {
        write((void *)&byte,1);
    }

    // CALLBACK. Indcates when frame transmission is finished; useful for buffering bytes into a single serial call.
    virtual void tx_start() {}
    virtual void tx_finished() {}

    // CALLBACK. Handle incoming MIN frame
    virtual void application_handler(uint8_t min_id, uint8_t const *min_payload, uint8_t len_payload)
    {
        if(_callbacks.find(min_id) != _callbacks.end())
        {
            RECEIVEFN &f = _callbacks[min_id];
            f(min_payload,len_payload); // invoke callback
        }
    }
    //
    // wrap MIN API
    // Queue a MIN frame in the transport queue
    bool queue_frame(uint8_t min_id, uint8_t const *payload, uint8_t payload_len);
    // is there space in the queue for the frame
    bool queue_has_space_for_frame(uint8_t payload_len);
    // Send a non-transport frame MIN frame
    void send_frame(uint8_t min_id, uint8_t const *payload, uint8_t payload_len);

    // Must be regularly called, with the received bytes since the last call.
    // NB: if the transport protocol is being used then even if there are no bytes
    // this call must still be made in order to drive the state machine for retransmits.
    virtual void poll();
    //
    // Reset the state machine and (optionally) tell the other side that we have done so
    // Call this before first transaction - otherwise sequence errors
    //
    void transport_reset( bool inform_other_side);
    //
    static wxStopWatch _timer;

    typedef std::unique_ptr<MinP2PSerial> PTR;
    static std::map<std::string,PTR > _connections;

    static bool addConnection(const std::string &s, bool attachToDaq = true);
    static MinP2PSerial * find(const std::string &s);
    static bool exists(const std::string &s) { return _connections.find(s) != _connections.end();}
    static void pollAll();


    struct P2PMSG
    {
        uint8_t _op; // the opcode
        uint32_t _id; // transaction id - used to match request - response
        uint8_t _len; // payload length
        uint8_t _payload[120]; // the payload
    };


    static void packP2PMsg(P2PMSG &msg, uint8_t *buf)
    {
        buf[0] = msg._op;
        buf[1] = msg._len;
        memcpy(buf + 2, &msg._id, sizeof(uint32_t));
        memcpy(buf + 6,msg._payload,msg._len);
    }

    static void unpackP2PMsg(P2PMSG &msg, uint8_t const *buf)
    {
        msg._op = buf[0];
        msg._len = buf[2];
        memcpy(&msg._id,buf+2,sizeof(uint32_t));
        memcpy(msg._payload,buf + 6,msg._len);
    }


};
}
#endif // MINP2PSERIAL_H
