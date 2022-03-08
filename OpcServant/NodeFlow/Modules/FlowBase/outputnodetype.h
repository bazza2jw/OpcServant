#ifndef OUTPUTNODETYPE_H
#define OUTPUTNODETYPE_H
/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include <functional>
#include <NodeFlow/NodeFlow/nodetype.h>
#include <NodeFlow/NodeFlow/nodeset.h>
#include <NodeFlow/NodeFlow/fparser.hh>
#include <NodeFlow/NodeFlow/PropertiesEditorDialog.h>

namespace NODEFLOW
{
    // trace a message
    class TraceNodeType : public NodeType
    {
    public:
        /*!
         * \brief TraceNodeType
         * \param s
         */
        TraceNodeType(const std::string &s) : NodeType(s) {}

        /*!
         * \brief nodeClass
         * \return
         */
        virtual const char * nodeClass() const { return "Outputs";}
        /*!
         * \brief setupConnections
         */
        void setupConnections()
        {
            inputs().resize(1);
            inputs()[0] = Connection("in",Multiple,Any);
            // set up the outputs
            outputs().resize(2);
            outputs()[0] = Connection("out",Multiple,Any);
            outputs()[1] = Connection("Trace",Multiple,String);
        }
        /*!
         * \brief process
         * \param ns
         * \param nodeId
         * \param id
         * \param data
         * \return
         */
        bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            // log the data to standard error
            std::ostringstream os;
            os << "Node Id:" << nodeId  << " InputId:" << id << " Value:" << data;
            VALUE tv;
            setValueData(std::string("Trace"),os.str(),tv);
            post(ns,nodeId,1,tv); // send the trace message out of the trace output as a string - the node set out queue

            return post(ns,nodeId,Output,data); // just forward
        }

    };



}
#endif // OUTPUTNODETYPE_H
