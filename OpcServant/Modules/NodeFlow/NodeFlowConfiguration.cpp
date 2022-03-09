#include "NodeFlowConfiguration.h"
#include "FlowEditorDialog.h"
/*!
 * \brief NodeFlowConfiguration::NodeFlowConfiguration
 * \param parent
 * \param path
 */
NodeFlowConfiguration::NodeFlowConfiguration(wxWindow* parent, const MRL::PropertyPath &path)
    : NodeFlowConfigurationBase(parent),_path(path)
{
    //


    Json::Value v;
    if(MRL::Common::configuration().getData(_path,v))
    {
        _configuration.fromJson(v); // load configuration
        //
        // Get the list of flows
        wxFileName fn(_configuration.getString("/Flow"));
        GetEnabled()->SetValue(_configuration.getBool("/Enabled"));
        GetFlowList()->SetFileName(fn); // get the current flow
        //
    }
}
/*!
 * \brief NodeFlowConfiguration::~NodeFlowConfiguration
 */
NodeFlowConfiguration::~NodeFlowConfiguration()
{
}
/*!
 * \brief NodeFlowConfiguration::onOK
 * \param event
 */
void NodeFlowConfiguration::onOK(wxCommandEvent& /*event*/)
{
    _configuration.setValue("/Enabled",GetEnabled()->GetValue());
    _configuration.setString("/Flow",GetFlowList()->GetPath().ToStdString());
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
/*!
 * \brief NodeFlowConfiguration::onEdit
 * \param event
 */
void NodeFlowConfiguration::onEdit(wxCommandEvent& /*event*/)
{
    FlowEditorDialog dlg(this,GetFlowList()->GetPath());
    dlg.ShowModal();
}
