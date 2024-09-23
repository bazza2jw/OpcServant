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
#include <deque>
#include <vector>
#include <queue>

namespace MRL
{
/*!
 * \brief RECEIVEFN
 */
typedef std::function<void (uint8_t const *, uint8_t)> RECEIVEFN;
//
// data frame - because comms has its own thread, queues are used
//

struct FrameElement
{
    uint8_t _id = 0;
    std::vector<uint8_t> _data;
    FrameElement() = default;
    FrameElement(uint8_t id, const uint8_t *data, size_t len) : _id(id),_data(len)
    {
        memcpy(_data.data(),data,_data.size());
    }
    void set(uint8_t id, const uint8_t *data, int len )
    {
        _id = id;
        _data.resize(len);
        memcpy(_data.data(),data,_data.size());
    }
    FrameElement(const FrameElement &) = default;
};

typedef std::queue<FrameElement> FRAMEELEMENTQUEUE;

//
// this encapsulates sessions
// the id is 8 bits
// use the low 4 bits for the channel / service
// use the top 4 bits for the session
//
class MinP2PSerial;
//
enum
{
    SESSION_MASK = 0xF0,
    CHANNEL_MASK = 0x0F,
    SESSION_MAX = 0x0F
};

class Session
{
    uint8_t _id = 0; // session id -  0 - 0xF
    std::map<uint8_t,RECEIVEFN> _callbacks;
    mutable ReadWriteMutex _mutex;
    FRAMEELEMENTQUEUE _received;
public:
    Session() = default;
    Session( uint8_t id) : _id(id) {}
    virtual ~Session() {}

    void receive(const FrameElement &e) // receive from transport layer and queue for later processing
    {
        WriteLock l(_mutex);
        _received.push(e); // queue for processing by the application layer
    }

    // this is the presenation / application layer
    bool hasCallback(uint8_t b) {
        return _callbacks.find(b) != _callbacks.end();
    }
    void addCallback(uint8_t b, const RECEIVEFN &f) {
        if(!hasCallback(b)) _callbacks[b] = f;
    }
    void removeCallback(uint8_t b) {
        _callbacks.erase(b);
    }
    //
    //
    void processReceived()
    {
        while(!_received.empty())
        {

            FrameElement &e = _received.front();
            if(_callbacks.find(e._id) != _callbacks.end())
            {
                RECEIVEFN &f = _callbacks[e._id];
                f(e._data.data(),e._data.size()); // invoke callback
            }
            {
                WriteLock l(_mutex);
                _received.pop();
            }
        }
    }
};

typedef std::unique_ptr<Session> SESSIONPTR;
typedef std::map<uint8_t,SESSIONPTR > SESSIONMAP; // the application end has a set of sessions

// helper for packing data
typedef union {
    uint8_t b[4];
    uint16_t w[2];
    uint32_t l;
    float f;
} VALUE;
typedef std::vector<VALUE> VALUELIST;

/*!
 * \brief The MinP2PSerial class
 */
class MinP2PSerial : public MRL::SerialConnect, public BObject
{
    min_context _context;
    static bool _timerInit;
    mutable ReadWriteMutex _mutex;
    wxStopWatch _pollTimer;

    FRAMEELEMENTQUEUE _out;

    SESSIONMAP _sessions; // set of session

public:
    MinP2PSerial(const std::string &port = "");
    virtual ~MinP2PSerial();
    //
    /*!
     * \brief addSession
     * \param id
     */
    Session * addSession(uint8_t id)
    {
        WriteLock l(mutex());
        if(!findSession(id))
        {
            _sessions[id] = std::make_unique<MRL::Session>(id);
        }
        SESSIONPTR &p = _sessions[id];
        return p.get();
    }
    /*!
     * \brief removeSession
     * \param id
     */
    void removeSession(uint8_t id)
    {
        WriteLock l(mutex());
        _sessions.erase(id);
    }

    void send(uint8_t channel, uint8_t *data, int len, uint8_t session = 0)
    {
        FrameElement e(channel | (session << 4),data,len); // add session id
        _out.push(e);
    }
    /*!
     * \brief findSession
     * \param id
     * \return session pointer
     */
    Session * findSession(uint8_t id)
    {
        if(_sessions.find(id) != _sessions.end())
        {
            SESSIONPTR &p = _sessions[id];
            return p.get();
        }
        return nullptr;
    }

    /*!
     * \brief mutex
     * \return
     */
    ReadWriteMutex & mutex() {
        return  _mutex;
    }
    //
    min_context *context() {
        return &_context;
    }
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
        Session *s =  findSession(min_id >> 4);
        if(s)
        {
            FrameElement e(min_id & CHANNEL_MASK, min_payload, len_payload);
            s->receive(e); // pass up to the session layer
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

    static bool addConnection(const std::string &s);
    static MinP2PSerial * find(const std::string &s);
    static bool exists(const std::string &s) {
        return _connections.find(s) != _connections.end();
    }
    static void pollAll();

};
}
#endif // MINP2PSERIAL_H
