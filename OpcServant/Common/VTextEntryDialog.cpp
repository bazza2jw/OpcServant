#include "VTextEntryDialog.h"
/*!
 * \brief VTextEntryDialog::VTextEntryDialog
 * \param parent
 */
VTextEntryDialog::VTextEntryDialog(wxWindow* parent)
    : VTextEntryDialogBase(parent)
{
}
/*!
 * \brief VTextEntryDialog::~VTextEntryDialog
 */
VTextEntryDialog::~VTextEntryDialog()
{
}
/*!
 * \brief VTextEntryDialog::onOK
 * \param event
 */
void VTextEntryDialog::onOK(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}
/*!
 * \brief VTextEntryDialog::setText
 * \param s
 */
void VTextEntryDialog::setText(const wxString &s)
{
    GetTextEntry()->SetValue(s);
}
/*!
 * \brief VTextEntryDialog::getText
 * \return
 */
wxString VTextEntryDialog::getText()
{
    return GetTextEntry()->GetValue();
}
/*!
 * \brief VTextEntryDialog::onChar
 * \param event
 */
void VTextEntryDialog::onChar(wxCommandEvent& event)
{
    static const char * keys = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ.-_/ ";
    int i = event.GetId() - 20000;
    char c = keys[i];
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = c;
    e.SetShiftDown(GetCapsLock()->GetValue());
    GetTextEntry()->EmulateKeyPress(e);
}
/*!
 * \brief VTextEntryDialog::onDelete
 */
void VTextEntryDialog::onDelete(wxCommandEvent& /*event*/)
{
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = WXK_BACK;
    GetTextEntry()->EmulateKeyPress(e);
}
/*!
 * \brief VTextEntryDialog::onCancel
 * \param event
 */
void VTextEntryDialog::onCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
/*!
 * \brief VTextEntryDialog::onCapsLock
 * \param event
 */
void VTextEntryDialog::onCapsLock(wxCommandEvent& event)
{
    _capsLock = GetCapsLock()->GetValue();
}
