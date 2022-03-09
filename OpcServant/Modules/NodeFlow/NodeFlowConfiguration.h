#ifndef NODEFLOWCONFIGURATION_H
#define NODEFLOWCONFIGURATION_H
#include "NodeFlowUI.h"

#include <Common/common.h>
#include <MrlLib/variantpropertytree.h>

class NodeFlowConfiguration : public NodeFlowConfigurationBase
{
    const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree _configuration; // tree of configuration values

public:
    NodeFlowConfiguration(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~NodeFlowConfiguration();
protected:
    virtual void onEdit(wxCommandEvent& event);
    virtual void onOK(wxCommandEvent& event);
};
#endif // NODEFLOWCONFIGURATION_H
