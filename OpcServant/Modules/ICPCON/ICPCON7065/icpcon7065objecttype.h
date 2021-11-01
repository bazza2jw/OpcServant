#ifndef ICPCON7065OBJECTTYPE_H
#define ICPCON7065OBJECTTYPE_H

#include "../icpcon_global.h"
#include <open62541cpp/serverobjecttype.h>
namespace MRL {

    class ICPCON7065ObjectType : public Open62541::ServerObjectType {

        public:
            ICPCON7065ObjectType(Open62541::Server &s, const std::string &n)
                : Open62541::ServerObjectType(s, n) {}
            bool addChildren(const Open62541::NodeId &parent) ;
            virtual bool addInstance(const std::string &n,
                                     const Open62541::NodeId &parent,
                                     Open62541::NodeId &nodeId,
                                     const Open62541::NodeId &requestNodeId = Open62541::NodeId::Null,
                                     Open62541::NodeContext *context = nullptr);
    };
}

#endif // ICPCON7065OBJECTTYPE_H
