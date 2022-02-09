#include "NE_NodeValueCache.hpp"
#include "NE_Debug.hpp"

namespace NE
{

NodeValueCache::NodeValueCache ()
{

}

NodeValueCache::~NodeValueCache ()
{

}

bool NodeValueCache::Add (const NodeId& id, const ValueConstPtr& value, const std::string &s)
{
    if (DBGERROR (Contains (id,s))) {
        return false; // already exists
	}

    if(cache.find (id) == cache.end ())
    {
        cache[id] = std::make_unique<NodeValueSet>( ); // create the dict for the node
    }
    //
    NodeValueSetPtr &m = cache[id];
    (*m)[s] = value; // set the value
	return true;
}

bool NodeValueCache::Remove (const NodeId& id, const std::string &s)
{
    if(!s.size() )
    {
        cache.erase(id);
    }
    else
    {
        if (DBGERROR (!Contains (id,s))) {
            return false;
        }
        NodeValueSetPtr &m = cache[id];
        m->erase(s);
    }
	return true;
}

void NodeValueCache::Clear ()
{
	return cache.clear ();
}

bool NodeValueCache::Contains (const NodeId& id, const std::string &s) const
{
    auto i = cache.find (id);
    if(cache.find (id) != cache.end ())
    {
       const NodeValueSetPtr &m = i->second;
       if(m)
       {
           return m->find(s) != m->end();
       }
    }
    return false;
}

const ValueConstPtr& NodeValueCache::Get (const NodeId& id,const std::string &s) const
{
    if(cache.find (id) != cache.end ())
    {
        const NodeValueSetPtr &m = cache.at(id);
        return m->at(s);
    }
    return _null;
}

}
