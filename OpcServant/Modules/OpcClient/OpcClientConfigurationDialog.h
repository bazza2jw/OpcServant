#ifndef OPCCLIENTCONFIGURATIONDIALOG_H
#define OPCCLIENTCONFIGURATIONDIALOG_H
#include "OpcClientUI.h"
#include <Common/common.h>
#include <open62541cpp/open62541client.h>
#include <open62541cpp/clientbrowser.h>

class OpcClientConfigurationDialog : public OpcClientConfigurationDialogBase
{
    const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree _configuration; // tree of configuration values

public:
    OpcClientConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~OpcClientConfigurationDialog();
protected:
    virtual void onAdd(wxCommandEvent& event);
    virtual void onRemove(wxCommandEvent& event);
    virtual void onOk(wxCommandEvent& event);
    virtual void onBrowse(wxCommandEvent& event);
};
#endif // OPCCLIENTCONFIGURATIONDIALOG_H
