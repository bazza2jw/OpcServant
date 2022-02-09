#include "NE_InputSlot.hpp"
#include "NE_Node.hpp"
#include "NE_NodeManager.hpp"

namespace NE
{

DYNAMIC_SERIALIZATION_INFO (InputSlot, 1, "{59CFA81D-588F-40EF-A16B-0E57C55AA7E6}");

InputSlot::InputSlot () :
	Slot (),
	outputSlotConnectionMode (OutputSlotConnectionMode::Disabled)
{

}

InputSlot::InputSlot (const SlotId& id,  OutputSlotConnectionMode outputSlotConnectionMode) :
	Slot (id),
	outputSlotConnectionMode (outputSlotConnectionMode)
{

}

InputSlot::~InputSlot ()
{

}

OutputSlotConnectionMode InputSlot::GetOutputSlotConnectionMode () const
{
	return outputSlotConnectionMode;
}


Stream::Status InputSlot::Read (InputStream& inputStream)
{
	ObjectHeader header (inputStream);
	Slot::Read (inputStream);
	bool hasDefaultValue = false;
	inputStream.Read (hasDefaultValue);
	ReadEnum (inputStream, outputSlotConnectionMode);
	return inputStream.GetStatus ();
}

Stream::Status InputSlot::Write (OutputStream& outputStream) const
{
	ObjectHeader header (outputStream, serializationInfo);
	Slot::Write (outputStream);
	WriteEnum (outputStream, outputSlotConnectionMode);
	return outputStream.GetStatus ();
}

}
