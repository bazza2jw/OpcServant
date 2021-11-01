#ifndef READONLYCONTEXT_H
#define READONLYCONTEXT_H
#include <open62541cpp/nodecontext.h>
namespace MRL
{
/*!
 * \brief The ReadOnlyContext class
 * This context handles read only access to the runtime value tree
 */
class ReadOnlyContext : public Open62541::NodeContext
{

public:
    ReadOnlyContext() : NodeContext("ReadOnly") {}
    virtual void readValue(Open62541::Server &/*server*/, Open62541::NodeId &/*node*/,
                           const UA_NumericRange * /*range*/, const UA_DataValue * /*value*/);

};
}
#endif // READONLYCONTEXT_H
