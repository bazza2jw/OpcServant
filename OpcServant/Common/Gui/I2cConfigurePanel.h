#ifndef I2CCONFIGUREPANEL_H
#define I2CCONFIGUREPANEL_H
#include "BourbonCommonUI.h"
#include <Common/common.h>

/*!
 * \brief The I2cConfigurePanel class
 */
class I2cConfigurePanel : public I2cConfigurePanelBase
{
public:
    /*!
     * \brief I2cConfigurePanel
     * \param parent
     */
    I2cConfigurePanel(wxWindow* parent);
    /*!
     * \brief ~I2cConfigurePanel
     */
    virtual ~I2cConfigurePanel();
    /*!
     * \brief read
     * \param c
     */
    virtual void read( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        GetI2cType()->SetSelection(c.getValue<int>(p,"I2CType"));
        wxString s(c.getValue<std::string>(p,"I2CAddress"));
        GetI2cDevice()->SetValue(s);
        GetI2cPort()->SetValue(c.getValue<int>(p,"I2CPort"));
    }
    /*!
     * \brief write
     * \param c
     */
    virtual void write( MRL::VariantPropertyTree &c, MRL::PropertyPath &p)
    {
        c.setValue(p,"I2CType",GetI2cType()->GetSelection());
        c.setValue(p,"I2CAddress",GetI2cDevice()->GetValue().ToStdString());
        c.setValue(p,"I2CPort",GetI2cPort()->GetValue());
    }
};
#endif // I2CCONFIGUREPANEL_H
