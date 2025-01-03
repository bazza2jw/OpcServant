#include <Common/Daq/minp2pserial.h>
#include <Common/Daq/daq.h>


wxStopWatch MRL::MinP2PSerial::_timer;
bool MRL::MinP2PSerial::_timerInit = false;





// map of connections - do not squirrel pointers. Always fetch by name!
std::map<std::string,MRL::MinP2PSerial::PTR > MRL::MinP2PSerial::_connections;

void MRL::MinP2PSerial::pollAll()
{
    // drive all connections
    for(auto const &i : _connections)
    {
        const PTR &p = i.second;
        if(p)
        {
            p->poll();
        }
    }
}

/*!
 * \brief MRL::MinP2PSerial::addConnection
 * \param s
 * \param ptr
 * \return
 */
bool MRL::MinP2PSerial::addConnection(const std::string &s,  LibSerial::BaudRate  baudRate)
{
    //
    // add a MinP2P connection to the set and attach for polling. There is no timing contention with other DAQ objects
    // GUI runs in different thread to DAQ - so get the messaging handling right or things might not work, always.
    //
    if(!exists(s))
    {
        _connections[s] = std::make_unique<MRL::MinP2PSerial>(s,baudRate); // transfer ownership to this object
        return true;
    }
    return false; // already exists
}
/*!
 * \brief find
 * \param s
 * \return
 */
MRL::MinP2PSerial * MRL::MinP2PSerial::find(const std::string &s)
{
    if(exists(s))
    {
        return _connections[s].get();
    }
    return nullptr;
}


/*!
 * \brief MRL::MinP2PSerial::MinP2PSerial
 * \param serial
 * \param id
 */
MRL::MinP2PSerial::MinP2PSerial(const std::string &port, LibSerial::BaudRate  baudRate) :
    _port(port),
    _baudRate(baudRate)
{
    if(port.size() > 2)
    {
        try
        {
            Open(port);
            SetBaudRate(baudRate);
        }
        catch(...)
        {
            std::cerr << "Invalid Serial Port " << port << std::endl;
        }
    }
    min_init_context(&_context, (void *)(this));
    _pollTimer.Start();
    if(!_timerInit)
    {
        _timer.Start();
        _timerInit = true;
    }
}
/*!
 * \brief MRL::MinP2PSerial::~MinP2PSerial
 */
MRL::MinP2PSerial::~MinP2PSerial()
{
}

/*!
 * \brief MRL::MinP2PSerial::tx_space
 * \return
 */
uint16_t MRL::MinP2PSerial::tx_space()
{
    // wrinkle here - payload size is fixed in the build - it has to support the frame and payload size on the target - TBD abstract this to make a bit better
    return MAX_PAYLOAD;
}
/*!
 * \brief MRL::MinP2PSerial::queue_frame
 * \param min_id
 * \param payload
 * \param payload_len
 * \return
 */
bool MRL::MinP2PSerial::queue_frame(uint8_t min_id, uint8_t const *payload, uint8_t payload_len)
{
#ifdef TRANSPORT_PROTOCOL
    WriteLock l(mutex());

    // Queue a MIN frame in the transport queue from session layer
    return min_queue_frame(context(), min_id, payload, payload_len);
#else
    return false;
#endif
}

// Determine if MIN has space to queue a transport frame
/*!
 * \brief MRL::MinP2PSerial::queue_has_space_for_frame
 * \param payload_len
 * \return
 */
bool MRL::MinP2PSerial::queue_has_space_for_frame(uint8_t payload_len)
{
#ifdef TRANSPORT_PROTOCOL
    WriteLock l(mutex());
    return min_queue_has_space_for_frame(context(), payload_len);
#else
    return false;
#endif
}
/*!
 * \brief send_frame
 * \param min_id
 * \param payload
 * \param payload_len
 */
void MRL::MinP2PSerial::send_frame(uint8_t min_id, uint8_t const *payload, uint8_t payload_len)
{
    // Send a non-transport frame MIN frame from session layer
    WriteLock l(mutex());
    min_send_frame(context(),  min_id, payload, payload_len);
}

// Must be regularly called, with the received bytes since the last call.
// NB: if the transport protocol is being used then even if there are no bytes
// this call must still be made in order to drive the state machine for retransmits.
void MRL::MinP2PSerial::poll( )
{
    if(IsOpen())
    {
        //
        // Drive Receive
        //
        LibSerial::DataBuffer buf;
        if(IsDataAvailable())
        {
            Read(buf,GetNumberOfBytesAvailable());
            min_poll(context(), buf.data(), buf.size());
        }
        else
        {
            min_poll(context(), buf.data(), 0);
        }
        //
        // can we send a pending message(s)
        //
        while(!_out.empty())
        {
            FrameElement &e = _out.front();
#ifdef TRANSPORT_PROTOCOL

            if(queue_has_space_for_frame(e._data.size()))
            {
                queue_frame(e._id, e._data.data(), e._data.size());
                {
                    WriteLock l(mutex());
                    _out.pop();
                }
            }

#else
            send_frame(e._id, e._data.data(), e._data.size());
            {
                WriteLock l(mutex());
                _out.pop();
            }

#endif
        }
    }
}

// Reset the state machine and (optionally) tell the other side that we have done so
void MRL::MinP2PSerial::transport_reset( bool inform_other_side)
{
#ifdef TRANSPORT_PROTOCOL

    WriteLock l(mutex());
    min_transport_reset(context(), inform_other_side);
#endif
}


extern "C"
{
// CALLBACK. Must return current time in milliseconds.
// Typically a tick timer interrupt will increment a 32-bit variable every 1ms (e.g. SysTick on Cortex M ARM devices).
    uint32_t min_time_ms(void)
    {
        return (uint32_t)MRL::MinP2PSerial::_timer.Time();
    }

// CALLBACK. Must return current buffer space in the given port. Used to check that a frame can be
// queued.
    uint16_t min_tx_space(void *context)
    {
        if(context)
        {
            MRL::MinP2PSerial * p = static_cast<MRL::MinP2PSerial *>(context);
            return p->tx_space();
        }
        return 0;
    }

// CALLBACK. Send a byte on the given line.

    void min_tx_byte(void *context, uint8_t byte)
    {
        if(context)
        {
            MRL::MinP2PSerial * p = static_cast<MRL::MinP2PSerial *>(context);
            p->tx_byte(byte);
        }
    }

// CALLBACK. Indcates when frame transmission is finished; useful for buffering bytes into a single serial call.
    /*!
     * \brief min_tx_start
     * \param context
     */
    void min_tx_start(void *context)
    {
        if(context)
        {
            MRL::MinP2PSerial * p = static_cast<MRL::MinP2PSerial *>(context);
            p->tx_start();
        }
    }
    /*!
     * \brief min_tx_finished
     * \param context
     */
    void min_tx_finished(void *context)
    {
        if(context)
        {
            MRL::MinP2PSerial * p = static_cast<MRL::MinP2PSerial *>(context);
            p->tx_finished();
        }
    }

// CALLBACK. Handle incoming MIN frame
    void min_application_handler(uint8_t min_id, uint8_t const *min_payload, uint8_t len_payload, void *context)
    {
        if(context)
        {
            MRL::MinP2PSerial * p = static_cast<MRL::MinP2PSerial *>(context);
            p->application_handler(min_id,min_payload,len_payload);
        }
    }
}
