#ifndef NE_NODEVALUECACHE_HPP
#define NE_NODEVALUECACHE_HPP

#include "NE_NodeId.hpp"
#include "NE_Slot.hpp"
#include "NE_Value.hpp"
#include <unordered_map>
#include <map>
namespace NE
{

typedef std::map<NE::SlotId,  ValueConstPtr> NodeValueSet;
class NodeValueCache
{
public:
	NodeValueCache ();
	~NodeValueCache ();

	bool					Add (const NodeId& id, const ValueConstPtr& value);
	bool					Remove (const NodeId& id);
	void					Clear ();
	
	bool					Contains (const NodeId& id) const;
	const ValueConstPtr&	Get (const NodeId& id) const;

private:
	std::unordered_map<NodeId, ValueConstPtr>	cache;
};

}

#endif
