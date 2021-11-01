#ifndef LIBMODBUS_CONFIG_H
#define LIBMODBUS_CONFIG_H
    #if defined(_WIN32)
        #include "configWin32.h"
    #else
        #include "configLinux.h"
    #endif
#endif
