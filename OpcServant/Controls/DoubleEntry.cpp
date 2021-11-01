#include "DoubleEntry.h"
#include "NumpadDialog.h"
/*!
 * \brief DoubleEntry::DoubleEntry
 * \param parent
 */
DoubleEntry::DoubleEntry(wxWindow* parent)
    : DoubleEntryBase(parent)
{
    GetValue()->SetRange(-100000,100000);
    GetValue()->SetIncrement(0.1);
    GetValue()->SetDigits(2);
}

/*!
 * \brief DoubleEntry::~DoubleEntry
 */
DoubleEntry::~DoubleEntry()
{

}

/*!
 * \brief DoubleEntry::onKeypad
 */
void DoubleEntry::onKeypad(wxCommandEvent& /*event*/)
{
    NumpadDialog dlg(this);
    wxString s = wxString::Format("%g",GetValue()->GetValue());
    dlg.GetKeypad()->GetValue()->SetValue(s);
    if(dlg.ShowModal() == wxID_OK)
    {
        // set value from number pad
        wxString s = dlg.GetKeypad()->GetValue()->GetValue();
        GetValue()->SetValue(s);
    }
}
/*!
 * \brief DoubleEntry::getValue
 * \return
 */
double DoubleEntry::getValue()
{
    return GetValue()->GetValue();
}

/*!
 * \brief DoubleEntry::setValue
 * \param v
 */
void DoubleEntry::setValue(double v)
{
    GetValue()->SetValue(v);
}
