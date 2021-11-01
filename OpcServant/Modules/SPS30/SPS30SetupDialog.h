#ifndef SPS30SETUPDIALOG_H
#define SPS30SETUPDIALOG_H
#include "SPS30UI.h"

class SPS30SetupDialog : public SPS30SetupDialogBase
{
public:
    SPS30SetupDialog(wxWindow* parent);
    virtual ~SPS30SetupDialog();
protected:
    virtual void onConfigure(wxCommandEvent& event);
    virtual void onOk(wxCommandEvent& event);
};
#endif // SPS30SETUPDIALOG_H
