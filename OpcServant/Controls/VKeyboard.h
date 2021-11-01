#ifndef VKEYBOARD_H
#define VKEYBOARD_H
#include "keypad.h"

class VKeyboard : public VKeyboardBase
{
public:
    VKeyboard(wxWindow* parent);
    virtual ~VKeyboard();
protected:
    virtual void onSpace(wxCommandEvent& event);
    virtual void onKey(wxCommandEvent& event);
    virtual void onBack(wxCommandEvent& event);
    virtual void onClear(wxCommandEvent& event);
};
#endif // VKEYBOARD_H
