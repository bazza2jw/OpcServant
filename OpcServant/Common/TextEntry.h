#ifndef TEXTENTRY_H
#define TEXTENTRY_H
#include "VirtualKeypadUI.h"
class TextEntry : public TextEntryBase
{
public:
    TextEntry(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~TextEntry();
    //
    void SetValue(const wxString &s) { GetText()->SetValue(s);}
    wxString GetValue()  { return GetText()->GetValue();}
    //
protected:
    virtual void onFocus(wxFocusEvent& event); // on focus open the virtual keyboard
};
#endif // TEXTENTRY_H
