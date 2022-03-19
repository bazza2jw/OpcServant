#include "mimicdisplayrt.h"
#include <Common/Daq/daqcommon.h>
#include <Common/messageids.h>
#include <Common/stockobjectids.h>

/*!
    \brief MimicDisplayRT
    \param id
*/
MRL::MimicDisplayRT::MimicDisplayRT(unsigned id) : RTObject(id) {
}
/*!
    \brief start
*/
void MRL::MimicDisplayRT::start() {
    RTObject::start();

}
/*!
    \brief stop
*/
void MRL::MimicDisplayRT::stop() {
    RTObject::stop();
}


