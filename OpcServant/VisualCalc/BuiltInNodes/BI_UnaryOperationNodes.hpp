#ifndef BI_UNARYOPERATIONNODES_HPP
#define BI_UNARYOPERATIONNODES_HPP

#include "../NodeEngine/NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "../NodeEngine/NE_InputSlot.hpp"
#include <math.h>

namespace BI
{
template <typename T>
    class BaseUnaryOperationNode : public BasicUINode
    {
    public:
        BaseUnaryOperationNode () :
            BasicUINode (NE::LocString (), NUIE::Point ()) {}
        BaseUnaryOperationNode (const NE::LocString& name, const NUIE::Point& position)
            : BasicUINode (name, position) {}
        virtual ~BaseUnaryOperationNode () {}
        virtual void Initialize ()
        {
            RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), NE::LocString (L"IN"), NE::OutputSlotConnectionMode::Single)));
            RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::LocString (L"OUT"))));
        }

        virtual void RegisterParameters(NUIE::NodeParameterList& parameterList) const
        {
            BasicUINode::RegisterParameters (parameterList);
        }

        void Process(const NE::InputSlotConstPtr &, JSONVALUEPTR &v)
        {
            try
            {
                if(v)
                {
                    JSONVALUEPTR res  = std::make_unique<Json::Value>(Op(v->as<T>()));
                    Post("out",res);
                }
            }
            catch(...)
            {

            }
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

        virtual T Op(T a) const { return  T(0);}
    };



class UnaryOperationNode : public BaseUnaryOperationNode<double>
{
    SERIALIZABLE

public:
    UnaryOperationNode () :  UnaryOperationNode (NE::LocString (), NUIE::Point ()) {}
    UnaryOperationNode (const NE::LocString& name, const NUIE::Point& position) : BaseUnaryOperationNode<double> (name,position) {}
    double Op(double ) const { return 0.0;}
};

#define UNARYOPNODE(n,o,v) \
class n : public UnaryOperationNode\
{\
     DYNAMIC_SERIALIZABLE (n)\
public:\
    n() : UnaryOperationNode() {}\
    n(const NE::LocString& name, const NUIE::Point& position) : UnaryOperationNode ( name, position) {}\
    double Op(double a) const  { if(v) return o (a); return (0); }\
};

UNARYOPNODE(NegativeNode,-,true)
UNARYOPNODE(FloorNode,floor,true)
UNARYOPNODE(CeilNode,ceil,true)
UNARYOPNODE(SqrtNode,sqrt,(a >= 0))
UNARYOPNODE(AbsNode,abs,true)
UNARYOPNODE(LogNode,log,(a > 0))
UNARYOPNODE(Log10Node,log10,(a > 0))



class NotNode : public BaseUnaryOperationNode<bool>
{
    DYNAMIC_SERIALIZABLE (NotNode)
public:
    NotNode () : BaseUnaryOperationNode<bool>(){}
    NotNode (const NE::LocString& name,
             const NUIE::Point& position)
                           : BaseUnaryOperationNode<bool>(name,position) {}
    bool Op(bool a ) const { return !a;}
};


}

#endif
