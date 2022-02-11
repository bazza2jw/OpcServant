#ifndef BI_BINARYOPERATIONNODES_HPP
#define BI_BINARYOPERATIONNODES_HPP

#include "../NodeEngine/NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"
#include <functional>
#include "BI_InputUINodes.hpp"
namespace BI
{
template <typename A, typename R>
class BaseOperationNode : public BasicUINode
{

    A _a;
    A _b;
    bool _aSet = false;
    bool _bSet = false;
    bool _reArmOnTick = true;
public:
    BaseOperationNode () :
        BaseOperationNode (NE::LocString (), NUIE::Point ())
    {

    }

    BaseOperationNode (const NE::LocString& name, const NUIE::Point& position) :
        BasicUINode (name, position)
    {
    }

    virtual ~BaseOperationNode () {}

    virtual void Initialize () override
    {
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), NE::LocString (L"A"),
                                                                          NE::OutputSlotConnectionMode::Single)));
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), NE::LocString (L"B"),
                                                                          NE::OutputSlotConnectionMode::Single)));
        RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"),
                                                                             NE::LocString (L"Result"))));
    }

    void Tick()
    {
        if(_reArmOnTick)
        {
            _aSet = _bSet = false; // need both to have been updated before generating event
        }
    }
    void Process(const NE::InputSlotConstPtr &in, JSONVALUEPTR &v)
    {
        try
        {
            if(v)
            {
                if(v->is<A>())
                {
                    if((in->GetId().Id() == "a"))
                    {
                        _a = v->as<A>();
                        _aSet = true;
                    }
                    else if(in->GetId().Id() == "b")
                    {
                        _b = v->as<A>();
                        _bSet = true;
                    }
                    if(_aSet && _bSet)
                    {
                        JSONVALUEPTR vo = std::make_unique<Json::Value>(Op(_a,_b));
                        Post("result",vo); // propogate forwards
                    }
                }
            }
        }
        catch(...)
        {

        }
    }

    virtual R Op(A , A) const { return R(0);}

    virtual void RegisterParameters (NUIE::NodeParameterList& parameterList) const override
    {

        BasicUINode::RegisterParameters (parameterList);
    }

    virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override
    {
        NE::ObjectHeader header (inputStream);
        BasicUINode::Read (inputStream);
        return inputStream.GetStatus ();

    }
    virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override
    {
        NE::ObjectHeader header (outputStream, serializationInfo);
        BasicUINode::Write (outputStream);
        return outputStream.GetStatus ();
    }
};


class BinaryOperationNode : public BaseOperationNode<double,double>
{
public:
    BinaryOperationNode () :
        BaseOperationNode (NE::LocString (), NUIE::Point ())
    {

    }

    BinaryOperationNode (const NE::LocString& name, const NUIE::Point& position) :
        BaseOperationNode (name, position)
    {

    }
    virtual double Op (double a, double b) const = 0; // bools are not an internal type
};

#define BINARYOPNODE(n,o) \
class n : public BinaryOperationNode\
{\
     DYNAMIC_SERIALIZABLE (n)\
public:\
    n() : BinaryOperationNode() {}\
    n(const NE::LocString& name, const NUIE::Point& position) : BinaryOperationNode ( name, position) {}\
    double Op(double a, double b) const { return a o b;}\
};

BINARYOPNODE(AdditionNode,+)
BINARYOPNODE(SubtractionNode,-)
BINARYOPNODE(MultiplicationNode,*)

class DivisionNode : public BinaryOperationNode
{
     DYNAMIC_SERIALIZABLE (DivisionNode)
public:
    DivisionNode() : BinaryOperationNode() {}
    DivisionNode(const NE::LocString& name, const NUIE::Point& position) : BinaryOperationNode ( name, position) {}
    double Op(double a, double b) const { return (a != 0)? (a / b):0.0;}
};



#define COMPOPNODE(n,o) \
class n : public ComparisonOperationNode\
{\
     DYNAMIC_SERIALIZABLE (n)\
public:\
    n() : ComparisonOperationNode() {}\
    n(const NE::LocString& name, const NUIE::Point& position) : ComparisonOperationNode( name, position) {}\
    bool Op(double a, double b) const { return a o b;}\
};

class ComparisonOperationNode :  public BaseOperationNode<double,bool>
{
public:
    ComparisonOperationNode () : BaseOperationNode<double,bool>() {}
    ComparisonOperationNode (const NE::LocString& name, const NUIE::Point& position) : BaseOperationNode<double,bool>(name,position) {}
private:
    virtual bool Op (double a, double b) const = 0; // bools are not an internal type
};

COMPOPNODE(GreaterThanNode,<)
COMPOPNODE(LessThanNode,>)
COMPOPNODE(EqualsNode,==)
COMPOPNODE(NotEqualsNode,!=)


#define LOGICOPNODE(n,o) \
class n : public LogicOperationNode\
{\
     DYNAMIC_SERIALIZABLE (n)\
public:\
    n() : LogicOperationNode() {}\
    n(const NE::LocString& name, const NUIE::Point& position) : LogicOperationNode( name, position) {}\
    bool Op(bool a, bool b) const { return a o b;}\
};


class LogicOperationNode : public BaseOperationNode<bool,bool>
{

public:
    LogicOperationNode () : BaseOperationNode<bool,bool>() {}
    LogicOperationNode (const NE::LocString& name, const NUIE::Point& position) : BaseOperationNode<bool,bool>(name,position) {}
private:
    virtual bool Op (bool a, bool b) const = 0;
};


LOGICOPNODE(AndNode, &&)
LOGICOPNODE(OrNode, ||)
LOGICOPNODE(XorNode, ^)



#define BITOPNODE(n,o) \
class n : public BitOperationNode\
{\
     DYNAMIC_SERIALIZABLE (n)\
public:\
    n() : BitOperationNode() {}\
    n(const NE::LocString& name, const NUIE::Point& position) : BitOperationNode( name, position) {}\
    virtual unsigned Op(unsigned a, unsigned b) const  { return a o b;}\
};

class BitOperationNode :  public BaseOperationNode<unsigned, unsigned>
{

public:
    BitOperationNode () {}
    BitOperationNode (const NE::LocString& name, const NUIE::Point& position) : BaseOperationNode<unsigned, unsigned>(name,position) {}
private:
    virtual unsigned Op (unsigned a, unsigned b) const = 0;
};

BITOPNODE(BitAndNode, &)
BITOPNODE(BitOrNode, |)
BITOPNODE(BitXorNode, ^)

}

#endif
