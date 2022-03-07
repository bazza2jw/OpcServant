#include "OpcClientConfigurationDialog.h"
#include "ClientBrowserDialog.h"
/*!
 * \brief OpcClientConfigurationDialog::OpcClientConfigurationDialog
 * \param parent
 */
OpcClientConfigurationDialog::OpcClientConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : OpcClientConfigurationDialogBase(parent),_path(path)
{

    Json::Value v;
    if(MRL::Common::configuration().getData(path,v))
    {
        _configuration.fromJson(v); // load configuration
        GetServerHost()->SetValue(_configuration.getString("/ServerHost"));
        GetUsername()->SetValue(_configuration.getString("/Username"));
        GetPassword()->SetValue(_configuration.getString("/Password"));
        GetUseSecurity()->SetValue(_configuration.getBool("/UseSecurity"));
        GetEnabled()->SetValue(_configuration.getBool("/Enabled"));
        //
        // Now get the subscription list
        //
        MRL::StringList l;
        _configuration.listChildren("/Subs",l);
        //
        MRL::PropertyPath lp;
        lp.push_back("Subs");
        //
        for(auto i = l.begin(); i != l.end(); i++)
        {
            std::string s = _configuration.getValue<std::string>(lp,*i);
            wxVector<wxVariant> data;
            data.push_back(wxVariant(wxString(*i)));
            data.push_back(wxVariant(wxString(s)));
            GetListCtl()->AppendItem(data);
        }

    }
}
/*!
 * \brief OpcClientConfigurationDialog::~OpcClientConfigurationDialog
 */
OpcClientConfigurationDialog::~OpcClientConfigurationDialog()
{
}
/*!
 * \brief OpcClientConfigurationDialog::onBrowse
 * \param event
 */
void OpcClientConfigurationDialog::onBrowse(wxCommandEvent& event)
{
    // browse for a value
    std::string _host =  GetServerHost()->GetValue().ToStdString();
    std::string _user = GetUsername()->GetValue().ToStdString();
    std::string _password = GetPassword()->GetValue().ToStdString();
    bool _useSecurity = GetUseSecurity()->GetValue();

    ClientBrowserDialog dlg(this,_host,_user,_password,_useSecurity);
    if(dlg.ShowModal() == wxID_OK)
    {
        MRL::PropertyPath &ip = dlg.itemPath();
        std::string s;
        ip.toString(s);
        GetBrowseItem()->SetValue(s);

    }
}
/*!
 * \brief OpcClientConfigurationDialog::onOk
 * \param event
 */
void OpcClientConfigurationDialog::onOk(wxCommandEvent& /*event*/)
{
    _configuration.setString("/ServerHost",GetServerHost()->GetValue().ToStdString());
    _configuration.setString("/Username",GetUsername()->GetValue().ToStdString());
    _configuration.setString("/Password",GetPassword()->GetValue().ToStdString());
    _configuration.setBool("/UseSecurity",GetUseSecurity()->GetValue());
    _configuration.setBool("/Enabled",GetEnabled()->GetValue());
    //
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
    //
}
/*!
 * \brief OpcClientConfigurationDialog::onAdd
 * \param event
 */
void OpcClientConfigurationDialog::onAdd(wxCommandEvent& /*event*/)
{
    wxString  id = GetValueId()->GetValue().Trim(true).Trim(false);
    // Search for the id
    for(int i = 0; i < GetListCtl()->GetItemCount(); i++)
    {
        if(GetListCtl()->GetTextValue(i,0) == id) return;
    }
    wxVector<wxVariant> data;
    data.push_back(wxVariant(id));
    data.push_back(wxVariant(GetBrowseItem()->GetValue()));
    GetListCtl()->AppendItem(data);
    //
    MRL::PropertyPath p;
    p.push_back("Subs");
    p.push_back(id.ToStdString());
    _configuration.setValue(p,GetBrowseItem()->GetValue().ToStdString());
}
/*!
 * \brief OpcClientConfigurationDialog::onRemove
 * \param event
 */
void OpcClientConfigurationDialog::onRemove(wxCommandEvent& /*event*/)
{
    int i = GetListCtl()->GetSelectedRow();
    if(i != wxNOT_FOUND)
    {
        MRL::PropertyPath p;
        p.push_back("Subs");
        p.push_back(GetListCtl()->GetTextValue(i,0).ToStdString());
        _configuration.remove(p); // delete the item
        GetListCtl()->DeleteItem(i);
    }
}
