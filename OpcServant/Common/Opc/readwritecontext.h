#ifndef READWRITECONTEXT_H
#define READWRITECONTEXT_H
#include <open62541cpp/nodecontext.h>
namespace MRL {
    class ReadWriteContext  : public Open62541::NodeContext {
        public:
            ReadWriteContext(): NodeContext("ReadWrite") {}
            /*!
                \brief writeValue
                \param node
            */
           void writeValue(Open62541::Server &/*server*/, Open62541::NodeId &/*node*/,
                           const UA_NumericRange * /*range*/, const UA_DataValue &/*value*/);

    };
}
#endif // READWRITECONTEXT_H
