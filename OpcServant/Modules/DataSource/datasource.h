#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "datasource_global.h"

#include <Common/plugin.h>
#include <Common/objectmanager.h>
namespace MRL
{
    class  DataSource : public Plugin {
    public:
        DataSource();
        //
        // Global initialise - create the object manager
        virtual void initialise();
        virtual void terminate();
        //
    };
}
#endif // DATASOURCE_H
