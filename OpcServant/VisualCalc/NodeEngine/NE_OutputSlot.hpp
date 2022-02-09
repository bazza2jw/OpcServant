#ifndef NE_OUTPUTSLOT_HPP
#define NE_OUTPUTSLOT_HPP

#include "NE_Slot.hpp"
#include "NE_Value.hpp"

namespace NE
{

class OutputSlot : public Slot
{
	DYNAMIC_SERIALIZABLE (OutputSlot);

public:
	OutputSlot ();
	OutputSlot (const SlotId& id);
	virtual ~OutputSlot ();
	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

};

}

#endif
