#ifndef BI_BINARYOPERATIONNODES_HPP
#define BI_BINARYOPERATIONNODES_HPP

#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"

namespace BI
{

class BinaryOperationNode : public BasicUINode
{
    SERIALIZABLE;

public:
    BinaryOperationNode ();
    BinaryOperationNode (const NE::LocString& name, const NUIE::Point& position);
    virtual ~BinaryOperationNode ();

    virtual void				Initialize () override;
    virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

    virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
    virtual bool				IsForceCalculated () const override;

    virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
    virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
    virtual NE::ValuePtr		DoSingleOperation (const NE::ValueConstPtr& aValue, const NE::ValueConstPtr& bValue) const;
    virtual double				DoOperation (double a, double b) const = 0;
};


class ComparisonOperationNode :  public BasicUINode
{
    SERIALIZABLE;

public:
    ComparisonOperationNode () {}
    ComparisonOperationNode (const NE::LocString& name, const NUIE::Point& position) : BasicUINode(name,position) {}
    virtual void				Initialize () override
    {
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"),
                             NE::LocString (L"A"),
                             NE::ValuePtr (new NE::DoubleValue (0.0)),
                             NE::OutputSlotConnectionMode::Single)));
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), NE::LocString (L"B"),
                             NE::ValuePtr (new NE::DoubleValue (0.0)),
                             NE::OutputSlotConnectionMode::Single)));
        RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), NE::LocString (L"Result"))));
        RegisterFeature (NodeFeaturePtr (new ValueCombinationFeature (NE::ValueCombinationMode::Longest)));

    }
    virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override
    {
        NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
        NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);
        if (NE::IsSingleValue (aValue) && NE::IsSingleValue (bValue)) {
            return DoSingleOperation (aValue, bValue);
        } else {
            NE::ListValuePtr resultListValue (new NE::ListValue ());
            std::shared_ptr<ValueCombinationFeature> valueCombination = GetValueCombinationFeature (this);
            bool isValid = valueCombination->CombineValues ({ aValue, bValue }, [&] (const NE::ValueCombination& combination) {
                NE::ValuePtr result = DoSingleOperation (combination.GetValue (0), combination.GetValue (1));
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

    virtual void RegisterParameters (NUIE::NodeParameterList& parameterList) const override
    {
        BasicUINode::RegisterParameters (parameterList);
        NUIE::RegisterSlotDefaultValueNodeParameter<ComparisonOperationNode, NE::BooleanValue> (parameterList,
                NE::SlotId ("a"), NE::LocString (L"A"),
                NUIE::ParameterType::Boolean);
        NUIE::RegisterSlotDefaultValueNodeParameter<ComparisonOperationNode, NE::BooleanValue> (parameterList,
                NE::SlotId ("b"), NE::LocString (L"B"),
                NUIE::ParameterType::Boolean);

    }
    virtual bool				IsForceCalculated () const override {
        return true;
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

private:
    virtual NE::ValuePtr DoSingleOperation (const NE::ValueConstPtr& aValue, const NE::ValueConstPtr& bValue) const
    {
        double aDouble = NE::NumberValue::ToDouble (aValue);
        double bDouble = NE::NumberValue::ToDouble (bValue);
        bool result = DoOperation (aDouble, bDouble);
        return NE::ValuePtr (new NE::BooleanValue (result));
    }
    virtual bool DoOperation (double a, double b) const = 0; // bools are not an internal type
};


class LogicOperationNode : public BasicUINode
{
    SERIALIZABLE;

public:
    LogicOperationNode () {}
    LogicOperationNode (const NE::LocString& name, const NUIE::Point& position) : BasicUINode(name,position) {}

    virtual void				Initialize () override
    {
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"),
                             NE::LocString (L"A"),
                             NE::ValuePtr (new NE::BooleanValue (0.0)),
                             NE::OutputSlotConnectionMode::Single)));
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), NE::LocString (L"B"),
                             NE::ValuePtr (new NE::BooleanValue (0.0)),
                             NE::OutputSlotConnectionMode::Single)));
        RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), NE::LocString (L"Result"))));
        RegisterFeature (NodeFeaturePtr (new ValueCombinationFeature (NE::ValueCombinationMode::Longest)));

    }
    virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override
    {
        NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
        NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);
        if (NE::IsSingleValue (aValue) && NE::IsSingleValue (bValue)) {
            return DoSingleOperation (aValue, bValue);
        } else {
            NE::ListValuePtr resultListValue (new NE::ListValue ());
            std::shared_ptr<ValueCombinationFeature> valueCombination = GetValueCombinationFeature (this);
            bool isValid = valueCombination->CombineValues ({ aValue, bValue }, [&] (const NE::ValueCombination& combination) {
                NE::ValuePtr result = DoSingleOperation (combination.GetValue (0), combination.GetValue (1));
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

    virtual void RegisterParameters (NUIE::NodeParameterList& parameterList) const override
    {
        BasicUINode::RegisterParameters (parameterList);
        NUIE::RegisterSlotDefaultValueNodeParameter<LogicOperationNode, NE::BooleanValue> (parameterList,
                NE::SlotId ("a"), NE::LocString (L"A"),
                NUIE::ParameterType::Boolean);
        NUIE::RegisterSlotDefaultValueNodeParameter<LogicOperationNode, NE::BooleanValue> (parameterList,
                NE::SlotId ("b"), NE::LocString (L"B"),
                NUIE::ParameterType::Boolean);

    }
    virtual bool				IsForceCalculated () const override {
        return true;
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
private:
    NE::ValuePtr	DoSingleOperation (const NE::ValueConstPtr& aValue, const NE::ValueConstPtr& bValue) const
    {
        bool a = NE::NumberValue::ToBool(aValue);
        bool b = NE::NumberValue::ToBool(bValue);
        bool result = DoOperation (a, b);
        return NE::ValuePtr (new NE::BooleanValue (result));
    }

    virtual bool DoOperation (bool a, bool b) const = 0;
};



class BitOperationNode : public BasicUINode
{
    SERIALIZABLE;

public:
    BitOperationNode () {}
    BitOperationNode (const NE::LocString& name, const NUIE::Point& position) : BasicUINode(name,position) {}

    virtual void				Initialize () override
    {
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"),
                             NE::LocString (L"A"),
                             NE::ValuePtr (new NE::UnsignedValue (0.0)),
                             NE::OutputSlotConnectionMode::Single)));
        RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), NE::LocString (L"B"),
                             NE::ValuePtr (new NE::UnsignedValue (0.0)),
                             NE::OutputSlotConnectionMode::Single)));
        RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), NE::LocString (L"Result"))));
        RegisterFeature (NodeFeaturePtr (new ValueCombinationFeature (NE::ValueCombinationMode::Longest)));

    }
    virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override
    {
        NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
        NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);
        if (NE::IsSingleValue (aValue) && NE::IsSingleValue (bValue)) {
            return DoSingleOperation (aValue, bValue);
        } else {
            NE::ListValuePtr resultListValue (new NE::ListValue ());
            std::shared_ptr<ValueCombinationFeature> valueCombination = GetValueCombinationFeature (this);
            bool isValid = valueCombination->CombineValues ({ aValue, bValue }, [&] (const NE::ValueCombination& combination) {
                NE::ValuePtr result = DoSingleOperation (combination.GetValue (0), combination.GetValue (1));
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

    virtual void RegisterParameters (NUIE::NodeParameterList& parameterList) const override
    {
        BasicUINode::RegisterParameters (parameterList);
        NUIE::RegisterSlotDefaultValueNodeParameter<LogicOperationNode, NE::UnsignedValue> (parameterList,
                NE::SlotId ("a"), NE::LocString (L"A"),
                NUIE::ParameterType::Unsigned);
        NUIE::RegisterSlotDefaultValueNodeParameter<LogicOperationNode, NE::UnsignedValue> (parameterList,
                NE::SlotId ("b"), NE::LocString (L"B"),
                NUIE::ParameterType::Unsigned);

    }
    virtual bool	IsForceCalculated () const override {
        return true;
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
private:
    NE::ValuePtr	DoSingleOperation (const NE::ValueConstPtr& aValue, const NE::ValueConstPtr& bValue) const
    {
        unsigned a = NE::NumberValue::ToUnsigned(aValue);
        unsigned b = NE::NumberValue::ToUnsigned(bValue);
        unsigned result = DoOperation (a, b);
        return NE::ValuePtr (new NE::UnsignedValue (result));
    }

    virtual unsigned DoOperation (unsigned a, unsigned b) const = 0;
};
//
// Bit wise logic
//
class BitAndNode : public BitOperationNode
{
    DYNAMIC_SERIALIZABLE (BitAndNode);
public:
    BitAndNode () {}
    BitAndNode (const NE::LocString& name, const NUIE::Point& position) : BitOperationNode(name,position) {}
private:
    virtual unsigned DoOperation (unsigned a, unsigned b) const  {
        return a & b;
    }
};

class BitOrNode : public BitOperationNode
{
    DYNAMIC_SERIALIZABLE (BitOrNode);
public:
    BitOrNode () {}
    BitOrNode (const NE::LocString& name, const NUIE::Point& position) : BitOperationNode(name,position) {}
private:
    virtual unsigned DoOperation (unsigned a, unsigned b) const  {
        return a | b;
    }
};

class BitXorNode : public BitOperationNode
{
    DYNAMIC_SERIALIZABLE (BitXorNode);
public:
    BitXorNode () {}
    BitXorNode (const NE::LocString& name, const NUIE::Point& position) : BitOperationNode(name,position) {}
private:
    virtual unsigned DoOperation (unsigned a, unsigned b) const {
        return a ^ b;
    }
};


//
// Logic
//
class AndNode : public LogicOperationNode
{
    DYNAMIC_SERIALIZABLE (AndNode);
public:
    AndNode () {}
    AndNode (const NE::LocString& name, const NUIE::Point& position) : LogicOperationNode(name,position) {}
private:
    virtual bool DoOperation (bool a, bool b) const  {
        return a && b;
    }
};

class OrNode : public LogicOperationNode
{
    DYNAMIC_SERIALIZABLE (OrNode);
public:
    OrNode () {}
    OrNode (const NE::LocString& name, const NUIE::Point& position) : LogicOperationNode(name,position) {}
private:
    virtual bool DoOperation (bool a, bool b) const  {
        return a || b;
    }
};

class XorNode : public LogicOperationNode
{
    DYNAMIC_SERIALIZABLE (XorNode);
public:
    XorNode () {}
    XorNode (const NE::LocString& name, const NUIE::Point& position) : LogicOperationNode(name,position) {}
private:
    virtual bool DoOperation (bool a, bool b) const {
        return a ^ b;
    }
};


//
//
//
class GreaterThanNode : public ComparisonOperationNode
{
    DYNAMIC_SERIALIZABLE (GreaterThanNode);
public:
    GreaterThanNode () {}
    GreaterThanNode (const NE::LocString& name, const NUIE::Point& position) : ComparisonOperationNode(name,position) {}
private:
    virtual bool DoOperation (double a, double b) const override {
        return (a > b);
    }
};

class LessThanNode : public ComparisonOperationNode
{
    DYNAMIC_SERIALIZABLE (LessThanNode);
public:
    LessThanNode () {}
    LessThanNode (const NE::LocString& name, const NUIE::Point& position) : ComparisonOperationNode(name,position) {}
private:
    virtual bool DoOperation (double a, double b) const override {
        return (a < b);
    }
};

class EqualsNode : public ComparisonOperationNode
{
    DYNAMIC_SERIALIZABLE (EqualsNode);
public:
    EqualsNode () {}
    EqualsNode (const NE::LocString& name, const NUIE::Point& position) : ComparisonOperationNode(name,position) {}
private:
    virtual bool DoOperation (double a, double b) const override {
        return (a == b);
    }
};

class NotEqualsNode : public ComparisonOperationNode
{
    DYNAMIC_SERIALIZABLE (NotEqualsNode);
public:
    NotEqualsNode () {}
    NotEqualsNode (const NE::LocString& name, const NUIE::Point& position) : ComparisonOperationNode(name,position) {}
private:
    virtual bool DoOperation (double a, double b) const override {
        return (a != b);
    }
};
//
//
//
class AdditionNode : public BinaryOperationNode
{
    DYNAMIC_SERIALIZABLE (AdditionNode);

public:
    AdditionNode ();
    AdditionNode (const NE::LocString& name, const NUIE::Point& position);
    virtual ~AdditionNode ();

private:
    virtual double DoOperation (double a, double b) const override;
};

class SubtractionNode : public BinaryOperationNode
{
    DYNAMIC_SERIALIZABLE (SubtractionNode);

public:
    SubtractionNode ();
    SubtractionNode (const NE::LocString& name, const NUIE::Point& position);
    virtual ~SubtractionNode ();

private:
    virtual double DoOperation (double a, double b) const override;
};

class MultiplicationNode : public BinaryOperationNode
{
    DYNAMIC_SERIALIZABLE (MultiplicationNode);

public:
    MultiplicationNode ();
    MultiplicationNode (const NE::LocString& name, const NUIE::Point& position);
    virtual ~MultiplicationNode ();

private:
    virtual double DoOperation (double a, double b) const override;
};

class DivisionNode : public BinaryOperationNode
{
    DYNAMIC_SERIALIZABLE (DivisionNode);

public:
    DivisionNode ();
    DivisionNode (const NE::LocString& name, const NUIE::Point& position);
    virtual ~DivisionNode ();

private:
    virtual double DoOperation (double a, double b) const override;
};

}

#endif
