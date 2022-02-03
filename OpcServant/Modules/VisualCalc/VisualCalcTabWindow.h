#ifndef VISUALCALCTABWINDOW_H
#define VISUALCALCTABWINDOW_H
#include "VisualCalcUI.h"

class VisualCalcTabWindow : public VisualCalcTabWindowBase
{
public:
    VisualCalcTabWindow(wxWindow* parent);
    virtual ~VisualCalcTabWindow();
protected:
    virtual void onStartTimer(wxTimerEvent& event);
};
#endif // VISUALCALCTABWINDOW_H
