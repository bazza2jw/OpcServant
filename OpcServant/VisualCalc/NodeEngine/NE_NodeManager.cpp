#include "NE_NodeManager.hpp"
#include "NE_Utils.hpp"
#include "NE_Node.hpp"
#include "NE_Debug.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_MemoryStream.hpp"
#include "NE_NodeManagerSerialization.hpp"

namespace NE
{

    SERIALIZATION_INFO (NodeManager, 4);

    template <typename SlotListType, typename SlotType>
    static bool HasDuplicates (const SlotListType& slots)
    {
        bool hasDuplicates = false;
        std::unordered_set<SlotType> slotSet;
        slots.Enumerate ([&] (const SlotType& slot) {
            if (slotSet.find (slot) != slotSet.end ()) {
                hasDuplicates = true;
            }
            slotSet.insert (slot);
            return !hasDuplicates;
        });
        return hasDuplicates;
    }
}

NE::OutputSlotList::OutputSlotList ()
{

}

NE::OutputSlotList::~OutputSlotList ()
{

}

bool NE::OutputSlotList::IsEmpty () const
{
    return GetSize () == 0;
}

NE::InputSlotList::InputSlotList ()
{

}

NE::InputSlotList::~InputSlotList ()
{

}

bool NE::InputSlotList::IsEmpty () const
{
    return GetSize () == 0;
}

NE::NodeManager::NodeManager () :
    idGenerator (),
    nodeList (),
    connectionManager (),
    nodeGroupList (),
    updateMode (UpdateMode::Automatic)
{

}

NE::NodeManager::~NodeManager ()
{

}

void NE::NodeManager::Clear ()
{
    idGenerator.Clear ();
    nodeList.Clear ();
    connectionManager.Clear ();
    nodeGroupList.Clear ();
    updateMode = UpdateMode::Automatic;
}

bool NE::NodeManager::IsEmpty () const
{
    return nodeList.IsEmpty () && nodeGroupList.IsEmpty () && connectionManager.IsEmpty ();
}

size_t NE::NodeManager::GetNodeCount () const
{
    return nodeList.Count ();
}

size_t NE::NodeManager::GetNodeGroupCount () const
{
    return nodeGroupList.Count ();
}

size_t NE::NodeManager::GetConnectionCount () const
{
    return connectionManager.GetConnectionCount ();
}

void NE::NodeManager::EnumerateNodes (const std::function<bool (NodePtr)>& processor)
{
    nodeList.Enumerate (processor);
}

void NE::NodeManager::EnumerateNodes (const std::function<bool (NodeConstPtr)>& processor) const
{
    nodeList.Enumerate (processor);
}

bool NE::NodeManager::ContainsNode (const NodeId& id) const
{
    return nodeList.ContainsNode (id);
}

NE::NodeConstPtr NE::NodeManager::GetNode (const NodeId& id) const
{
    return nodeList.GetNode (id);
}

NE::NodePtr NE::NodeManager::GetNode (const NodeId& id)
{
    return nodeList.GetNode (id);
}

NE::NodePtr NE::NodeManager::AddNode (const NodePtr& node)
{
    return AddNode (node, IdPolicy::GenerateNew, InitPolicy::Initialize);
}

bool NE::NodeManager::DeleteNode (const NodeId& id)
{
    if (DBGERROR (!ContainsNode (id))) {
        return false;
    }
    return DeleteNode (GetNode (id));
}

bool NE::NodeManager::DeleteNode (const NodePtr& node)
{

    if (DBGERROR (!ContainsNode (node->GetId ()))) {
        return false;
    }

    nodeGroupList.RemoveNodeFromGroup (node->GetId ());

    node->EnumerateInputSlots ([&] (InputSlotConstPtr inputSlot) {
        connectionManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
        return true;
    });

    node->EnumerateOutputSlots ([&] (OutputSlotConstPtr outputSlot) {
        connectionManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
        return true;
    });

    nodeList.DeleteNode (node->GetId ());

    return true;
}

