#include "i2csetupdialog.h"
#include <Common/Gui/ConfigureI2cDialog.h>

/*!
 * \brief I2cSetupDialog::I2cSetupDialog
 * \param parent
 * \param path
 */
I2cSetupDialog::I2cSetupDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : SetupDialog(parent,path)
{

}

/*!
 * \brief I2cSetupDialog::addFields
 * \param sizer
 * \param row
 * \return
 */
int I2cSetupDialog::addFields(wxFlexGridSizer *sizer, int row)
{
    row = SetupDialog::addFields(sizer,row);
    wxButton* m_buttonConfigure;
    m_buttonConfigure = new wxButton(this, wxID_ANY, _("I2C Configure ..."));
    m_buttonConfigure->SetDefault();
    sizer->Add(m_buttonConfigure, 0, wxALL, WXC_FROM_DIP(5));
    m_buttonConfigure->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                               wxCommandEventHandler(I2cSetupDialog::onConfigure), NULL, this);
    return ++row;
}

/*!
 * \brief I2cSetupDialog::onConfigure
 * \param event
 */
void I2cSetupDialog::onConfigure(wxCommandEvent& /*event*/)
{
    ConfigureI2cDialog dlg(this,configuration());
    dlg.ShowModal();
}
