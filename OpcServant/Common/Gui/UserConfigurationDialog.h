#ifndef USERCONFIGURATIONDIALOG_H
#define USERCONFIGURATIONDIALOG_H
#include "CommonGUI.h"
#include <Common/common.h>

/*!
 * \brief The UserConfigurationDialog class
 */
class UserConfigurationDialog : public UserConfigurationDialogBase
{
    MRL::VariantPropertyTree &_settings;
public:
    UserConfigurationDialog(wxWindow* parent,MRL::VariantPropertyTree &s);
    virtual ~UserConfigurationDialog();
protected:
    virtual void onOk(wxCommandEvent& event);
    virtual void onSelection(wxCommandEvent& event);
    virtual void onApply(wxCommandEvent& event);
    virtual void onDelete(wxCommandEvent& event);
    virtual void onNew(wxCommandEvent& event);
    void setup();
};
#endif // USERCONFIGURATIONDIALOG_H
