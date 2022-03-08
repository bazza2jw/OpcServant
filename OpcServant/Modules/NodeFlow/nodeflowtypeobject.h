#ifndef NODEFLOWTYPEOBJECT_H
#define NODEFLOWTYPEOBJECT_H
#include <open62541cpp/serverobjecttype.h>

namespace MRL
{
/*!
     * \brief The NodeFlowTypeObject class
     */
    class NodeFlowTypeObject : public Open62541::ServerObjectType
    {
    public:
        NodeFlowTypeObject(Open62541::Server &s, const std::string &n)
            : Open62541::ServerObjectType(s,n) {}
        bool addChildren(const Open62541::NodeId &parent) ;
        virtual bool addInstance(const std::string &n,
                                 const Open62541::NodeId &parent,
                                 Open62541::NodeId &nodeId,
                                 const Open62541::NodeId &requestNodeId = Open62541::NodeId::Null,
                                 Open62541::NodeContext *context = nullptr);

    };
}
#endif // NODEFLOWTYPEOBJECT_H
