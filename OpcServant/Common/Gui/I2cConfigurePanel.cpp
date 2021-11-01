#include "I2cConfigurePanel.h"

I2cConfigurePanel::I2cConfigurePanel(wxWindow* parent)
    : I2cConfigurePanelBase(parent)
{
    wxDir d("/dev");
    if(d.IsOpened())
    {
        wxString f;
        if(d.GetFirst (&f, "*i2c*", wxDIR_FILES ))
        {
            do
            {
                wxString s = "/dev/" + f;
                GetI2cDevice()->AppendString(s);
            } while(d.GetNext(&f));
        }
    }

}

I2cConfigurePanel::~I2cConfigurePanel()
{
}

