#include "HarnessTestDialog.h"
#include "buiapp.h"
HarnessTestDialog::HarnessTestDialog(wxWindow* parent)
    : HarnessTestDialogBase(parent)
{
}

HarnessTestDialog::~HarnessTestDialog()
{
}

void HarnessTestDialog::onCancel(wxCommandEvent& event)
{
    BuiApp::GetInstance()->Exit();
}
void HarnessTestDialog::onOk(wxCommandEvent& event)
{
    BuiApp::GetInstance()->Exit();
}
