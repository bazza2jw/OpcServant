#ifndef NODEFLOWOBJECTMANAGER_H
#define NODEFLOWOBJECTMANAGER_H
#include <Common/objectmanager.h>
#include <Common/Gui/SetupDialog.h>
#include <Common/Web/webpanel.h>
#include "nodeflowrt.h"
#include "NodeFlowConfiguration.h"
namespace MRL{

typedef ObjectManagerRT<NodeFlowRT,NodeFlowConfiguration> NodeFlowObjectManagerBase;


class NodeFlowObjectManager : public NodeFlowObjectManagerBase
{
public:
    NodeFlowObjectManager() : NodeFlowObjectManagerBase("NodeFlow") {}
};
}
#endif // NODEFLOWOBJECTMANAGER_H
