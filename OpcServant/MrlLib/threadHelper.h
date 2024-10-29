#ifndef THREADHELPER_H
#define THREADHELPER_H
#include <MrlLib/mrllib.h>
#include <iostream>
#include <thread>
#include <functional>

namespace MRL {
class ThreadHelper
{
    bool _run = false;
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
    //
    void run()
    {
        try
        {
            begin();
            while(_run) process();
            end();
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
            if(!_run)
            {
                _run = true;
                auto f = [this] { run();};
                _thread.reset(0);
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
            if(_run)
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
