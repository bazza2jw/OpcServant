#ifndef NODEFLOWRT_H
#define NODEFLOWRT_H
#include <Common/Daq/rtobject.h>
#include <NodeFlow/NodeFlow/nodeflow.h>
#include <NodeFlow/NodeFlow/nodeset.h>

namespace MRL
{
    class NodeFlowRT : public MRL::RTObject
    {

        std::string _flowName; // the flow file name
        NODEFLOW::NodeSet _nodeSet; // the node set
    public:
       static MRL::StringVector _inputs;
       StringVector & inputs() { return _inputs;}
       NodeFlowRT(int id) : RTObject(id)
        {
        }

       /*!
           \brief start
       */
       virtual void start();
       /*!
           \brief stop
       */
       virtual void stop();

       /*!
        * \brief onOneSecond
        * \param t
        */
       void onOneSecond(time_t t);

       /*!
        * \brief hasInputs
        * \return true if has inputs
        */
       virtual bool hasInputs() const { return true;}
    };
}
#endif // NODEFLOWRT_H
