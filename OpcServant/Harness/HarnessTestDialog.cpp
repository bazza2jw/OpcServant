#include "HarnessTestDialog.h"
#include "buiapp.h"
HarnessTestDialog::HarnessTestDialog(wxWindow* parent)
    : HarnessTestDialogBase(parent)
{
    wxArrayString a;
    a.Add("Yes");
    a.Add("No");
    a.Add("Maybe");
    GetTextSpin()->setItems(a);
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
