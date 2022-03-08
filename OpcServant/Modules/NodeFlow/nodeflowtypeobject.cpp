#include "nodeflowtypeobject.h"

/*!
 * \brief MRL::LinkTypeObject::addChildren
 * \param parent
 * \return
 */
bool MRL::NodeFlowTypeObject::addChildren(const Open62541::NodeId &parent)
{

    return true;
}

/*!
 * \brief MRL::LinkTypeObject::addInstance
 * \param n
 * \param parent
 * \param nodeId
 * \param requestNodeId
 * \param context
 * \return
 */
bool MRL::NodeFlowTypeObject::addInstance(const std::string &n,
                         const Open62541::NodeId &parent,
                         Open62541::NodeId &nodeId,
                         const Open62541::NodeId &requestNodeId,
                         Open62541::NodeContext *context)
{
    if(Open62541::ServerObjectType::addInstance(n,parent,nodeId,requestNodeId,context))
    {

        return true;
    }
    return false;
}
