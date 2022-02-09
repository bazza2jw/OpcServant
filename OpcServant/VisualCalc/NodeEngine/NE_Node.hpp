#ifndef NE_NODE_HPP
#define NE_NODE_HPP

#include "NE_Serializable.hpp"
#include "NE_NodeId.hpp"
#include "NE_SlotId.hpp"
#include "NE_SlotList.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_Value.hpp"
#include "NE_EvaluationEnv.hpp"
#include "NE_NodeValueCache.hpp"

#include <memory>
#include <functional>
#include <unordered_set>
// use JSON values to pass values through flows - these cover most needs but are only one value
#include <MrlLib/json/value.h>
typedef std::unique_ptr<Json::Value> JSONVALUEPTR;

namespace NE
{
class NodeManager;

class Node : public DynamicSerializable
{
    SERIALIZABLE;
    friend class NodeManager;

public:
    enum class CalculationStatus
    {
        Calculated,
        NeedToCalculate,
        NeedToCalculateButDisabled
    };

    Node ();
    Node (const Node& src) = delete;
    virtual ~Node ();

    bool					IsEmpty () const;
    const NodeId&			GetId () const;

    void SetNodeManager(NodeManager * p) {
        nodeManager = p;
    }
    NodeManager * GetNodeManager() const {
        return nodeManager;
    }

    bool					HasInputSlot (const SlotId& slotId) const;
    bool					HasOutputSlot (const SlotId& slotId) const;

    InputSlotConstPtr		GetInputSlot (const SlotId& slotId) const;
    OutputSlotConstPtr		GetOutputSlot (const SlotId& slotId) const;

    size_t					GetInputSlotCount () const;
    size_t					GetOutputSlotCount () const;

    void					EnumerateInputSlots (const std::function<bool (InputSlotPtr)>& processor);
    void					EnumerateOutputSlots (const std::function<bool (OutputSlotPtr)>& processor);

    void					EnumerateInputSlots (const std::function<bool (InputSlotConstPtr)>& processor) const;
    void					EnumerateOutputSlots (const std::function<bool (OutputSlotConstPtr)>& processor) const;

    virtual Stream::Status	Read (InputStream& inputStream) override;
    virtual Stream::Status	Write (OutputStream& outputStream) const override;

    static NodePtr			Clone (const NodeConstPtr& node);
    static bool				IsEqual (const NodeConstPtr& aNode, const NodeConstPtr& bNode);

    template <class Type>
    static bool IsType (Node* node);

    template <class Type>
    static bool IsType (const NodePtr& node);

    template <class Type>
    static bool IsTypeConst (const NodeConstPtr& node);

    template <class Type>
    static Type* Cast (Node* node);

    template <class Type>
    static std::shared_ptr<Type> Cast (const NodePtr& node);

    template <class Type>
    static std::shared_ptr<const Type> CastConst (const NodeConstPtr& node);

    virtual void			Initialize () = 0;    // initialise a node - get parameters in locally clear any states, claim resources
    virtual void            Process(void) const {}
    virtual void            Process(const InputSlotConstPtr &in, JSONVALUEPTR &v) ; // periodic processing - for example triggering a timer or reading an event queue
    virtual void            Post(const std::string &out, JSONVALUEPTR &v) const; // send a value to an output slot
    virtual void            Tick()  {} // periodic processing
    size_t                  NumberInputs() const { return inputSlots.Count();}
    size_t                  NumberOutputs() const { return outputSlots.Count();}

protected:
    bool					RegisterInputSlot (const InputSlotPtr& newInputSlot);
    bool					RegisterOutputSlot (const OutputSlotPtr& newOutputSlot);
    void					SetId (const NodeId& newNodeId);



    NodeId					nodeId;
    SlotList<InputSlot>		inputSlots;
    SlotList<OutputSlot>	outputSlots;
    NodeManager  *      nodeManager = nullptr;
};

template <class Type>
bool Node::IsType (Node* node)
{
    return dynamic_cast<Type*> (node) != nullptr;
}

template <class Type>
bool Node::IsType (const NodePtr& node)
{
    return dynamic_cast<Type*> (node.get ()) != nullptr;
}

template <class Type>
bool Node::IsTypeConst (const NodeConstPtr& node)
{
    return dynamic_cast<const Type*> (node.get ()) != nullptr;
}

template <class Type>
Type* Node::Cast (Node* node)
{
    return dynamic_cast<Type*> (node);
}

template <class Type>
std::shared_ptr<Type> Node::Cast (const NodePtr& node)
{
    return std::dynamic_pointer_cast<Type> (node);
}

template <class Type>
std::shared_ptr<const Type> Node::CastConst (const NodeConstPtr& node)
{
    return std::dynamic_pointer_cast<const Type> (node);
}

}

#endif
