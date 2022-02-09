#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_Debug.hpp"
#include "NE_MemoryStream.hpp"
#include "NE_NodeManager.hpp"

namespace NE
{

SERIALIZATION_INFO (Node, 1);


Node::Node () :
    nodeId (NullNodeId),
	inputSlots (),
    outputSlots ()

{

}

Node::~Node ()
{

}

bool Node::IsEmpty () const
{
    return (nodeId == NullNodeId) && inputSlots.IsEmpty () && outputSlots.IsEmpty ();
}

const NodeId& Node::GetId () const
{
	return nodeId;
}

bool Node::HasInputSlot (const SlotId& slotId) const
{
	return inputSlots.Contains (slotId);
}

bool Node::HasOutputSlot (const SlotId& slotId) const
{
	return outputSlots.Contains (slotId);
}


InputSlotConstPtr Node::GetInputSlot (const SlotId& slotId) const
{
	return inputSlots.Get (slotId);
}

OutputSlotConstPtr Node::GetOutputSlot (const SlotId& slotId) const
{
	return outputSlots.Get (slotId);
}

size_t Node::GetInputSlotCount () const
{
	return inputSlots.Count ();
}

size_t Node::GetOutputSlotCount () const
{
	return outputSlots.Count ();
}

void Node::EnumerateInputSlots (const std::function<bool (InputSlotPtr)>& processor)
{
	inputSlots.Enumerate (processor);
}

void Node::EnumerateOutputSlots (const std::function<bool (OutputSlotPtr)>& processor)
{
	outputSlots.Enumerate (processor);
}

void Node::EnumerateInputSlots (const std::function<bool (InputSlotConstPtr)>& processor) const
{
	inputSlots.Enumerate (processor);
}

void Node::EnumerateOutputSlots (const std::function<bool (OutputSlotConstPtr)>& processor) const
{
	outputSlots.Enumerate (processor);
}




Stream::Status Node::Read (InputStream& inputStream)
{
	if (DBGERROR (!IsEmpty ())) {
		return Stream::Status::Error;
	}

	ObjectHeader header (inputStream);
	nodeId.Read (inputStream);
	size_t inputSlotCount = 0;
	inputStream.Read (inputSlotCount);
	for (size_t i = 0; i < inputSlotCount; ++i) {
		InputSlotPtr inputSlot (ReadDynamicObject<InputSlot> (inputStream));
		if (DBGERROR (inputSlot == nullptr)) {
			return Stream::Status::Error;
		}
		if (DBGERROR (!RegisterInputSlot (inputSlot))) {
			return Stream::Status::Error;
		}
	}
	size_t outputSlotCount = 0;
	inputStream.Read (outputSlotCount);
	for (size_t i = 0; i < outputSlotCount; ++i) {
		OutputSlotPtr outputSlot (ReadDynamicObject<OutputSlot> (inputStream));
		if (DBGERROR (outputSlot == nullptr)) {
			return Stream::Status::Error;
		}
		if (DBGERROR (!RegisterOutputSlot (outputSlot))) {
			return Stream::Status::Error;
		}
	}
	return inputStream.GetStatus ();
}

Stream::Status Node::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	nodeId.Write (outputStream);
	outputStream.Write (inputSlots.Count ());
	inputSlots.Enumerate ([&] (const InputSlotConstPtr& inputSlot) {
		WriteDynamicObject (outputStream, inputSlot.get ());
		return true;
	});
	outputStream.Write (outputSlots.Count ());
	outputSlots.Enumerate ([&] (const OutputSlotConstPtr& outputSlot) {
		WriteDynamicObject (outputStream, outputSlot.get ());
		return true;
	});
	return outputStream.GetStatus ();
}


bool Node::RegisterInputSlot (const InputSlotPtr& newInputSlot)
{
	if (DBGERROR (inputSlots.Contains (newInputSlot->GetId ()))) {
		return false;
	}
	if (DBGERROR (newInputSlot->HasOwnerNode ())) {
		return false;
	}
	if (DBGERROR (!inputSlots.Insert (newInputSlot))) {
		return false;
	}
	if (DBGERROR (!newInputSlot->SetOwnerNode (this))) {
		return false;
	}
	return true;
}

bool Node::RegisterOutputSlot (const OutputSlotPtr& newOutputSlot)
{
	if (DBGERROR (outputSlots.Contains (newOutputSlot->GetId ()))) {
		return false;
	}
	if (DBGERROR (newOutputSlot->HasOwnerNode ())) {
		return false;
	}
	if (DBGERROR (!outputSlots.Insert (newOutputSlot))) {
		return false;
	}
	if (DBGERROR (!newOutputSlot->SetOwnerNode (this))) {
		return false;
	}
	return true;
}


void Node::SetId (const NodeId& newNodeId)
{
	nodeId = newNodeId;
}


void Node::Process(const InputSlotConstPtr &, JSONVALUEPTR &) // periodic processing - for example triggering a timer or reading an event queue
{
    // process the data and post to outputs as required
}

void  Node::Post(const std::string &out, JSONVALUEPTR &v) const // send a value to an output slot
{
    if(nodeManager)
    {
        // find the slot
        SlotId oid(out);
        if(outputSlots.Contains(oid))
        {
            // iterate the connections to the slot
            OutputSlotConstPtr o = outputSlots.Get(oid); // get reference to slot
            //
            if(o)
            {
            // iterate the connected inputs
                if(nodeManager->HasConnectedInputSlots(o))
                {
                    nodeManager->EnumerateConnectedInputSlots (o, [&] (const InputSlotConstPtr&p)
                    {
                         NodePtr n =  nodeManager->GetNode(p->GetOwnerNodeId());
                         if(n)
                         {
                            n->Process(p,v); // this is recursive - ends when the flow ends
                         }
                    });
                }
            }
        }
    }
}



NodePtr Node::Clone (const NodeConstPtr& node)
{
	MemoryOutputStream outputStream;
	if (DBGERROR (!WriteDynamicObject (outputStream, node.get ()))) {
		return nullptr;
	}

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	NodePtr result (ReadDynamicObject<Node> (inputStream));
	if (DBGERROR (result == nullptr)) {
		return nullptr;
	}

	return result;
}

bool Node::IsEqual (const NodeConstPtr& aNode, const NodeConstPtr& bNode)
{
	MemoryOutputStream aStream;
	MemoryOutputStream bStream;

	aNode->Write (aStream);
	bNode->Write (bStream);

	return aStream.GetBuffer () == bStream.GetBuffer ();
}

}
