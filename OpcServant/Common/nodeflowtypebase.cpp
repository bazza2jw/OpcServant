#include "nodeflowtypebase.h"


/*!
 * \brief NODEFLOW::NodeType::properties
 * \param parent
 * \param ns
 * \param nodeId
 * \return
 */
bool NODEFLOW::NodeType::properties(wxWindow *parent, NodeSet &ns, unsigned nodeId)
{
    try
    {
        // property page
        MRL::PropertyPath p;
        NodePtr &n = ns.findNode(nodeId);
        n->toPath(p);
        PropertiesEditorDialog dlg(parent,ns.data(),p);
        //
        load(dlg,ns,p);
        if(dlg.ShowModal() == wxID_OK)
        {
            save(dlg,ns,p);
            return true;
        }
    }
    CATCH_DEF
    return false;
}

/*!
 * \brief NODEFLOW::NodeType::properties
 * \param parent
 * \param ns
 * \param nodeId
 * \return
 */
bool NODEFLOW::NodeType::properties(Wt::WWidget *parent, NodeSet &ns, unsigned nodeId)
{
    MRL::PropertyPath p;
    NodePtr &n = ns.findNode(nodeId);
    n->toPath(p);
    std::unique_ptr<NODEFLOW::WebPropertiesDialog> pd = std::make_unique<NODEFLOW::WebPropertiesDialog>(ns,n);
    NODEFLOW::WebDialogBase::showDialog<NODEFLOW::WebPropertiesDialog>(parent,pd);
    return true; // does not block
}
