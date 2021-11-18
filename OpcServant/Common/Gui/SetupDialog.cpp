#include "SetupDialog.h"

/*!
 * \brief SetupDialog::SetupDialog
 * \param parent
 * \param path
 * \param dataPath
 */
SetupDialog::SetupDialog(wxWindow* parent, const MRL::PropertyPath &path, const MRL::PropertyPath &dataPath)
    : SetupDialogBase(parent),_path(path),_dataPath(dataPath)
{
    Json::Value v;
    if(MRL::Common::configuration().getData(path,v))
    {
        _configuration.fromJson(v); // load configuration
        setFields(_dataPath);
    }
}

SetupDialog::~SetupDialog()
{
}
/*!
 * \brief SetupDialog::onOk
 * \param event
 */
void SetupDialog::onOk(wxCommandEvent& /*event*/)
{
    getFields(_dataPath);
    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);
    EndModal(wxID_OK);
}
/*!
 * \brief SetupDialog::setFields
 */
void SetupDialog::setFields(MRL::PropertyPath &p)
{
    GetMeasureInterval()->SetValue(_configuration.getValue<int>(p,"MeasureInterval"));
    GetPublishInterval()->SetValue(_configuration.getValue<int>(p,"PublishInterval"));
    GetEnabled()->SetValue(_configuration.getValue<bool>(p,"Enabled"));
    GetEnableTabView()->SetValue(_configuration.getValue<bool>(p,"EnableTabView"));

}

/*!
 * \brief SetupDialog::getFields
 */
void SetupDialog::getFields(MRL::PropertyPath &p)
{
    _configuration.setValue(p,"EnableTabView",GetEnableTabView()->GetValue());
    _configuration.setValue(p,"MeasureInterval", GetMeasureInterval()->GetValue());
    _configuration.setValue(p,"PublishInterval",GetPublishInterval()->GetValue());
    _configuration.setValue(p,"Enabled", GetEnabled()->GetValue());
}
