#ifndef RTOBJECTSCRITPED_H
#define RTOBJECTSCRITPED_H
#include <Common/Daq/rtobject.h>
#include <MrlLib/luascript.h>
namespace MRL {

/*!
 * \brief The RTObjectScritped class
 */
class RTObjectScritped : public RTObject
{
    std::string _script;
public:
    RTObjectScritped(unsigned id, const std::string &s) : RTObject(id),_script(s) {}

};
}
#endif // RTOBJECTSCRITPED_H
