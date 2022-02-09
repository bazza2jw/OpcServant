#include "BI_ViewerUINodes.hpp"
#include "BI_UINodePanels.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>
#include <iostream>

namespace BI
{

DYNAMIC_SERIALIZATION_INFO (ViewerNode, 1, "{417392AA-F72D-4E84-8F58-766D0AAC07FC}");


ViewerNode::ViewerNode () :
	ViewerNode (NE::LocString (), NUIE::Point ())
{

}

ViewerNode::ViewerNode (const NE::LocString& name, const NUIE::Point& position) :
    BasicUINode (name, position)
{

}

ViewerNode::~ViewerNode ()
{

}

void ViewerNode::Process(const NE::InputSlotConstPtr &in, JSONVALUEPTR &v)
{
    try
    {
        if(v && in)
        {
            std::cerr << in->GetId().Id() << ":" << v->asString() << std::endl;
        }
        else
        {
            std::cerr << "Viewer Process" << std::endl;
        }

    }
    catch(...)
    {

    }
}


void ViewerNode::Initialize ()
{
    RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), NE::LocString (L"Input"), NE::OutputSlotConnectionMode::Single)));
}


NE::Stream::Status ViewerNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ViewerNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}


}
