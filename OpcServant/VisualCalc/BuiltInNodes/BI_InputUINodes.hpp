#ifndef BI_INPUTUINODES_HPP
#define BI_INPUTUINODES_HPP

#include "../NodeEngine/NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_UINodeLayouts.hpp"
#include "BI_UINodePanels.hpp"
#include "../NodeEngine/NE_Localization.hpp"
#include "../NodeUIEngine/NUIE_NodeParameters.hpp"
#include "../NodeUIEngine/NUIE_NodeCommonParameters.hpp"
#include "../NodeUIEngine/NUIE_NodeUIManager.hpp"
#include "../NodeUIEngine/NUIE_SkinParams.hpp"

#include <cmath>
#include <algorithm>
#include <chrono>


namespace BI
{

// defines the UI to set values
template<typename NODETYPE, typename VALUETYPE >
class ValueParameter : public NUIE::TypedNodeParameter<NODETYPE, VALUETYPE>
{
public:
    ValueParameter (const NUIE::ParameterType& type , const std::wstring &s = L"Value" ) :
         NUIE::TypedNodeParameter<NODETYPE, VALUETYPE>(NE::LocString (s), type)
    {

    }

    virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
    {
        return NE::ValuePtr (new VALUETYPE (NE::Node::CastConst<NODETYPE>(uiNode)->GetValue ()));
    }

    virtual bool SetValueInternal (NUIE::UINodeInvalidator& /*invalidator*/, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
    {
        NE::Node::Cast<NODETYPE>(uiNode)->SetValue (VALUETYPE::Get (value));
        return true;
    }
};



template <typename T>
class ValueNode  : public BasicUINode
{
    T val ;
public:
    typedef ValueNode<T> ValueNodeType;
    ValueNode() : ValueNode (NE::LocString (), NUIE::Point (),T(0)) {}
    ValueNode(const NE::LocString& name, const NUIE::Point& position, T v) :
        BasicUINode (name, position),val(v) {}

    virtual void	Initialize () override
    {
        RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("value"), NE::LocString (L"Value"))));
    }


    virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override
    {
        NE::ObjectHeader header (inputStream);
        BasicUINode::Read (inputStream);
        inputStream.Read (val);
        return inputStream.GetStatus ();
    }

    virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override
    {
        BasicUINode::Write (outputStream);
        outputStream.Write (val);
        return outputStream.GetStatus ();
    }

    T	GetValue () const {
        return val;
    }

    void	SetValue (T newVal) {
        val = newVal;
    }

    virtual void  Process()
    {
        JSONVALUEPTR vo = std::make_unique<Json::Value>(val);
        Post("value",vo);
    }

};

//
// Macro to define the class
// Keeps definitons down

#define VALUENODEDEF(n,t,v,p)\
class n : public ValueNode<t>\
{\
    DYNAMIC_SERIALIZABLE (n)\
public:\
        n () : ValueNode<t>() {}\
        n (const NE::LocString &name, const NUIE::Point& position, bool f) :  ValueNode<t> ( name,  position,f) {}\
    typedef ValueParameter<n, v> PARAMETER;\
    void RegisterParameters (NUIE::NodeParameterList& parameterList) const\
    {\
        BasicUINode::RegisterParameters (parameterList);\
        parameterList.AddParameter (NUIE::NodeParameterPtr (new PARAMETER (p)));\
    }\
};

VALUENODEDEF(BooleanNode,bool,NE::BooleanValue, NUIE::ParameterType::Boolean)
VALUENODEDEF(IntegerNode,int,NE::IntValue, NUIE::ParameterType::Integer)
VALUENODEDEF(DoubleNode,double,NE::DoubleValue, NUIE::ParameterType::Double)
VALUENODEDEF(UnsignedNode,unsigned,NE::UnsignedValue, NUIE::ParameterType::Unsigned)

/*!
 * \brief The Timer class
 * Generates timed events
 */
class TimerNode : public ValueNode<int>
{
    DYNAMIC_SERIALIZABLE (TimerNode)
    std::chrono::milliseconds _lastTime; // last event time
    int count = 0;
public:
        TimerNode() : ValueNode<int>()
        {
            _lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        }
        TimerNode (const NE::LocString &name, const NUIE::Point& position, bool f) :  ValueNode<int> ( name,  position,f)
        {
            _lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        }
    typedef ValueParameter<TimerNode, NE::IntValue> PARAMETER;
    void RegisterParameters (NUIE::NodeParameterList& parameterList) const
    {
        BasicUINode::RegisterParameters (parameterList);
        parameterList.AddParameter (NUIE::NodeParameterPtr (new PARAMETER (NUIE::ParameterType::Integer)));
    }
    /*!
     * \brief Process
     * Check the timer
     */
    void Process()
    {
        std::chrono::milliseconds n = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        unsigned  diff = unsigned(n.count() - _lastTime.count());
        if(diff >= GetValue())
        {
            _lastTime = n;
            JSONVALUEPTR vo = std::make_unique<Json::Value>(count++);
            Post("value",vo);
        }
    }
};

/*!
 * \brief The EventNode class
 * Base class for event handlers
 */
class EventNode : public ValueNode<bool>
{
    DYNAMIC_SERIALIZABLE (EventNode)
    int count = 0;

public:
        EventNode() : ValueNode<bool>() {}
        EventNode (const NE::LocString &name, const NUIE::Point& position, bool f) :  ValueNode<bool> ( name,  position,f) {}
    typedef ValueParameter<TimerNode, NE::BooleanValue> PARAMETER;
    void RegisterParameters (NUIE::NodeParameterList& parameterList) const
    {
        BasicUINode::RegisterParameters (parameterList);
        parameterList.AddParameter (NUIE::NodeParameterPtr (new PARAMETER (NUIE::ParameterType::Boolean)));
    }
     void Process()
     {
         if(GetValue())
         {
             JSONVALUEPTR vo = std::make_unique<Json::Value>(count++);
             Post("value",vo);
         }
     }
};


/*!
 * \brief The OutputNode class
 */
class OutputNode  : public BasicUINode
{
    DYNAMIC_SERIALIZABLE (OutputNode)
public:
    OutputNode() : OutputNode (NE::LocString (), NUIE::Point ()) {}
    OutputNode(const NE::LocString& name, const NUIE::Point& position) :
        BasicUINode (name, position) {}

    virtual void	Initialize () override
    {
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), NE::LocString (L"Input"), NE::OutputSlotConnectionMode::Multiple)));
    }

    virtual void	RegisterParameters (NUIE::NodeParameterList& parameterList) const override
    {
        BasicUINode::RegisterParameters ( parameterList);
    }

    virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override
    {
        NE::ObjectHeader header (inputStream);
        BasicUINode::Read (inputStream);
        return inputStream.GetStatus ();
    }

    virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override
    {
        BasicUINode::Write (outputStream);
        return outputStream.GetStatus ();
    }
    virtual void Process(const NE::InputSlotConstPtr &in, JSONVALUEPTR &v)
    {
        // default behaviour is to trace the request
        try
        {
            if(v)
            {
                std::cerr << " Input Slot:" << in->GetId().Id() <<   " Output:" << *v << std::endl;
            }
            else
            {
                std::cerr << "Null value" << std::endl;
            }
        }
        catch(...)
        {
            std::cerr << "Exception" << std::endl;
        }
    }
};


}

#endif
