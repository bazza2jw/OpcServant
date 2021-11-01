#include "ConfigureI2cDialog.h"
/*!
 * \brief ConfigureI2cDialog::ConfigureI2cDialog
 * \param parent
 * \param c
 */
ConfigureI2cDialog::ConfigureI2cDialog(wxWindow* parent, MRL::VariantPropertyTree &c)
    : ConfigureI2cDialogBase(parent),_configuration(c)
{
    GetI2cType()->SetSelection(_configuration.getInt("/I2CType"));
    GetI2cDevice()->SetValue(_configuration.getString("/I2CAddress"));
    GetI2cPort()->SetValue(_configuration.getInt("/I2CPort"));

}

/*!
 * \brief ConfigureI2cDialog::~ConfigureI2cDialog
 */
ConfigureI2cDialog::~ConfigureI2cDialog()
{
}

/*!
 * \brief ConfigureI2cDialog::onOK
 * \param event
 */
void ConfigureI2cDialog::onOK(wxCommandEvent& event)
{
    _configuration.setInt("/I2CType",GetI2cType()->GetSelection());
    _configuration.setString("/I2CAddress",GetI2cDevice()->GetValue().ToStdString());
    _configuration.setInt("/I2CPort",GetI2cPort()->GetValue());
    EndModal(wxID_OK);
}

/*!
 * \brief ConfigureI2cDialog::onI2cTypeSet
 */
void ConfigureI2cDialog::onI2cTypeSet()
{
    switch(GetI2cType()->GetSelection())
    {
    case 0:
    {
        // Local
        GetI2cDevice()->Clear();
        // look for i2c
        wxDir d("/dev");
        if(d.IsOpened())
        {
            wxString f;
            if(d.GetFirst (&f, "*i2c*", wxDIR_FILES ))
            {
                do
                {
                    GetI2cDevice()->Append(f);
                } while(d.GetNext(&f));
            }
        }
        GetI2cPort()->Enable(false);
    }
    break;
    case 1:
    {
        GetI2cDevice()->Clear();
        GetI2cDevice()->Append("localhost");
        GetI2cPort()->Enable();
    }
    break;
    default:
        break;
    }
}
/*!
 * \brief ConfigureI2cDialog::onTypeChanged
 * \param event
 */
void ConfigureI2cDialog::onTypeChanged(wxCommandEvent& event)
{
    onI2cTypeSet();
}
