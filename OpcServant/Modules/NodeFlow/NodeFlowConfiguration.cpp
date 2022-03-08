#include "NodeFlowConfiguration.h"
/*!
 * \brief NodeFlowConfiguration::NodeFlowConfiguration
 * \param parent
 * \param path
 */
NodeFlowConfiguration::NodeFlowConfiguration(wxWindow* parent, const MRL::PropertyPath &path)
    : NodeFlowConfigurationBase(parent),_path(path)
{
    Json::Value v;
    if(MRL::Common::configuration().getData(_path,v))
    {
        _configuration.fromJson(v); // load configuration
        //
        // Get the list of flows
        GetEnabled()->SetValue(_configuration.getBool("/Enabled"));
        //
        MRL::StringList l;
        _configuration.listChildren("/Inputs",l);
        for(auto i = l.begin(); i != l.end(); i++)
        {
            wxString s(*i);
            GetListInputs()->Append(s);
        }
        GetListInputs()->SetSelection(0);
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
void NodeFlowConfiguration::onOK(wxCommandEvent& event)
{
    _configuration.setValue("/Enabled",GetEnabled()->GetValue());
    _configuration.remove("/Inputs");
    MRL::PropertyPath p;
    p.push_back("Inputs");
    for(int i = 0; i < GetListInputs()->GetCount(); i++)
    {
        std::string s = GetListInputs()->GetString(i).ToStdString();
        _configuration.setValue(p,s,true);
    }
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
/*!
 * \brief NodeFlowConfiguration::onAdd
 * \param event
 */
void NodeFlowConfiguration::onAdd(wxCommandEvent& event)
{
   GetListInputs()->Append(GetInputName()->GetValue());
}
void NodeFlowConfiguration::onRemove(wxCommandEvent& event)
{
    int i = GetListInputs()->GetSelection();
    if(i != wxNOT_FOUND)
    {
        GetListInputs()->Delete(i);
    }
}
