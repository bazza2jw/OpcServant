#include "TextEntry.h"
#include "VTextEntryDialog.h"
#include "virtualkeypaddate.h"
#include "virtualkeypadtime.h"
#include "VirtualKeypad.h"


wxDEFINE_EVENT(wxEVT_FIELD_UPDATED, wxCommandEvent);
/*!
 * \brief TextEntry::TextEntry
 * \param parent
 */
TextEntry::TextEntry(wxWindow* parent, wxWindowID id)
    : wxTextCtrl(parent,id)
{
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(TextEntry::onClick), NULL, this);
}

/*!
 * \brief TextEntry::~TextEntry
 */
TextEntry::~TextEntry()
{
    Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(TextEntry::onClick), NULL, this);
}

/*!
 * \brief TextEntry::openKeyPad
 */
bool TextEntry::openKeyPad()
{
    // open the virtual keyboard dialog
    VTextEntryDialog dlg(this);
    dlg.setText(GetValue());
    if(dlg.ShowModal() == wxID_OK)
    {
        wxString s = dlg.getText();
        SetValue(s);
        return true;
    }
    return false;
}

/*!
 * \brief DateEntry::openKeyPad
 */
bool DateEntry::openKeyPad()
{
    VirtualKeypadDate dlg(this);
    dlg.setDate(_date);
    // set the date
    if(dlg.ShowModal() == wxID_OK)
    {
        // get the date
        SetDate(dlg.getDate());
        return true;
    }
    return false;
}
/*!
 * \brief TimeEntry::openKeyPad
 */
bool TimeEntry::openKeyPad()
{
    VirtualKeypadTime dlg(this);
    dlg.setTime(_time);
    if(dlg.ShowModal() == wxID_OK)
    {
        SetTime(dlg.getTime());
        return true;
    }
    return false;
}
/*!
 * \brief IntEntry::openKeyPad
 */
bool IntEntry::openKeyPad()
{
    VirtualKeypad dlg(this);
    if(dlg.ShowModal() == wxID_OK)
    {
        int i = 0;
        dlg.GetTextEntry()->GetValue().ToInt(&i);
        if(i < _minimum) i = _minimum;
        if(i > _maximum) i = _maximum;
        SetValue( i );
        return true;
    }
    return false;
}
/*!
 * \brief DoubleEntry::openKeyPad
 */
bool DoubleEntry::openKeyPad()
{
    VirtualKeypad dlg(this);
    if(dlg.ShowModal() == wxID_OK)
    {
        double i = 0.0;
        dlg.GetTextEntry()->GetValue().ToDouble(&i);
        if(i < _minimum) i = _minimum;
        if(i > _maximum) i = _maximum;
        SetValue( i );
        return true;
    }
    return false;
}