bool NE::NodeManager::IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const
{
    if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
        return false;
    }
    return connectionManager.IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot);
}

bool NE::NodeManager::CanConnectOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const
{
    return connectionManager.CanConnectOutputSlotToInputSlot (inputSlot);
}

bool NE::NodeManager::CanConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const
{
    if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
        return false;
    }

    if (!connectionManager.CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
        return false;
    }

    NodeConstPtr outputNode = GetNode (outputSlot->GetOwnerNodeId ());
    NodeConstPtr inputNode = GetNode (inputSlot->GetOwnerNodeId ());
    if (DBGERROR (outputNode == nullptr || inputNode == nullptr)) {
        return false;
    }

    bool willCreateCycle = false;
    if (outputNode == inputNode) {
        willCreateCycle = true;
    } else {
        EnumerateDependentNodesRecursive (inputNode, [&] (const NodeConstPtr& dependentNode) {
            if (outputNode == dependentNode) {
                willCreateCycle = true;
            }
        });
    }

    if (willCreateCycle) {
        return false;
    }

    return true;
}

bool NE::NodeManager::CanConnectOutputSlotsToInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot) const
{
    if (DBGERROR (outputSlots.IsEmpty () || inputSlot == nullptr)) {
        return false;
    }

    if (HasDuplicates<OutputSlotList, OutputSlotConstPtr> (outputSlots)) {
        return false;
    }

    if (outputSlots.GetSize () > 1 && inputSlot->GetOutputSlotConnectionMode () != OutputSlotConnectionMode::Multiple) {
        return false;
    }

    bool canConnect = true;
    outputSlots.Enumerate ([&] (const OutputSlotConstPtr& outputSlot) {
        if (!CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
            canConnect = false;
        }
        return canConnect;
    });

    return canConnect;
}

bool NE::NodeManager::CanConnectOutputSlotToInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots) const
{
    if (DBGERROR (outputSlot == nullptr || inputSlots.IsEmpty ())) {
        return false;
    }

    if (HasDuplicates<InputSlotList, InputSlotConstPtr> (inputSlots)) {
        return false;
    }

    bool canConnect = true;
    inputSlots.Enumerate ([&] (const InputSlotConstPtr& inputSlot) {
        if (!CanConnectOutputSlotToInputSlot (outputSlot, inputSlot)) {
            canConnect = false;
        }
        return canConnect;
    });

    return canConnect;
}

bool NE::NodeManager::ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot)
{
    if (DBGERROR (!CanConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
        return false;
    }

    return connectionManager.ConnectOutputSlotToInputSlot (outputSlot, inputSlot);
}

bool NE::NodeManager::ConnectOutputSlotsToInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot)
{
    if (DBGERROR (!CanConnectOutputSlotsToInputSlot (outputSlots, inputSlot))) {
        return false;
    }

    bool success = true;
    outputSlots.Enumerate ([&] (OutputSlotConstPtr outputSlot) {
        if (DBGERROR (!ConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
            success = false;
        }
        return success;
    });

    return success;
}

bool NE::NodeManager::ConnectOutputSlotToInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots)
{
    if (DBGERROR (!CanConnectOutputSlotToInputSlots (outputSlot, inputSlots))) {
        return false;
    }

    bool success = true;
    inputSlots.Enumerate ([&] (InputSlotConstPtr inputSlot) {
        if (DBGERROR (!ConnectOutputSlotToInputSlot (outputSlot, inputSlot))) {
            success = false;
        }
        return success;
    });

    return success;
}

bool NE::NodeManager::DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot)
{
    if (DBGERROR (outputSlot == nullptr || inputSlot == nullptr)) {
        return false;
    }

    if (DBGERROR (!IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot))) {
        return false;
    }

    return connectionManager.DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot);
}

