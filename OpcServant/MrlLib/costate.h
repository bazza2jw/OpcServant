#ifndef COSTATE_H
#define COSTATE_H
#include "stopwatch.h"
/*!
 * \brief The CoRoutine struct
 * State machines in a co routine style
 * Very simple and should be portable
 *
 */
class CoRoutineState
{
    int _state = 0;
    stopwatch::Stopwatch _timer; // elapsed time
    std::uint64_t _waitTime = 0; // time to wait in milli-seconds
    //
public:
    virtual void reset(int s = 0) {
        _state = s;
        _waitTime = 0;
        _timer.start();
    }

    void next(int i) {
        _state = i;
    }

    void waitSeconds(std::uint64_t t)
    {
        wait(t * 1000);
    }

    void wait(std::uint64_t t) {
        _waitTime = t;
        _timer.start();
    }
    // reason to skip - defaults to wait time
    virtual bool waiting() {
        if (_timer.elapsed_ms() < _waitTime) return true;
        return false;
    }

    int state() const {
        return _state;
    }
    CoRoutineState() {}
    virtual ~CoRoutineState() {}

};

// coroutine start -  skips if waiting
#define COBEGIN(s) if(!s.waiting()) { CoRoutineState &__state__ = s; switch(__state__.state()) { case 0:{
// end of co-routine
#define COEND } break; default: __state__.reset();break;}}
// target for a next or target for COGOTO
#define COLABEL(l) } case l : {

// how to yield
#define COYIELD  __state__.next(__LINE__ + 1000);  } break; case (__LINE__ + 1000): {
#define COWAIT(t) __state__.wait(t);COYIELD
// goto a state / COLABEL
#define COGOTO(l) __state__.next(l);break;

#endif // COSTATE_H
