#include "MimicDisplayConfigurationDialog.h"
#include "MimicEditorDialog.h"

MimicDisplayConfigurationDialog::MimicDisplayConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : MimicDisplayConfigurationDialogBase(parent),_path(path)
{
    Json::Value v;
    if(MRL::Common::configuration().getData(path,v))
    {
        _configuration.fromJson(v); // load configuration
        GetUpdateInterval()->SetValue(_configuration.getInt("/UpdateInterval"));
        GetEnabled()->SetValue(_configuration.getBool("/Enabled"));
        GetEnableTabView()->SetValue(_configuration.getBool("/EnableTabView"));
        //
        wxArrayString l;
        wxString dir(MRL::Common::baseDir());
        dir += "/mimic";
        MRL::getFileList(l,dir,"*.mim");
        GetFileName()->Append(l);
        MRL::SetChoice(GetFileName(),_configuration.getString("/Filename"));

    }
}
/*!
 * \brief MimicDisplayConfigurationDialog::~MimicDisplayConfigurationDialog
 */
MimicDisplayConfigurationDialog::~MimicDisplayConfigurationDialog()
{
}
/*!
 * \brief MimicDisplayConfigurationDialog::onEdit
 * \param event
 */
void MimicDisplayConfigurationDialog::onEdit(wxCommandEvent& /*event*/)
{
    MimicEditorDialog dlg(this,MRL::GetChoice(GetFileName()));  // open the editor
    dlg.ShowModal();
    //
    std::string s = MRL::GetChoice(GetFileName());
    //
    // update list - maybe new ones added
    wxArrayString l;
    wxString dir(MRL::Common::baseDir());
    dir += "/mimic";
    MRL::getFileList(l,dir,"*.mim");
    GetFileName()->Clear();
    GetFileName()->Append(l);
    MRL::SetChoice(GetFileName(),s);
}
/*!
 * \brief MimicDisplayConfigurationDialog::onOk
 * \param event
 */
void MimicDisplayConfigurationDialog::onOk(wxCommandEvent& event)
{
    _configuration.setInt("/UpdateInterval",GetUpdateInterval()->GetValue());
    _configuration.setBool("/Enabled",GetEnabled()->GetValue());
    _configuration.setBool("/EnableTabView",GetEnableTabView()->GetValue());
    _configuration.setString("/Filename",MRL::GetChoice(GetFileName()));
    //
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
