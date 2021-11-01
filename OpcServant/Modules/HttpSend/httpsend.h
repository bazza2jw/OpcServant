#ifndef HTTPSEND_H
#define HTTPSEND_H

#include "httpsend_global.h"
#include <Common/plugin.h>
#include <Common/objectmanager.h>
namespace MRL
{
    class  HttpSend : public Plugin {
    public:
        HttpSend();
        //
        // Global initialise - create the object manager
        virtual void initialise();
        virtual void terminate();
        //
    };
}

#endif // HTTPSEND_H
