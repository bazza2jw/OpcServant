#ifndef ICPCON7065OBJECTMANAGER_H
#define ICPCON7065OBJECTMANAGER_H

#include <Common/objectmanager.h>
#include "icpcon7065rt.h"

namespace MRL {
typedef ObjectManagerRT<ICPCON7065RT,SetupNodeSerial,ConfigureSerialNodeWebDialog> ICPCON7065ObjectManagerBase;
    class ICPCON7065ObjectManager  : public ICPCON7065ObjectManagerBase {
        public:
        ICPCON7065ObjectManager() : ICPCON7065ObjectManagerBase("ICPCON7065") {}
    };
}

#endif // ICPCON7065OBJECTMANAGER_H
