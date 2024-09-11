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


namespace MRL
{
typedef std::function<void (uint8_t const *, uint8_t)> RECEIVEFN;
class MinP2PSerial : public MRL::SerialConnect
{
    min_context _context;
    static bool _timerInit;
    std::map<uint8_t,RECEIVEFN> _callbacks;

public:
    MinP2PSerial();
    virtual ~MinP2PSerial();

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
    // Reset the state machine and (optionally) tell the other side that we have done so
    void transport_reset( bool inform_other_side);
    //
    static wxStopWatch _timer;
};
}
#endif // MINP2PSERIAL_H
