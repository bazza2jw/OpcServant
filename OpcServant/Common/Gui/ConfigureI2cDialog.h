#ifndef CONFIGUREI2CDIALOG_H
#define CONFIGUREI2CDIALOG_H
#include "BourbonCommonUI.h"
#include <Common/common.h>
/*!
 * \brief The ConfigureI2cDialog class
 */
class ConfigureI2cDialog : public ConfigureI2cDialogBase
{
    MRL::VariantPropertyTree &_configuration; // tree of configuration values

public:
    ConfigureI2cDialog(wxWindow* parent, MRL::VariantPropertyTree &c);
    virtual ~ConfigureI2cDialog();
protected:
    virtual void onTypeChanged(wxCommandEvent& event);
    virtual void onOK(wxCommandEvent& event);
    void onI2cTypeSet();
};
#endif // CONFIGUREI2CDIALOG_H
