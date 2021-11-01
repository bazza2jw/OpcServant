#include "VKeyboard.h"

/*!
 * \brief VKeyboard::VKeyboard
 * \param parent
 */



VKeyboard::VKeyboard(wxWindow* parent)
    : VKeyboardBase(parent)
{
    // add the keypad

}
/*!
 * \brief VKeyboard::~VKeyboard
 */
VKeyboard::~VKeyboard()
{
}
/*!
 * \brief VKeyboard::onBack
 * \param event
 */
void VKeyboard::onBack(wxCommandEvent& /*event*/)
{
    wxString s = GetValue()->GetValue();
    s.RemoveLast();
    GetValue()->SetValue(s);
}
/*!
 * \brief VKeyboard::onClear
 * \param event
 */
void VKeyboard::onClear(wxCommandEvent& /*event*/)
{
    GetValue()->Clear();
}


/*!
 * \brief VKeyboard::onKey
 * \param event
 */
void VKeyboard::onKey(wxCommandEvent&event)
{
    wxWindow *w = FindWindow (event.GetId());
    if(w)
    {
        wxString s = w->GetLabel();
        if(!GetShift()->GetValue())
        {
            s.LowerCase();
        }
        GetValue()->AppendText(s);
    }

}
/*!
 * \brief VKeyboard::onSpace
 * \param event
 */
void VKeyboard::onSpace(wxCommandEvent& event)
{
    GetValue()->AppendText(" ");
}
