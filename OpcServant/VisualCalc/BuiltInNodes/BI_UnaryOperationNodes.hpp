#ifndef BI_UNARYOPERATIONNODES_HPP
#define BI_UNARYOPERATIONNODES_HPP

#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"

namespace BI
{

class UnaryOperationNode : public BasicUINode
{
	SERIALIZABLE;

public:
	UnaryOperationNode ();
	UnaryOperationNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~UnaryOperationNode ();

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
		
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual bool				IsForceCalculated () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	NE::ValuePtr				DoSingleOperation (const NE::ValueConstPtr& aValue) const;
	virtual bool				IsValidInput (double a) const;
    virtual double				DoOperation (double a) const { return 0.0;}
};


class NotNode : public UnaryOperationNode
{
    DYNAMIC_SERIALIZABLE (NotNode)

public:
    NotNode (){}
    NotNode (const NE::LocString& name, const NUIE::Point& position) : UnaryOperationNode(name,position) {}
    virtual ~NotNode (){}

    virtual void				Initialize () override
    {
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), NE::LocString (L"A"),
                                                                          NE::ValuePtr (new NE::BooleanValue (false)),
                                                                          NE::OutputSlotConnectionMode::Single)));
        RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), NE::LocString (L"Result"))));
    }
    virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override
    {
        NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
        if (!NE::IsComplexType<NE::NumberValue> (aValue)) {
            return nullptr;
        }

        if (NE::IsSingleValue (aValue)) {
            return DoSingleOperation (aValue);
        } else {
            NE::ListValuePtr resultListValue (new NE::ListValue ());
            bool isValid = NE::FlatEnumerate (aValue, [&] (const NE::ValueConstPtr& val) {
                NE::ValuePtr result = DoSingleOperation (val);
                if (result == nullptr) {
                    return false;
                }
                resultListValue->Push (result);
                return true;
            });
            if (!isValid) {
                return nullptr;
            }
            return resultListValue;
        }

    }

    virtual void	RegisterParameters (NUIE::NodeParameterList& parameterList) const
    {
        BasicUINode::RegisterParameters (parameterList);
        NUIE::RegisterSlotDefaultValueNodeParameter<NotNode, NE::BooleanValue> (parameterList, NE::SlotId ("a"),
                                                                                NE::LocString (L"A"),
                                                                                NUIE::ParameterType::Boolean);

    }
    virtual bool				IsForceCalculated () const override { return true;}

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
private:
    NE::ValuePtr	DoSingleOperation (const NE::ValueConstPtr& aValue) const
    {
        bool a = NE::NumberValue::ToBool (aValue);
        return NE::ValuePtr (new NE::BooleanValue (!a));
    }
};



class AbsNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (AbsNode);

public:
	AbsNode ();
	AbsNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~AbsNode ();

private:
	virtual double DoOperation (double a) const override;
};

class FloorNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (FloorNode);

public:
	FloorNode ();
	FloorNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~FloorNode ();

private:
	virtual double DoOperation (double a) const override;
};

class CeilNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (CeilNode);

public:
	CeilNode ();
	CeilNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~CeilNode ();

private:
	virtual double DoOperation (double a) const override;
};

class NegativeNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (NegativeNode);

public:
	NegativeNode ();
	NegativeNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~NegativeNode ();

private:
	virtual double DoOperation (double a) const override;
};

class SqrtNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (SqrtNode);

public:
	SqrtNode ();
	SqrtNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~SqrtNode ();

private:
	virtual bool	IsValidInput (double a) const override;
	virtual double	DoOperation (double a) const override;
};

}

#endif
