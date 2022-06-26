#ifndef COSTATE_H
#define COSTATE_H
#include "stopwatch.h"
#include <stack>
#include <queue>
/*!
 * \brief The CoRoutine struct
 * switch/case state machines in a co routine style - should be easier to write and follow.
 * Very simple and should be portable
 * Avoid using CO* macros in switch,while and for blocks.
 * variable scoping may be a problem/confusing.
 * Best to have data external to the state machine
 */
class CoRoutineState
{
    int _state = 0;
    stopwatch::Stopwatch _timer; // elapsed time
    std::uint64_t _waitTime = 0; // time to wait in milli-seconds
    //
    std::queue<int> _events; // the event queue - events are states to goto. events are processed when the state machine is in idle, state == 0
    std::stack<int> _stateStack;
public:
    /*!
     * \brief reset
     * \param s
     */
    virtual void reset(int s = 0) {
        _state = s;
        _waitTime = 0;
        _timer.start();
        while(!_events.empty()) _events.pop();
        while(!_stateStack.empty()) _stateStack.pop();
    }

    /*!
     * \brief postEvent
     * \param e
     */
    void postEvent(int e) {_events.push(e);}
    /*!
     * \brief idle
     * \return  true is the state machine is in an idle state
     */
    bool idle() { return _state == 0;}
    /*!
     * \brief nextEvent
     */
    void nextEvent()
    {
        if(!_events.empty())
        {
            next(_events.front());
            _events.pop();
        }
    }

    /*!
     * \brief next
     * \param i - state to go to
     * This clears any wait state - so when a push returns the wait will have been set to zero
     * Be careful when mixing linear state progression style with event processing style, it may not work
     */
    void next(int i) {
        _waitTime = 0; // clear any wait state
        _state = i;
    }
    /*!
     * \brief waitSeconds
     * \param t
     */
    void waitSeconds(std::uint64_t t)
    {
        wait(t * 1000);
    }
    /*!
     * \brief wait
     * \param t
     */
    void wait(std::uint64_t t) {
        _waitTime = t;
        _timer.start();
    }
    // reason to skip - defaults to wait time
    /*!
     * \brief waiting
     * \return
     */
    virtual bool waiting() {
        if (_timer.elapsed_ms() < _waitTime) return true;
        return false;
    }
    /*!
     * \brief state
     * \return
     */
    int state() const {
        return _state;
    }

    /*!
     * \brief push - this can be a call or an event trigger
     * \param i
     */
    void push(int i)
    {
        _stateStack.push(_state);
        next(i);
    }

    /*!
     * \brief pop - return from call
     */
    void pop()
    {
        if(!_stateStack.empty())
        {
            next(_stateStack.top());
            _stateStack.pop();
        }
    }
    /*!
     * \brief CoRoutineState
     */
    CoRoutineState() {}
    /*!
     * \brief ~CoRoutineState
     */
    virtual ~CoRoutineState() {}
};

// coroutine start -  skips if waiting, if idle gotos the next event if any
#define COBEGIN(s) if(!s.waiting()) { CoRoutineState &__state__ = s; switch(__state__.state()) { case 0:{ __state__.nextEvent();break;
// end of co-routine
#define COEND } break; default: __state__.reset();break;}}
// target for a next or target for COGOTO
#define COLABEL(l) } case l : {
// how to yield
#define COYIELD  __state__.next(__LINE__ + 1000);  } break; case (__LINE__ + 1000): {
// wait for milli-seconds
#define COWAIT(t) __state__.next(__LINE__ + 1000); __state__.wait(t); } break; case (__LINE__ + 1000): {
// goto a state / COLABEL
#define COGOTO(l) __state__.next(l);break;
//
// in effect a call - l is the COLABEL state/label to "call"
#define COPUSH(l) __state__.next(__LINE__ + 1000);__state__.push(l);};break;case (__LINE__ + 1000): {
// in effect a return from a call
#define COPOP __state__.pop();break;
// Reset the state and clear stacks
#define CORESET __state__.reset();break;
// Goto idle , process next event - this is the end of an event process sequence
#define COIDLE  __state__.next(0);break;
//
#endif // COSTATE_H
