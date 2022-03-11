#include "scriptednodetype.h"



/*!
 * \brief NODEFLOW::ScriptedNodeType::start
 * \param ns
 * \param n
 */
void NODEFLOW::ScriptedNodeType::start(NodeSet &ns,  NodePtr &n)
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::trigger
 * \param ns
 * \param n
 */
void NODEFLOW::ScriptedNodeType::trigger(NodeSet &ns, NodePtr &n )
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::setupConnections
 */
void NODEFLOW::ScriptedNodeType::setupConnections()
{

        inputs().resize(1);
        inputs()[0] = Connection("in",Multiple);

        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple);
}
/*!
 * \brief NODEFLOW::ScriptedNodeType::properties
 * \param parent
 * \param ns
 * \param nodeId
 * \return
 */
bool NODEFLOW::ScriptedNodeType::properties(wxWindow * parent,NodeSet &ns, unsigned nodeId)
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::process
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return
 */
bool NODEFLOW::ScriptedNodeType::process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
{

}

/*!
 * \brief NODEFLOW::ScriptedNodeType::setup
 */
void NODEFLOW::ScriptedNodeType::setup()
{

}



