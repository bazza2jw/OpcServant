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


#include "nodesettype.h"
#include "NodeFlowEditorDialog.h"
#include "NodeEditorPanel.h"
#include "webproperties.h"

/*!
 * \brief The NodeSetPropertiesEditorDialog class
 */
class NodeSetPropertiesEditorDialog : public PropertiesEditorDialog
{
public:
    NodeSetPropertiesEditorDialog(wxWindow* parent, MRL::VariantPropertyTree &data,  MRL::PropertyPath &p) :
        PropertiesEditorDialog(parent,data,p) {}
    void OnSettings(wxCommandEvent& /*event*/)
    {
        std::string f = data().getValue<std::string>(path(),"NodeSet");
        if(f[0] != '/')
        {
            f = NODE_SET_DIR + f; // add the default directory
        }
        NodeFlowEditorDialog dlg(this,f);
        dlg.editor()->canvas()->readSet(f);
        if(dlg.ShowModal() == wxID_OK)
        {
            dlg.editor()->canvas()->save();
        }
    }
};


/*!
 * \brief NODEFLOW::NodeSetType::properties
 * \param parent
 * \param ns
 * \param nodeId
 * \return true on success
 */
bool NODEFLOW::NodeSetType::properties(wxWindow * parent, NodeSet &ns, unsigned nodeId)
{
    MRL::PropertyPath p;
    NodePtr &n = ns.findNode(nodeId);
    n->toPath(p);
    NodeSetPropertiesEditorDialog dlg(parent,ns.data(),p);
    //
    load(dlg,ns,p);
    if(dlg.ShowModal() == wxID_OK)
    {
        save(dlg,ns,p);
        return true;
    }
    return false;
}

/*!
 * \brief NODEFLOW::NodeSetType::load
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::NodeSetType::load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::load(dlg,ns,p);
    dlg.loader().addStringProperty("Node Set","NodeSet",ns.data().getValue<std::string>(p,"NodeSet"));
}

/*!
 * \brief NODEFLOW::NodeSetType::save
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::NodeSetType::save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    NODEFLOW::NodeType::save(dlg,ns,p);
    wxVariant v = dlg.loader().fields()[PropField1]->GetValue();
    ns.data().setValue(p,"NodeSet",v.GetString().ToStdString());
}

/*!
 * \brief NODEFLOW::NodeSetType::load
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::NodeSetType::load(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::load(dlg,ns, p);
    dlg->addStringProperty("Node Set",ns.data().getValue<std::string>(p,"NodeSet"));
}
/*!
 * \brief NODEFLOW::NodeSetType::save
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::NodeSetType::save(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::save(dlg,ns, p);
    ns.data().setValue(p,"NodeSet",dlg->getString(PropField1));

}

/*!
 * \brief NODEFLOW::NodeSetType::process
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return true on success / posted
 */
bool NODEFLOW::NodeSetType::process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
{
    NodePtr &n = ns.findNode(nodeId);
    if(n && n->enabled())
    {
        switch(id)
        {
        case 0:
        {
            SetNode * s = static_cast<SetNode *>(n.get());
            s->set().step(data);
            VALUEQUEUE &q = s->set().outValue(); // there may be zero or more output values - topics can be different
            while(q.size() > 0)
            {
                post(ns,nodeId,Output,q.front()); // forward item on queue
                q.pop();
            }
            true;
        }
        default:
            break;
        }
    }
    return false;
}
