#include "nodeflownodes.h"
#include <Common/Daq/rtobject.h>
#include <Common/Daq/daq.h>
#include <Common/Daq/daqcommon.h>


/*!
 * \brief buildOutputList
 * \param l
 */
static void buildOutputList(MRL::StringList &l)
{
    for (auto i = MRL::Common::daq().objects().begin(); i != MRL::Common::daq().objects().end(); i++) {
        MRL::RtObjectRef &r = i->second;
        if (r) {
            if (r->hasOutputs()) {
                std::string pt;
                r->path().toString(pt);
                MRL::StringVector &l = r->outputs();
                for(unsigned j = 0; j < l.size(); j++)
                {
                    std::string s = pt + TAG_SEPERATOR + l[j];
                    l.push_back(s);
                }
            }
        }
    }
}
/*!
 * \brief buildAliasList
 * \param l
 */
static void buildAliasList(MRL::StringList &l)
{
    MRL::StringMap &a = MRL::Common::aliasMap();
    for(auto i = a.begin(); i != a.end(); i++)
    {
        l.push_back(i->first);
    }
}

/*!
 * \brief MRL::OpcServantOutput::process
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return
 */
bool MRL::OpcServantOutput::process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned id, const NODEFLOW::VALUE &data)
{
    NODEFLOW::NodePtr &n = ns.findNode(nodeId);
    if(n && n->enabled())
    {
        // Send to output
        // post message
        unsigned id = n->data()["PATH"].asUInt();
        std::string out = data[DATA_TOPIC].asString();
        bool state = data[DATA_PAYLOAD].asBool();
        //
        // Now find the object and message it
        //
        MRL::RtObjectRef &r = MRL::Common::daq().objects()[id];
        if (r) {
            MRL::Message m(MESSAGEID::SetOutput);
            m.data().add(PARAMETERID::Index, out);
            m.data().add(PARAMETERID::Value, state);
            r->queueItem(m);
        }
    }
    return true;
}

/*!
 * \brief MRL::OpcServantInput::process
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return
 */
bool MRL::OpcServantInput::process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned id, const NODEFLOW::VALUE &/*data*/)
{
    // get the input id
    NODEFLOW::NodePtr &n = ns.findNode(nodeId);
    if(n && n->enabled())
    {
        unsigned id = n->data()["PATH"].asUInt();
        MRL::RtObjectRef &r = MRL::Common::daq().objects()[id];
        if (r) {
            double v = r->getInput<double>(n->data()["TAG"].asString());
            NODEFLOW::VALUE result;
            setValueData(n->data()["TAG"],v,result);
            post(ns,nodeId,Output,result);
        }
    }
    return true;
}

/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantInput::load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::load(dlg,ns,p);
    dlg.loader().addStringProperty("Input","Input",ns.data().getValue<std::string>(p,"Input"));
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantInput::save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::save(dlg,ns,p);
    ns.data().setValue(p,"Input",dlg.loader().get<wxString>(NODEFLOW::PropField1).ToStdString());

}
/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantInput::load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::load(dlg,ns,p);
    dlg->addStringProperty("Input",ns.data().getValue<std::string>(p,"Input"));
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantInput::save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::save(dlg,ns,p);
    std::string  v =  dlg->getString(NODEFLOW::PropField1);
    ns.data().setValue(p,"Input",v);
}



/*!
 * \brief MRL::OpcServantPublish::process
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return
 */
bool MRL:: OpcServantPublish::process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned id, const NODEFLOW::VALUE &data)
{
    //
    // Get the tag
    // post the value
    //
    NODEFLOW::NodePtr &n = ns.findNode(nodeId);
    if(n && n->enabled())
    {
        PropertyPath p;
        p.toList(ns.data().getString("/OWNER"));
        unsigned id = MRL::Common::configuration().find(p); // the object id
        MRL::RtObjectRef &r = MRL::Common::daq().objects()[id];
        if(r)
        {
            r->publishValue(data[DATA_PAYLOAD].asDouble(),data[DATA_TOPIC].asString(),STATES::States::StateOk );
        }
    }
    return true;
}

