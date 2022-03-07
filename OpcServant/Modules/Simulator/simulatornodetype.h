#ifndef SIMULATORNODETYPE_H
#define SIMULATORNODETYPE_H

#include <NodeFlow/nodetype.h>

namespace MRL
{
    /*!
     * \brief The SimulatorNodeType class
     * map on to a Simulator item
     * template for others, probably
     */
    class SimulatorNodeType : public NODEFLOW::NodeType
    {

    public:
        SimulatorNodeType(const std::string &s = "Simulator") :  NODEFLOW::NodeType(s) {}

    };
}
#endif // SIMULATORNODETYPE_H
