#ifndef CLIENTBROWSERDIALOG_H
#define CLIENTBROWSERDIALOG_H
#include "OpcClientUI.h"
#include <Common/common.h>
#include <open62541cpp/open62541client.h>
#include <open62541cpp/clientbrowser.h>

class ClientBrowserDialog : public ClientBrowserDialogBase
{
    std::string _host;
    std::string _user;
    std::string _password;
    bool _useSecurity = false;
    Open62541::Client _client;
    wxTreeItemId _root;
    wxTreeItemId _parent; // current parent
    MRL::PropertyPath _itemPath;

public:
    ClientBrowserDialog(wxWindow* parent, const std::string &h, const std::string &u, const std::string &p, bool us = false);
    virtual ~ClientBrowserDialog();
    void doBrowse();
    void iterateNodes(Open62541::UANode *n,wxTreeItemId parent);
    MRL::PropertyPath & itemPath() { return  _itemPath;}
protected:
    virtual void onSelChanged(wxTreeEvent& event);
    virtual void onOk(wxCommandEvent& event);
};
#endif // CLIENTBROWSERDIALOG_H
