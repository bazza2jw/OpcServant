#include "ClientBrowserDialog.h"

ClientBrowserDialog::ClientBrowserDialog(wxWindow* parent, const std::string &h, const std::string &u, const std::string &p, bool us  )
    : ClientBrowserDialogBase(parent),
      _host(h),
      _user(u),
      _password(p),
      _useSecurity(us)
{
    if(us)
    {
       if( _client.connectUsername(_host,_user,_password))
       {
            doBrowse();
       }
       else
       {
           wxLogDebug("Failed to connect");
       }
    }
    else
    {
        if(_client.connect(_host))
        {
            doBrowse();
        }
        else
        {
            wxLogDebug("Failed to connect");
        }
    }
}
/*!
 * \brief ClientBrowserDialog::iterateNodes
 * \param n
 * \param parent
 */
void ClientBrowserDialog::iterateNodes(Open62541::UANode *n,  wxTreeItemId parent)
{
   wxTreeItemId cur = GetBrowseTree()->AppendItem(parent,n->name());
   //
   std::cerr << " Name " << n->name() << " Node ID " << Open62541::toString(n->data()) << std::endl;
   for (auto i = n->children().begin(); i != n->children().end(); i++) {
           iterateNodes(i->second,cur);
   }
}

/*!
 * \brief ClientBrowserDialog::doBrowse
 */
void ClientBrowserDialog::doBrowse()
{
    Open62541::UANodeTree _tree(Open62541::NodeId::Objects); // browse tree
    GetBrowseTree()->DeleteAllItems(); // clear the tree
    _root =  GetBrowseTree()->AddRoot(_host); // set the root item to the host name
    _client.browseTree(Open62541::NodeId::Objects,_tree); // browse the node tree
    for (auto i = _tree.root().children().begin(); i != _tree.root().children().end(); i++) {
            iterateNodes(i->second,_root);
    }
}


ClientBrowserDialog::~ClientBrowserDialog()
{
}

void ClientBrowserDialog::onOk(wxCommandEvent& /*event*/)
{
    // get the current ly selected item
    wxTreeItemId sel = GetBrowseTree()->GetSelection();
    if(sel.IsOk())
    {
        _itemPath.clear();
        while(sel.IsOk())
        {
            wxString t = GetBrowseTree()->GetItemText(sel);
            _itemPath.push_front(t.ToStdString());
            sel = GetBrowseTree()->GetItemParent(sel);
        }
        // we have the path to the selected item
        _itemPath.pop_front(); // remove the top most item
    }
    EndModal(wxID_OK);
}
void ClientBrowserDialog::onSelChanged(wxTreeEvent& event)
{
}
