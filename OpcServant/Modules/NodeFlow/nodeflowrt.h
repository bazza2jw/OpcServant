#ifndef NODEFLOWRT_H
#define NODEFLOWRT_H
#include <Common/Daq/rtobject.h>
#include <NodeFlow/nodeflow.h>
#include <NodeFlow/nodeset.h>

namespace MRL
{
    class NodeFlowRT : public MRL::RTObject
    {
        NODEFLOW::NodeSet _nodeSet; // the node set
    public:
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

       void process();
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
       virtual bool hasOutputs() const { return true;}
    };
}
#endif // NODEFLOWRT_H
