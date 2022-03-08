#ifndef NODEFLOWNODES_H
#define NODEFLOWNODES_H

#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/Daq/rtobject.h>
#include <NodeFlow/nodetype.h>
#include <NodeFlow/nodeset.h>
#include <NodeFlow/node.h>
#include <NodeFlow/PropertiesEditorDialog.h>
#include <NodeFlow/webproperties.h>


namespace MRL {

/*!
 * \brief The OpcServantInput class
 */
class OpcServantInput : public NODEFLOW::NodeType
{
    enum
    {
        Trigger = 0,
        Output = 0
    };
public:
    OpcServantInput(const std::string &s) : NODEFLOW::NodeType(s)
    {

    }

    virtual void start(NODEFLOW::NodeSet &ns,  NODEFLOW::NodePtr &n)
    {
        MRL::PropertyPath p;
        n->toPath(p);
        std::string t = ns.data().getValue<std::string>(p,"Input");
        n->data()["INPUT"] = t;
        //
        // Parse it
        //
        std::string o = MRL::Common::aliasMap()[t];
        // split on the tag separator
        MRL::PathTag pt;
        if(pt.parse(o))
        {
            PropertyPath p;
            p.toList(pt._path);
            n->data()["PATH"] = MRL::Common::configuration().find(p); // the object id
            n->data()["TAG"] = pt._tag;
        }
    }

    virtual const char * nodeClass() const { return "OpcServant";}

    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        //
        inputs()[0] = NODEFLOW::Connection("trig",NODEFLOW::Single,NODEFLOW::Bool);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = NODEFLOW::Connection("out",NODEFLOW::Multiple,NODEFLOW::Any); // pass through
    }

    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p);
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p);
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p);
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p);

   virtual bool process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned id, const NODEFLOW::VALUE &data);

};

/*!
 * \brief The OpcServantOutput class
 */
class OpcServantOutput : public NODEFLOW::NodeType
{
    enum
    {
        Input = 0
    };

public:
    OpcServantOutput(const std::string &s) : NODEFLOW::NodeType(s)
    {

    }

    virtual void start(NODEFLOW::NodeSet &ns,  NODEFLOW::NodePtr &n)
    {
        MRL::PropertyPath p;
        n->toPath(p);
        std::string t = ns.data().getValue<std::string>(p,"Output");
        MRL::PathTag pt;
        if(pt.parse(t))
        {
            PropertyPath p;
            p.toList(pt._path);
            n->data()["PATH"] = MRL::Common::configuration().find(p); // the object id
            n->data()["TAG"] = pt._tag;
        }
    }

    virtual const char * nodeClass() const { return "OpcServant";}

    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        //
        inputs()[0] = NODEFLOW::Connection("in",NODEFLOW::Single,NODEFLOW::Any);
        //
        // set up the outputs
        outputs().resize(0);
    }

    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg.loader().addStringProperty("Output","Output",ns.data().getValue<std::string>(p,"Output"));
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg->addStringProperty("Input",ns.data().getValue<std::string>(p,"Input"));
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        std::string  v =  dlg->getString(NODEFLOW::PropField1);
        ns.data().setValue(p,"Input",v);
    }

    virtual bool process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned id, const NODEFLOW::VALUE &data);

};

/*!
 * \brief The OpcServantPublish class
 */
class OpcServantPublish : public NODEFLOW::NodeType
{
    enum
    {
        Input = 0
    };

public:

    OpcServantPublish(const std::string &s) : NODEFLOW::NodeType(s)
    {

    }

    virtual const char * nodeClass() const { return "OpcServant";}

    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        //
        inputs()[0] = NODEFLOW::Connection("in",NODEFLOW::Single,NODEFLOW::Any);
        //
        // set up the outputs
        outputs().resize(0);
    }

    virtual void start(NODEFLOW::NodeSet &ns,  NODEFLOW::NodePtr &n)
    {
        std::string  t = ns.data().getString("/OWNER");
        n->data()["OWNER"] = t; // get the path to the owner
    }


    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg.loader().addStringProperty("Tag","Tag",ns.data().getValue<std::string>(p,"Tag"));
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        ns.data().setValue(p,"Tag",dlg.loader().get<wxString>(NODEFLOW::PropField1).ToStdString());
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg->addStringProperty("Tag",ns.data().getValue<std::string>(p,"Tag"));
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        std::string  v =  dlg->getString(NODEFLOW::PropField1);
        ns.data().setValue(p,"Tag",v);
    }

    virtual bool process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned id, const NODEFLOW::VALUE &data);

};



}
#endif // NODEFLOWNODES_H
