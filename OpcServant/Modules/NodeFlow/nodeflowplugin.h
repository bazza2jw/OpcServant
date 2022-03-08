#ifndef NODEFLOWPLUGIN_H
#define NODEFLOWPLUGIN_H

#include <Common/plugin.h>
#include <Common/objectmanager.h>

namespace MRL
{
/*!
     * \brief The NodeFlowPlugin class
     */
    class NodeFlowPlugin : public Plugin
    {
    public:
        NodeFlowPlugin();
        //
        // Global initialise
        virtual void initialise();
        // Initialise OPC
        virtual void initialiseOpc();
    };
}
#endif // NODEFLOWPLUGIN_H
