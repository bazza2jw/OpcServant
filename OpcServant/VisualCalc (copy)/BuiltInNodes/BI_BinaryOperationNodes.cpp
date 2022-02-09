#include "BI_BinaryOperationNodes.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeCommonParameters.hpp"

#include <cmath>

namespace BI
{

SERIALIZATION_INFO (BinaryOperationNode, 1);
DYNAMIC_SERIALIZATION_INFO (AdditionNode, 1, "{1A72C230-3D90-42AD-835A-43306E641EA2}");
DYNAMIC_SERIALIZATION_INFO (SubtractionNode, 1, "{80CACB59-C3E6-441B-B60C-37A6F2611FC2}");
DYNAMIC_SERIALIZATION_INFO (MultiplicationNode, 1, "{75F39B99-8296-4D79-8BB7-418D55F93C25}");
DYNAMIC_SERIALIZATION_INFO (DivisionNode, 1, "{652DDDFC-A441-40B1-87AC-0BED247F35E7}");
//
// Bit operations
SERIALIZATION_INFO (BitOperationNode,1);
DYNAMIC_SERIALIZATION_INFO (BitAndNode, 1, "{3ea6f05e-474b-47a3-be3c-d2ad97e2e54e}");
DYNAMIC_SERIALIZATION_INFO (BitOrNode, 1, "{ad834ae5-5d8b-49ac-a779-897c8cef1d74}");
DYNAMIC_SERIALIZATION_INFO (BitXorNode, 1, "{ca068080-b5d5-461f-bec7-36b47a32d574}");


// Logic nodes
//
SERIALIZATION_INFO (LogicOperationNode,1);
DYNAMIC_SERIALIZATION_INFO (AndNode, 1, "{9666e3db-8e70-4f10-a0f4-a011d44fa250}");
DYNAMIC_SERIALIZATION_INFO (OrNode, 1, "{a70ca6e1-94bd-431e-b142-01dc517983e9}");
DYNAMIC_SERIALIZATION_INFO (XorNode, 1, "{f695d0c9-6544-4c2d-ac5a-d565c7db95a9}");
//
// Comparison
//
SERIALIZATION_INFO (ComparisonOperationNode,1);
DYNAMIC_SERIALIZATION_INFO (GreaterThanNode, 1, "{3f4cace9-8841-4f90-b839-f74a869e6421}");
DYNAMIC_SERIALIZATION_INFO (LessThanNode, 1, "{b2036627-41df-4434-8ab0-e8f1c31b0b62}");
DYNAMIC_SERIALIZATION_INFO (EqualsNode, 1, "{6eb66e78-0e51-4e71-a04a-2d6df5f5b2d6}");
DYNAMIC_SERIALIZATION_INFO (NotEqualsNode, 1, "{f20cb62d-e24b-4fff-bade-b638665d20c5}");
//

BinaryOperationNode::BinaryOperationNode () :
	BinaryOperationNode (NE::LocString (), NUIE::Point ())
{

}

BinaryOperationNode::BinaryOperationNode (const NE::LocString& name, const NUIE::Point& position) :
	BasicUINode (name, position)
{
}

BinaryOperationNode::~BinaryOperationNode ()
{

}

void BinaryOperationNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("a"), NE::LocString (L"A"), NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), NE::LocString (L"B"), NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("result"), NE::LocString (L"Result"))));
	RegisterFeature (NodeFeaturePtr (new ValueCombinationFeature (NE::ValueCombinationMode::Longest)));
}

NE::ValueConstPtr BinaryOperationNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
	NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);
	if (!NE::IsComplexType<NE::NumberValue> (aValue) || !NE::IsComplexType<NE::NumberValue> (bValue)) {
		return nullptr;
	}

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

void BinaryOperationNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<BinaryOperationNode, NE::DoubleValue> (parameterList, NE::SlotId ("a"), NE::LocString (L"A"), NUIE::ParameterType::Double);
	NUIE::RegisterSlotDefaultValueNodeParameter<BinaryOperationNode, NE::DoubleValue> (parameterList, NE::SlotId ("b"), NE::LocString (L"B"), NUIE::ParameterType::Double);
}

bool BinaryOperationNode::IsForceCalculated () const
{
	return true;
}

NE::Stream::Status BinaryOperationNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status BinaryOperationNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr BinaryOperationNode::DoSingleOperation (const NE::ValueConstPtr& aValue, const NE::ValueConstPtr& bValue) const
{
	double aDouble = NE::NumberValue::ToDouble (aValue);
	double bDouble = NE::NumberValue::ToDouble (bValue);
	double result = DoOperation (aDouble, bDouble);
	if (std::isnan (result) || std::isinf (result)) {
		return nullptr;
	}
	return NE::ValuePtr (new NE::DoubleValue (result));
}

AdditionNode::AdditionNode () :
	BinaryOperationNode ()
{

}

AdditionNode::AdditionNode (const NE::LocString& name, const NUIE::Point& position) :
	BinaryOperationNode (name, position)
{

}

AdditionNode::~AdditionNode ()
{

}

double AdditionNode::DoOperation (double a, double b) const
{
	return a + b;
}

SubtractionNode::SubtractionNode () :
	BinaryOperationNode ()
{

}

SubtractionNode::SubtractionNode (const NE::LocString& name, const NUIE::Point& position) :
	BinaryOperationNode (name, position)
{

}

SubtractionNode::~SubtractionNode ()
{

}

double SubtractionNode::DoOperation (double a, double b) const
{
	return a - b;
}

MultiplicationNode::MultiplicationNode () :
	BinaryOperationNode ()
{

}

MultiplicationNode::MultiplicationNode (const NE::LocString& name, const NUIE::Point& position) :
	BinaryOperationNode (name, position)
{

}

MultiplicationNode::~MultiplicationNode ()
{

}

double MultiplicationNode::DoOperation (double a, double b) const
{
	return a * b;
}

DivisionNode::DivisionNode () :
	BinaryOperationNode ()
{

}

DivisionNode::DivisionNode (const NE::LocString& name, const NUIE::Point& position) :
	BinaryOperationNode (name, position)
{

}

DivisionNode::~DivisionNode ()
{

}

double DivisionNode::DoOperation (double a, double b) const
{
	return a / b;
}

}
