#ifndef CONFIGURATIONPROPERTYSHEET_H
#define CONFIGURATIONPROPERTYSHEET_H
#include "BourbonCommonUI.h"

class ConfigurationPropertySheet : public ConfigurationPropertySheetBase
{
public:
    ConfigurationPropertySheet(wxWindow* parent);
    virtual ~ConfigurationPropertySheet();
protected:
    virtual void onOk(wxCommandEvent& event);
};
#endif // CONFIGURATIONPROPERTYSHEET_H
