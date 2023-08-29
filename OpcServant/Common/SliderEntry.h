#ifndef SLIDERENTRY_H
#define SLIDERENTRY_H
#include "VirtualKeypadUI.h"

class SliderEntry : public SliderEntryBase
{
    double _minimum = 0.0;
    double _maximum = 1.0;
    wxString _format = "%8.3f";
    double _value = 0.0;

public:
    SliderEntry(wxWindow* parent);
    virtual ~SliderEntry();
    void SetMinimum(double v) { _minimum = v;}
    void SetMaximum(double v) {_maximum = v;}
    void SetFormat(const wxString &v) { _format = v;}
    virtual void SetValue(double v) {
        wxString s = wxString::Format(_format,v);
        GetText()->SetValue(s);
        _value = v;
    }
    int Value() const { return _value;}

protected:
    virtual void onChanged(wxScrollEvent& event);
};
#endif // SLIDERENTRY_H
