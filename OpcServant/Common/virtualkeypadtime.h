#ifndef VIRTUALKEYPADTIME_H
#define VIRTUALKEYPADTIME_H
#include "VirtualKeypadUI.h"

class VirtualKeypadTime : public VirtualKeypadTimeBase
{
    wxDateTime _d = wxDateTime::Now();
public:
    VirtualKeypadTime(wxWindow* parent);
    virtual ~VirtualKeypadTime();

    void setTime(const wxDateTime &t)
    {
        _d = t;
        update();
    }
    const wxDateTime & getTime()
    {
        return _d;
    }
    void update();
protected:
    virtual void onHourDown(wxCommandEvent& event);
    virtual void onHourUp(wxCommandEvent& event);
    virtual void onMinDown(wxCommandEvent& event);
    virtual void onMinUp(wxCommandEvent& event);
    virtual void onSecDown(wxCommandEvent& event);
    virtual void onSecUp(wxCommandEvent& event);
};
#endif // VIRTUALKEYPADTIME_H
