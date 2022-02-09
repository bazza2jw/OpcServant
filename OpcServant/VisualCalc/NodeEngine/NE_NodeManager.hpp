#ifndef NE_NODEMANAGER_HPP
#define NE_NODEMANAGER_HPP

#include "NE_NodeEngineTypes.hpp"
#include "NE_Serializable.hpp"
#include "NE_Node.hpp"
#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"
#include "NE_Value.hpp"
#include "NE_ConnectionInfo.hpp"
#include "NE_ConnectionManager.hpp"
#include "NE_NodeList.hpp"
#include "NE_NodeGroupList.hpp"
#include "NE_NodeValueCache.hpp"
#include "NE_UniqueIdGenerator.hpp"
#include <functional>
#include <set>

namespace NE
{

class OutputSlotList
{
public:
	OutputSlotList ();
	virtual ~OutputSlotList ();

	bool			IsEmpty () const;
	virtual size_t	GetSize () const = 0;
	virtual void	Enumerate (const std::function<bool (OutputSlotConstPtr)>& processor) const = 0;
};

class InputSlotList
{
public:
	InputSlotList ();
	virtual ~InputSlotList ();

	bool			IsEmpty () const;
	virtual size_t	GetSize () const = 0;
	virtual void	Enumerate (const std::function<bool (InputSlotConstPtr)>& processor) const = 0;
};




class NodeManager
{
	SERIALIZABLE;
	friend class NodeManagerMerge;
	friend class NodeManagerSerialization;

public:
	enum class UpdateMode
	{
		Automatic	= 0,
		Manual		= 1
	};

	NodeManager ();
	NodeManager (const NodeManager& src) = delete;
	NodeManager (NodeManager&& src) = delete;
    virtual ~NodeManager ();

	NodeManager&			operator= (const NodeManager& rhs) = delete;
	NodeManager&			operator= (NodeManager&& rhs) = delete;

	void					Clear ();
	bool					IsEmpty () const;
	size_t					GetNodeCount () const;
	size_t					GetNodeGroupCount () const;
	size_t					GetConnectionCount () const;

	void					EnumerateNodes (const std::function<bool (NodePtr)>& processor);
	void					EnumerateNodes (const std::function<bool (NodeConstPtr)>& processor) const;

	bool					ContainsNode (const NodeId& id) const;
	NodeConstPtr			GetNode (const NodeId& id) const;

	NodePtr					GetNode (const NodeId& id);
	NodePtr					AddNode (const NodePtr& node);
	bool					DeleteNode (const NodeId& id);
	bool					DeleteNode (const NodePtr& node);

	bool					IsOutputSlotConnectedToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	bool					CanConnectOutputSlotToInputSlot (const InputSlotConstPtr& inputSlot) const;
	bool					CanConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot) const;
	bool					CanConnectOutputSlotsToInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot) const;
	bool					CanConnectOutputSlotToInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots) const;

	bool					ConnectOutputSlotToInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	bool					ConnectOutputSlotsToInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot);
	bool					ConnectOutputSlotToInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots);
	bool					DisconnectOutputSlotFromInputSlot (const OutputSlotConstPtr& outputSlot, const InputSlotConstPtr& inputSlot);
	bool					DisconnectOutputSlotsFromInputSlot (const OutputSlotList& outputSlots, const InputSlotConstPtr& inputSlot);
	bool					DisconnectOutputSlotFromInputSlots (const OutputSlotConstPtr& outputSlot, const InputSlotList& inputSlots);
	bool					DisconnectAllInputSlotsFromOutputSlot (const OutputSlotConstPtr& outputSlot);
	bool					DisconnectAllOutputSlotsFromInputSlot (const InputSlotConstPtr& inputSlot);

	bool					HasConnectedOutputSlots (const InputSlotConstPtr& inputSlot) const;
	bool					HasConnectedInputSlots (const OutputSlotConstPtr& outputSlot) const;
	size_t					GetConnectedOutputSlotCount (const InputSlotConstPtr& inputSlot) const;
	size_t					GetConnectedInputSlotCount (const OutputSlotConstPtr& outputSlot) const;
	void					EnumerateConnectedOutputSlots (const InputSlotConstPtr& inputSlot, const std::function<void (const OutputSlotConstPtr&)>& processor) const;
	void					EnumerateConnectedInputSlots (const OutputSlotConstPtr& outputSlot, const std::function<void (const InputSlotConstPtr&)>& processor) const;
	void					EnumerateConnections (const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const;
	void					EnumerateConnections (const NodeCollection& nodes, const std::function<void (const OutputSlotConstPtr&, const InputSlotConstPtr&)>& processor) const;

    void					EvaluateAllNodes (EvaluationEnv& env) ;
	
	void					EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const;
	void					EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeId&)>& processor) const;

	void					EnumerateDependentNodes (const NodePtr& node, const std::function<void (const NodePtr&)>& processor);
	void					EnumerateDependentNodesRecursive (const NodePtr& node, const std::function<void (const NodePtr&)>& processor);

	void					EnumerateDependentNodes (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const;
	void					EnumerateDependentNodesRecursive (const NodeConstPtr& node, const std::function<void (const NodeConstPtr&)>& processor) const;

	bool					ContainsNodeGroup (const NodeGroupId& groupId) const;
	
	NodeGroupPtr			AddNodeGroup (const NodeGroupPtr& group);
	void					DeleteNodeGroup (const NodeGroupId& groupId);
	void					AddNodeToGroup (const NodeGroupId& groupId, const NodeId& nodeId);
	void					RemoveNodeFromGroup (const NodeId& nodeId);

	NodeGroupConstPtr		GetNodeGroup (const NodeId& nodeId) const;
	const NodeCollection&	GetGroupNodes (const NodeGroupId& groupId) const;

	void					EnumerateNodeGroups (const std::function<bool (NodeGroupConstPtr)>& processor) const;
	void					EnumerateNodeGroups (const std::function<bool (NodeGroupPtr)>& processor);
	void					DeleteAllNodeGroups ();

	UpdateMode				GetUpdateMode () const;
	void					SetUpdateMode (UpdateMode newUpdateMode);

	Stream::Status			Read (InputStream& inputStream);
	Stream::Status			Write (OutputStream& outputStream) const;



	static bool				Clone (const NodeManager& source, NodeManager& target);
	static bool				ReadFromBuffer (NodeManager& nodeManager, const std::vector<char>& buffer);
	static bool				WriteToBuffer (const NodeManager& nodeManager, std::vector<char>& buffer);

    /*
     * These are the routines that drive the flows
     * All flows start with a source node - Source nodes are evaluated and either do nothing or post a value to other nodes
     * Source nodes only have outputs, they do not have inputs, there fore flows cannot loop
     *
     * Flows run from input (source) to output (destination)
     * For each node each output acts as a source and so iterates
     */
     //
     virtual bool Begin(); // gather the source node list nodes
     virtual bool Step();  // iterate one flow step
     virtual bool End();
     //

private:
	enum class IdPolicy
	{
		KeepOriginal,
		GenerateNew
	};

	enum class InitPolicy
	{
		Initialize,
		DoNotInitialize
	};

	NodePtr				AddNode (const NodePtr& node, IdPolicy idHandling, InitPolicy initPolicy);
	NodeGroupPtr		AddNodeGroup (const NodeGroupPtr& group, IdPolicy idHandling);
	void				MakeNodesAndGroupsSorted ();

	UniqueIdGenerator						idGenerator;
	NodeList								nodeList;
	ConnectionManager						connectionManager;
	NodeGroupList							nodeGroupList;
	UpdateMode								updateMode;

    NodeList                                sourceNodes; // set of source nodes

};

}

#endif
