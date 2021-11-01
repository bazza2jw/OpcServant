#ifndef I2CSETUPDIALOG_H
#define I2CSETUPDIALOG_H
#include <Common/Gui/SetupDialog.h>

class I2cSetupDialog : public SetupDialog
{
public:
    I2cSetupDialog(wxWindow* parent, const MRL::PropertyPath & _path);
protected:
    virtual void onConfigure(wxCommandEvent& event);
    virtual int addFields(wxFlexGridSizer *, int row);

};

#endif // I2CSETUPDIALOG_H
