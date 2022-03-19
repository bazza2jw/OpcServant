#ifndef MIMICDISPLAYRT_H
#define MIMICDISPLAYRT_H
#include <Common/Daq/rtobject.h>

namespace MRL
{
class MimicDisplayRT : public RTObject
{
public:
    MimicDisplayRT(unsigned id);
    /*!
        \brief start
    */
    virtual void start();
    /*!
        \brief stop
    */
    virtual void stop();
};
}
#endif // MIMICDISPLAYRT_H
