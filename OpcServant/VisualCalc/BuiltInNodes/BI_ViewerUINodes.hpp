#ifndef BI_UINODES_HPP
#define BI_UINODES_HPP

#include "../NodeEngine/NE_SingleValues.hpp"
#include "../NodeUIEngine/NUIE_UINode.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_UINodeLayouts.hpp"

namespace BI
{

class ViewerNode : public BasicUINode
{
    DYNAMIC_SERIALIZABLE (ViewerNode)

public:


	ViewerNode ();
	ViewerNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~ViewerNode ();

	virtual void						Initialize () override;
    void Process(const NE::InputSlotConstPtr &in, JSONVALUEPTR &v);

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;
};

}

#endif
