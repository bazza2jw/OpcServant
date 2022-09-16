#ifndef TOGGLEOPTION_H
#define TOGGLEOPTION_H
#include "VirtualKeypadUI.h"

class ToggleOption : public ToggleOptionBase
{
public:
    ToggleOption(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~ToggleOption();
};
#endif // TOGGLEOPTION_H
