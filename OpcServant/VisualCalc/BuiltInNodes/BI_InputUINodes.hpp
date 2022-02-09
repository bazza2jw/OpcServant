#ifndef BI_INPUTUINODES_HPP
#define BI_INPUTUINODES_HPP

#include "../NodeEngine/NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_UINodeLayouts.hpp"

namespace BI
{


template <typename T>
class ValueNode  : public BasicUINode
{
    T val ;
public:
    ValueNode() : ValueNode (NE::LocString (), NUIE::Point (),T(0)) {}
    ValueNode(const NE::LocString& name, const NUIE::Point& position, T v) :
        BasicUINode (name, position),val(v) {}

    virtual void	Initialize () override
    {
        RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("value"), NE::LocString (L"Value"))));
    }

    virtual void	RegisterParameters (NUIE::NodeParameterList& parameterList) const override
    {
        BasicUINode::RegisterParameters ( parameterList);
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

    virtual void  Process() const
    {
        JSONVALUEPTR vo = std::make_unique<Json::Value>(val);
        Post("value",vo);
    }

};

class BooleanNode : public ValueNode<bool>
{
    DYNAMIC_SERIALIZABLE (BooleanNode)
public:
        BooleanNode () : ValueNode<bool>() {}
        BooleanNode (const NE::LocString &name, const NUIE::Point& position, bool f) :  ValueNode<bool> ( name,  position,f) {}
        virtual void Tick()
        {
            std::cerr << "Tick" << std::endl;
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
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), NE::LocString (L"Input"), NE::OutputSlotConnectionMode::Single)));
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
