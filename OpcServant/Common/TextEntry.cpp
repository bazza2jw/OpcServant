#include "TextEntry.h"
#include "VKeyboardPanel.h"

/*!
 * \brief TextEntry::TextEntry
 * \param parent
 */
TextEntry::TextEntry(wxWindow* parent, wxWindowID id)
    : TextEntryBase(parent,id)
{
}

/*!
 * \brief TextEntry::~TextEntry
 */
TextEntry::~TextEntry()
{
}

void TextEntry::onFocus(wxFocusEvent& /*event*/)
{
    VKeyboardPanel::keyboard()->setControl(GetText());
}
