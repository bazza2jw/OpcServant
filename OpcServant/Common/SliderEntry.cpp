#include "SliderEntry.h"

SliderEntry::SliderEntry(wxWindow* parent)
    : SliderEntryBase(parent)
{
}

SliderEntry::~SliderEntry()
{
}

void SliderEntry::onChanged(wxScrollEvent& event)
{
    double v = double(event.GetPosition()) / double(GetSlider()->GetMax() - GetSlider()->GetMin()) * (_maximum - _minimum) + _minimum;
    SetValue(v);
}
