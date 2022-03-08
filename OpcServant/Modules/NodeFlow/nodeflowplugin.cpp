#include "nodeflowplugin.h"
#include "nodeflowobjectmanager.h"
#include "nodeflowtypeobject.h"
#include "nodeflownodes.h"

static MRL::NodeFlowPlugin _instance;

/*!
 * \brief MRL::NodeFlowPlugin::NodeFlowPlugin
 */
MRL::NodeFlowPlugin::NodeFlowPlugin() : MRL::Plugin("NodeFlow") {}

//
// Global initialise
void MRL::NodeFlowPlugin::initialise()
{
    // add object factory
    (void) new MRL::NodeFlowObjectManager(); // create the object factory / manager
    NODEFLOW::NodeType::addType<OpcServantInput>("OpcServantInput");
    NODEFLOW::NodeType::addType<OpcServantOutput>("OpcServantOutput");
    NODEFLOW::NodeType::addType<OpcServantPublish>("OpcServantPublish");
}

// Initialise OPC
void MRL::NodeFlowPlugin::initialiseOpc()
{
    // Construct OPC type Object
    Common::opc().add<NodeFlowTypeObject>("NodeFlow");
}

