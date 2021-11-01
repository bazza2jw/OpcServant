#ifndef DOUBLEENTRY_H
#define DOUBLEENTRY_H
#include "textentry.h"

class DoubleEntry : public DoubleEntryBase
{
public:
    DoubleEntry(wxWindow* parent);
    virtual ~DoubleEntry();
    double getValue();
    void setValue(double);
protected:
    virtual void onKeypad(wxCommandEvent& event);
};
#endif // DOUBLEENTRY_H