bool NE::NodeManager::DisconnectOutputSlotsFromInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot)
{
    if (DBGERROR (outputSlots.IsEmpty () || inputSlot == nullptr)) {
        return false;
    }

    bool hasDuplicates = HasDuplicates<OutputSlotList, OutputSlotConstPtr> (outputSlots);
    if (DBGERROR (hasDuplicates)) {
        return false;
    }

    bool canDisconnect = true;
    outputSlots.Enumerate ([&] (OutputSlotConstPtr outputSlot) {
        if (DBGERROR (!IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot))) {
            canDisconnect = false;
        }
        return canDisconnect;
    });
    if (DBGERROR (!canDisconnect)) {
        return false;
    }

    bool success = true;
    outputSlots.Enumerate ([&] (OutputSlotConstPtr outputSlot) {
        if (DBGERROR (!DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot))) {
            success = false;
        }
        return success;
    });

    return success;
}

bool NE::NodeManager::DisconnectOutputSlotFromInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots)
{
    if (DBGERROR (outputSlot == nullptr || inputSlots.IsEmpty ())) {
        return false;
    }

    bool hasDuplicates = HasDuplicates<InputSlotList, InputSlotConstPtr> (inputSlots);
    if (DBGERROR (hasDuplicates)) {
        return false;
    }

    bool canDisconnect = true;
    inputSlots.Enumerate ([&] (InputSlotConstPtr inputSlot) {
        if (DBGERROR (!IsOutputSlotConnectedToInputSlot (outputSlot, inputSlot))) {
            canDisconnect = false;
        }
        return canDisconnect;
    });
    if (DBGERROR (!canDisconnect)) {
        return false;
    }

    bool success = true;
    inputSlots.Enumerate ([&] (InputSlotConstPtr inputSlot) {
        if (DBGERROR (!DisconnectOutputSlotFromInputSlot (outputSlot, inputSlot))) {
            success = false;
        }
        return success;
    });

    return success;
}

bool NE::NodeManager::DisconnectAllInputSlotsFromOutputSlot (const OutputSlotConstPtr& outputSlot)
{
    return connectionManager.DisconnectAllInputSlotsFromOutputSlot (outputSlot);
}

bool NE::NodeManager::DisconnectAllOutputSlotsFromInputSlot (const InputSlotConstPtr& inputSlot)
{
    return connectionManager.DisconnectAllOutputSlotsFromInputSlot (inputSlot);
}

bool NE::NodeManager::HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const
{
    return connectionManager.HasConnectedOutputSlots (inputSlot);
}

bool NE::NodeManager::HasConnectedInputSlots (const OutputSlotConstPtr& outputSlot) const
{
    return connectionManager.HasConnectedInputSlots (outputSlot);
}

size_t NE::NodeManager::GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const
{
    return connectionManager.GetConnectedOutputSlotCount (inputSlot);
}

size_t NE::NodeManager::GetConnectedInputSlotCount (const OutputSlotConstPtr& outputSlot) const
{
    return connectionManager.GetConnectedInputSlotCount (outputSlot);
}

void NE::NodeManager::EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const
{
    connectionManager.EnumerateConnectedOutputSlots (inputSlot, processor);
}

void NE::NodeManager::EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const
{
    connectionManager.EnumerateConnectedInputSlots (outputSlot, processor);
}

