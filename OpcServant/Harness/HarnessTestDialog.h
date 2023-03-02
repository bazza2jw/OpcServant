#ifndef HARNESSTESTDIALOG_H
#define HARNESSTESTDIALOG_H
#include "HarnessUI.h"

class HarnessTestDialog : public HarnessTestDialogBase
{
public:
    HarnessTestDialog(wxWindow* parent);
    virtual ~HarnessTestDialog();
protected:
    virtual void onCancel(wxCommandEvent& event);
    virtual void onOk(wxCommandEvent& event);
};
#endif // HARNESSTESTDIALOG_H
