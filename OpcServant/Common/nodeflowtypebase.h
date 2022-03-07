#ifndef NODEFLOWTYPEBASE_H
#define NODEFLOWTYPEBASE_H
#include <NodeFlow/nodetype.h>
#include <Common/objectmanager.h>

namespace MRL {

    class NodeFlowTypeBase : public NODEFLOW::NodeType
    {
        std::string _name;
    public:
        NodeFlowTypeBase(const std::string &s) : NODEFLOW::NodeType(s),_name(s) {}
        virtual const char * nodeClass() const { return "OpcServant";}


        virtual bool process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned id, const NODEFLOW::VALUE &data)
        {
            NODEFLOW::NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {
                MRL::PropertyPath p;
                n->toPath(p);
                int id = ns.data().getValue<int>(p,"Id");
                //
                //
                NODEFLOW::VALUE result;
                setValueData(data,v,result);
                return post(ns,nodeId,NODEFLOW::Output,result);
            }
            return false;
        }

        void setupConnections()
        {
            inputs().resize(1);
            inputs()[0] =  NODEFLOW::Connection("trig",NODEFLOW::Multiple,NODEFLOW::Bool);
            // set up the outputs
            outputs().resize(2);
            outputs()[0] = NODEFLOW::Connection("out",NODEFLOW::Multiple,NODEFLOW::Float);
            outputs()[1] = NODEFLOW::Connection("state",NODEFLOW::Multiple,NODEFLOW::Integer);
        }


        virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
            dlg.loader().addProp("Value","Value",ns.data().getValue<T>(p,"Value"));
        }
        virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::save(dlg,ns,p);
            T v = dlg.loader().get<T>(NODEFLOW::PropField1);
            ns.data().setValue(p,"Value",v);
        }

        void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
            dlg->addProp("Value",ns.data().getValue<T>(p,"Value"));
        }

        void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::save(dlg,ns,p);
            T v;
            dlg->get(NODEFLOW::PropField1,v);
            ns.data().setValue(p,"Value",v);
        }

        /*!
         * \brief properties
         * \param parent
         * \param ns
         * \param nodeId
         * \return
         */
        bool properties(wxWindow *parent, NodeSet &ns, unsigned nodeId);
        /*!
         * \brief properties
         * \param parent
         * \param ns
         * \param nodeId
         * \return
         */
        bool properties(Wt::WWidget *parent, NodeSet &ns, unsigned nodeId);


    };
}
#endif // NODEFLOWTYPEBASE_H
