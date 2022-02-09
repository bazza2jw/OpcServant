#ifndef NE_NODEVALUECACHE_HPP
#define NE_NODEVALUECACHE_HPP

#include "NE_NodeId.hpp"
#include "NE_Slot.hpp"
#include "NE_Value.hpp"
#include <unordered_map>
#include <map>
namespace NE
{
//
// one value per output
// There is a separate calculation for each output
//
typedef std::map<std::string,  ValueConstPtr> NodeValueSet;
typedef std::unique_ptr<NodeValueSet> NodeValueSetPtr;

class NodeValueCache
{
public:
	NodeValueCache ();
	~NodeValueCache ();

    bool					Add (const NodeId& id, const ValueConstPtr& value, const std::string &s = "result");
    bool					Remove (const NodeId& id, const std::string &s= "result");
	void					Clear ();
    bool					Contains (const NodeId& id, const std::string &s= "result") const;
    const ValueConstPtr&	Get (const NodeId& id, const std::string &s= "result") const;

private:
    std::unordered_map<NodeId, NodeValueSetPtr>	cache;
    ValueConstPtr _null;
};

}

#endif
