#ifndef TEXTCHOICE_H
#define TEXTCHOICE_H
#include "VirtualKeypadUI.h"

class TextChoice : public TextChoiceBase
{
public:
    TextChoice(wxWindow* parent);
    virtual ~TextChoice();
protected:
    virtual void onOK(wxCommandEvent& event);
};
#endif // TEXTCHOICE_H
