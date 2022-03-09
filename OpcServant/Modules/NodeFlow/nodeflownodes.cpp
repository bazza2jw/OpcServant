#include "nodeflownodes.h"
#include <Common/Daq/rtobject.h>
#include <Common/Daq/daq.h>
#include <Common/Daq/daqcommon.h>
#include "nodeflowrt.h"

/*!
 * \brief buildOutputList
 * \param l
 */
template <typename T>
void buildOutputList(T &l)
{
    l.clear();
    for (auto i = MRL::Common::daq().objects().begin(); i != MRL::Common::daq().objects().end(); i++) {
        MRL::RtObjectRef &r = i->second;
        if (r) {
            if (r->hasOutputs()) {
                std::string pt;
                r->path().toString(pt);
                MRL::StringVector &lo = r->outputs();
                for(unsigned j = 0; j < lo.size(); j++)
                {
                    std::string s = pt + TAG_SEPERATOR + lo[j];
                    l.push_back(s);
                }
            }
        }
    }
    l.push_back("(null)");
}
/*!
 * \brief buildAliasList
 * \param l
 */
template <typename T>
void buildAliasList(T &l)
{
    l.clear();
    MRL::StringMap &a = MRL::Common::aliasMap();
    for(auto i = a.begin(); i != a.end(); i++)
    {
        l.push_back(i->first);
    }
    l.push_back("(null)");
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
            setValueData(n->data()["TAG"].asString(),v,result);
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

static wxArrayString labels; // persists for an edit
static MRL::StringList strLabels;

void MRL::OpcServantInput::load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::load(dlg,ns,p);
    buildAliasList(labels);
    //
    wxString c(ns.data().getValue<std::string>(p,"Input"));
    int i = labels.Index(c);
    if(i < 0) i = 0;
    //
    wxEnumProperty * e = dlg.loader().addChoiceProperty("Input", "Input", i, labels);
    if(e)
    {
        e->SetChoiceSelection(i);
    }

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
    wxEnumProperty *e = dynamic_cast<wxEnumProperty *>(dlg.loader().fields()[NODEFLOW::PropField1]);
    if(e)
    {
        std::string s = labels[e->GetChoiceSelection()].ToStdString();
        ns.data().setValue(p,"Input",s);
    }
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
    buildAliasList(strLabels);
    buildAliasList(labels);

    dlg->addChoiceProperty("Input",0,strLabels);
    dlg->setChoice(NODEFLOW::PropField1,ns.data().getValue<std::string>(p,"Input"));

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
    int v =  dlg->getChoice(NODEFLOW::PropField1);
    ns.data().setValue(p,"Input", labels[v].ToStdString());
}


/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantOutput::load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::load(dlg,ns,p);
    buildOutputList(labels);
    //
    wxString c(ns.data().getValue<std::string>(p,"Output"));
    int i = labels.Index(c);
    if(i < 0) i = 0;
    wxEnumProperty * e = dlg.loader().addChoiceProperty("Output", "Output", i, labels);
    if(e)
    {
        e->SetChoiceSelection(i);
    }

}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantOutput::save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::save(dlg,ns,p);
    wxEnumProperty *e = dynamic_cast<wxEnumProperty *>(dlg.loader().fields()[NODEFLOW::PropField1]);
    if(e)
    {
        std::string s = labels[e->GetChoiceSelection()].ToStdString();
        ns.data().setValue(p,"Output",s);
    }
}
/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantOutput::load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::load(dlg,ns,p);
    buildOutputList(strLabels);
    buildOutputList(labels);
    dlg->addChoiceProperty("Output",0,strLabels);
    dlg->setChoice(NODEFLOW::PropField1,ns.data().getValue<std::string>(p,"Output"));
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantOutput::save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::save(dlg,ns,p);
    int v  = dlg->getChoice(NODEFLOW::PropField1);
    ns.data().setValue(p,"Output",labels[v].ToStdString());
}


/*!
 * \brief MRL::OpcServantPublish::process
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return
 */
bool MRL::OpcServantPublish::process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned /*id*/, const NODEFLOW::VALUE &data)
{
    // Get the tag
    // post the value
    NODEFLOW::NodePtr &n = ns.findNode(nodeId);
    if(n && n->enabled())
    {
        unsigned id = n->data()["OWNERID"].asUInt();
        MRL::RtObjectRef &r = MRL::Common::daq().objects()[id];
        if(r)
        {
            std::string t = n->data()["TAG"].asString();
            double d = data[DATA_PAYLOAD].asDouble();
            r->publishValue(d,t,STATES::States::StateOk );
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
void MRL::OpcServantPublish::load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    //
    // by fixing the possible tags there is no need to bind a flow config to a specific NodeFlowSet object instance
    // makes life a bit easier
    //
    NODEFLOW::NodeType::load(dlg,ns,p);
    labels.clear();
    for(auto j = MRL::NodeFlowRT::_inputs.begin(); j != MRL::NodeFlowRT::_inputs.end(); j++) labels.push_back(*j);
    wxString c(ns.data().getValue<std::string>(p,"Tag"));
    int i = labels.Index(c);
    if(i < 0) i = 0;
    wxEnumProperty * e = dlg.loader().addChoiceProperty("Tag", "Tag", i, labels);
    if(e)
    {
        e->SetChoiceSelection(i);
    }
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantPublish::save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::save(dlg,ns,p);
    wxEnumProperty *e = dynamic_cast<wxEnumProperty *>(dlg.loader().fields()[NODEFLOW::PropField1]);
    if(e)
    {
        std::string s = labels[e->GetChoiceSelection()].ToStdString();
        ns.data().setValue(p,"Tag",s);
    }
}
/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantPublish::load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::load(dlg,ns,p);
    strLabels.clear();
    for(auto j = MRL::NodeFlowRT::_inputs.begin(); j != MRL::NodeFlowRT::_inputs.end(); j++)
    {
        strLabels.push_back(*j);
        labels.push_back(*j);
    }
    dlg->addChoiceProperty("Tag",0,strLabels);
    dlg->setChoice(NODEFLOW::PropField1,ns.data().getValue<std::string>(p,"Tag"));
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void MRL::OpcServantPublish::save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::save(dlg,ns,p);
    int v = dlg->getChoice(NODEFLOW::PropField1);
    std::string t = labels[v].ToStdString();
    ns.data().setValue(p,"Tag",t);
}


