#include "nodeflowrt.h"
// Use alias to give meaningful names
MRL::StringVector MRL::NodeFlowRT::_inputs = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
/*!
    \brief start
*/
void MRL::NodeFlowRT::start()
{
    RTObject::start();
    _flowName = configuration().getString("/Flow");
    if(_nodeSet.data().load(_flowName))
    {
        _nodeSet.load(); // load the the graph
        _nodeSet.data().setValue("/OWNERID",dbId()); // bind this flow to this instance
        _nodeSet.data().setValue("/OWNER",pathAsString());
        _nodeSet.start(); // initialise all nodes in the graph
    }
    else
    {
        _nodeSet.clear();
    }
}
/*!
    \brief stop
*/
void MRL::NodeFlowRT::stop()
{
    RTObject::stop();
    _nodeSet.start(); // initialise all nodes in the graph
}

/*!
 * \brief onOneSecond
 * \param t
 */
void MRL::NodeFlowRT::onOneSecond(time_t /*t*/)
{
    // drive on the one second timer - suitable time scale for this application - change to process timer for faster tick rate
    NODEFLOW::VALUE v;
    _nodeSet.step(v);
}
