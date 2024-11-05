#ifndef THREADHELPER_H
#define THREADHELPER_H
#include <MrlLib/mrllib.h>
#include <iostream>
#include <thread>
#include <functional>

namespace MRL {
/*!
 * \brief The ThreadHelper class
 * This is thread wrapper class - threads are created with start()
 */
class ThreadHelper
{
    volatile bool _run = false;
    std::unique_ptr<std::thread> _thread;
public:
    ThreadHelper() {}
    virtual ~ThreadHelper() {
        stop();
    }
    void sleep(double t)
    {
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(t));
    }
    //
    virtual void begin() {}
    virtual void end() {}
    virtual void process();
    virtual bool running() const { return _run;}
    //
    /*!
     * \brief run
     * This is the thread function code
     */
    void run()
    {
        try
        {
            begin(); // call once on starting in thread context
            while(running()) process(); // process must return so the thread can be killed cleanly or it must query _run
            end(); // call once on finish in thread context
        }
        catch(std::exception &e)
        {
            EXCEPT_TRC;
        }
        catch (...) {
            EXCEPT_DEF;
        }

    }
    virtual void start()
    {
        try
        {
            if(!running())
            {
                _run = true;
                auto f = [this] { run();};
                _thread.reset(0); // delete old thread, if any
                _thread = std::make_unique<std::thread>(f);
            }
        }
        catch(std::exception &e)
        {
            EXCEPT_TRC;
        }
        catch (...) {
            EXCEPT_DEF;
        }

    }
    virtual void stop()
    {
        try
        {
            if(running())
            {
                _run = false;
                this->sleep(10);
                if(_thread && _thread->joinable())
                {
                    _thread->join();
                }
            }
        }
        catch(std::exception &e)
        {
            EXCEPT_TRC;
        }
        catch (...) {
            EXCEPT_DEF;
        }

    }
};
}


#endif // THREADHELPER_H
