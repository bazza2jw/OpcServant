#include "PinEntryDialog.h"
// simple PIN entry

PinEntryDialog::PinEntryDialog(wxWindow* parent)
    : PinEntryDialogBase(parent)
{
}

PinEntryDialog::~PinEntryDialog()
{
}

void PinEntryDialog::on0(wxCommandEvent& event)
{
    addChar('0');
}
void PinEntryDialog::on1(wxCommandEvent& event)
{
    addChar('1');
}
void PinEntryDialog::on2(wxCommandEvent& event)
{
    addChar('2');
}
void PinEntryDialog::on3(wxCommandEvent& event)
{
    addChar('3');
}
void PinEntryDialog::on4(wxCommandEvent& event)
{
    addChar('4');
}
void PinEntryDialog::on5(wxCommandEvent& event)
{
    addChar('5');
}
void PinEntryDialog::on6(wxCommandEvent& event)
{
    addChar('6');
}
void PinEntryDialog::on7(wxCommandEvent& event)
{
    addChar('7');
}
void PinEntryDialog::on8(wxCommandEvent& event)
{
    addChar('8');
}
void PinEntryDialog::on9(wxCommandEvent& event)
{
    addChar('9');
}
void PinEntryDialog::onDP(wxCommandEvent& event)
{
    // remove last character
    wxString s = GetText()->GetValue();
    if(s.Length() > 0)
    {
        s.Truncate(s.Length() - 1);
        GetText()->SetValue(s);
    }
}
void PinEntryDialog::onOk(wxCommandEvent& /*event*/)
{
    Hide();
}