void NE::NodeManager::EnumerateConnections (const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const
{
    nodeList.Enumerate ([&] (const NodeConstPtr& node) {
        node->EnumerateOutputSlots ([&] (OutputSlotConstPtr outputSlot) {
            connectionManager.EnumerateConnectedInputSlots (outputSlot, [&] (const InputSlotConstPtr& inputSlot) {
                processor (outputSlot, inputSlot);
            });
            return true;
        });
        return true;
    });
}

void NE::NodeManager::EnumerateConnections (const NodeCollection& nodes, const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const
{
    nodes.Enumerate ([&] (const NodeId& nodeId) {
        NodeConstPtr node = GetNode (nodeId);
        node->EnumerateOutputSlots ([&] (OutputSlotConstPtr outputSlot) {
            connectionManager.EnumerateConnectedInputSlots (outputSlot, [&] (const InputSlotConstPtr& inputSlot) {
                if (nodes.Contains (inputSlot->GetOwnerNodeId ())) {
                    processor (outputSlot, inputSlot);
                }
            });
            return true;
        });
        return true;
    });
}

void NE::NodeManager::EvaluateAllNodes (EvaluationEnv& env)
{
    EnumerateNodes ([&] (NodePtr node) {
        if(node)
        {
            node->Tick(); // drive the tick function for all the nodes
        }
        return true;
    });

    EnumerateNodes ([&] (NodePtr node) {
        if(node)
        {
            if(node->NumberInputs() == 0) // start of a flow
            {
                node->Process(); // drive the tick function
            }
        }
        return true;
    });

}


void NE::NodeManager::EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const
{
    node->EnumerateOutputSlots ([&] (OutputSlotConstPtr outputSlot) {
        if (connectionManager.HasConnectedInputSlots (outputSlot)) {
            connectionManager.EnumerateConnectedInputSlots (outputSlot, [&] (const InputSlotConstPtr& inputSlot) {
                processor (inputSlot->GetOwnerNodeId ());
            });
        }
        return true;
    });
}

void NE::NodeManager::EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const
{
    EnumerateDependentNodes (node, [&] (const NodeId& dependentNodeId) {
        NodeConstPtr dependentNode = GetNode (dependentNodeId);
        processor (dependentNodeId);
        EnumerateDependentNodesRecursive (dependentNode, processor);
    });
}

void NE::NodeManager::EnumerateDependentNodes (const NodePtr& node, const std::function<void (const NodePtr&)>& processor)
{
    EnumerateDependentNodes (node, [&] (const NodeId& dependentNodeId) {
        processor (GetNode (dependentNodeId));
    });
}

void NE::NodeManager::EnumerateDependentNodesRecursive (const NodePtr& node, const std::function<void (const NodePtr&)>& processor)
{
    EnumerateDependentNodesRecursive (node, [&] (const NodeId& dependentNodeId) {
        processor (GetNode (dependentNodeId));
    });
}

void NE::NodeManager::EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const
{
    EnumerateDependentNodes (node, [&] (const NodeId& dependentNodeId) {
        processor (GetNode (dependentNodeId));
    });
}

void NE::NodeManager::EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const
{
    EnumerateDependentNodesRecursive (node, [&] (const NodeId& dependentNodeId) {
        processor (GetNode (dependentNodeId));
    });
}

bool NE::NodeManager::ContainsNodeGroup (const NodeGroupId& groupId) const
{
    return nodeGroupList.Contains (groupId);
}

NE::NodeGroupPtr NE::NodeManager::AddNodeGroup (const NodeGroupPtr& group)
{
    return AddNodeGroup (group, IdPolicy::GenerateNew);
}

void NE::NodeManager::MakeNodesAndGroupsSorted ()
{
    nodeList.MakeSorted ();
    nodeGroupList.MakeSorted ();
}

void NE::NodeManager::DeleteNodeGroup (const NodeGroupId& groupId)
{
    return nodeGroupList.DeleteGroup (groupId);
}

void NE::NodeManager::AddNodeToGroup (const NodeGroupId& groupId, const NodeId& nodeId)
{
    DBGASSERT (ContainsNode (nodeId));
    nodeGroupList.AddNodeToGroup (groupId, nodeId);
}

void NE::NodeManager::RemoveNodeFromGroup (const NodeId& nodeId)
{
    nodeGroupList.RemoveNodeFromGroup (nodeId);
}

NE::NodeGroupConstPtr NE::NodeManager::GetNodeGroup (const NodeId& nodeId) const
{
    return nodeGroupList.GetNodeGroup (nodeId);
}

const NE::NodeCollection& NE::NodeManager::GetGroupNodes (const NodeGroupId& groupId) const
{
    return nodeGroupList.GetGroupNodes (groupId);
}

void NE::NodeManager::EnumerateNodeGroups (const std::function<bool (NodeGroupConstPtr)>& processor) const
{
    nodeGroupList.Enumerate (processor);
}

void NE::NodeManager::EnumerateNodeGroups (const std::function<bool (NodeGroupPtr)>& processor)
{
    nodeGroupList.Enumerate (processor);
}

void NE::NodeManager::DeleteAllNodeGroups ()
{
    nodeGroupList.Clear ();
}


NE::NodeManager::UpdateMode NE::NodeManager::GetUpdateMode () const
{
    return updateMode;
}

void NE::NodeManager::SetUpdateMode (UpdateMode newUpdateMode)
{
    updateMode = newUpdateMode;
}

NE::Stream::Status NE::NodeManager::Read (InputStream& inputStream)
{
    return NodeManagerSerialization::Read (*this, inputStream);
}

NE::Stream::Status NE::NodeManager::Write (OutputStream& outputStream) const
{
    return NodeManagerSerialization::Write (*this, outputStream);
}

bool NE::NodeManager::Clone (const NodeManager& source, NodeManager& target)
{
    if (DBGERROR (!target.IsEmpty ())) {
        return false;
    }

    MemoryOutputStream outputStream;
    if (DBGERROR (source.Write (outputStream) != Stream::Status::NoError)) {
        return false;
    }

    MemoryInputStream inputStream (outputStream.GetBuffer ());
    if (DBGERROR (target.Read (inputStream) != Stream::Status::NoError)) {
        return false;
    }

    return true;
}

bool NE::NodeManager::ReadFromBuffer (NodeManager& nodeManager, const std::vector<char>& buffer)
{
    MemoryInputStream inputStream (buffer);
    if (DBGERROR (nodeManager.Read (inputStream) != Stream::Status::NoError)) {
        return false;
    }
    return true;
}

bool NE::NodeManager::WriteToBuffer (const NodeManager& nodeManager, std::vector<char>& buffer)
{
    MemoryOutputStream outputStream;
    if (DBGERROR (nodeManager.Write (outputStream) != Stream::Status::NoError)) {
        return false;
    }
    buffer = outputStream.GetBuffer ();
    return true;
}

NE::NodePtr NE::NodeManager::AddNode (const NodePtr& node, IdPolicy idPolicy, InitPolicy initPolicy)
{

    NodeId nodeId;
    if (idPolicy == IdPolicy::KeepOriginal) {
        nodeId = node->GetId ();
    } else if (idPolicy == IdPolicy::GenerateNew) {
        nodeId = idGenerator.GenerateNodeId ();
    } else {
        DBGBREAK ();
        return nullptr;
    }

    if (DBGERROR (ContainsNode (nodeId))) {
        return nullptr;
    }

    node->SetId (nodeId);
    node->SetNodeManager (this);
    if (initPolicy == InitPolicy::Initialize) {
        node->Initialize ();
    }

    if (DBGERROR (!nodeList.AddNode (node->GetId (), node))) {
        return nullptr;
    }

    return node;
}

NE::NodeGroupPtr NE::NodeManager::AddNodeGroup (const NodeGroupPtr& group, IdPolicy idHandling)
{
    if (DBGERROR (group == nullptr)) {
        return nullptr;
    }

    NodeGroupId groupId;
    if (idHandling == IdPolicy::KeepOriginal) {
        groupId = group->GetId ();
    } else if (idHandling == IdPolicy::GenerateNew) {
        groupId = idGenerator.GenerateNodeGroupId ();
    } else {
        DBGBREAK ();
        return nullptr;
    }

    if (DBGERROR (ContainsNodeGroup (groupId))) {
        return nullptr;
    }

    group->SetId (groupId);
    if (DBGERROR (!nodeGroupList.AddGroup (group))) {
        return nullptr;
    }

    return group;
}


bool NE::NodeManager::Begin() // gather the source node list nodes
{
    return true;
}

bool NE::NodeManager::Step()  // iterate and call Tick functions
{
    return true;
}

bool NE::NodeManager::End()
{
    return true;
}

