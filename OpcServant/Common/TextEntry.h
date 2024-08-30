#ifndef TEXTENTRY_H
#define TEXTENTRY_H
#include "VirtualKeypadUI.h"
#include <wx/spinctrl.h>
//
// Fields that have virtual key pads tailored to the input type. Independent of the
// These are intended for instrument UIs that have a (small) touch screen. "Chunky Monkey" for ease of use.
wxDECLARE_EVENT(wxEVT_FIELD_UPDATED, wxCommandEvent);
//
class TextEntry : public wxTextCtrl
{
public:
    TextEntry(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~TextEntry();
    //
    //
    virtual bool openKeyPad();
protected:
    virtual void onClick(wxMouseEvent& ) // text field has been clicked open the entry dialog
    {
        SetFocus();
        if(openKeyPad())
        {
            notify();
        }
        SetFocus();
    }
    void notify()
    {
        wxCommandEvent eventCustom(wxEVT_FIELD_UPDATED);
        wxPostEvent(this, eventCustom);
    }
};

/*!
 * \brief The DateEntry class
 */
class DateEntry : public TextEntry
{
    wxDateTime _date;
public:
    DateEntry(wxWindow* parent, wxWindowID id = wxID_ANY) : TextEntry(parent,id)
    {
        _date = wxDateTime::Now();
    }
    virtual void SetDate(const wxDateTime &dt)
    {
        _date = dt;
        SetValue(wxString::Format("%02d %3s %04d",_date.GetDay(),wxDateTime::GetMonthName(_date.GetMonth(),wxDateTime::Name_Abbr),_date.GetYear()));
    }

    wxDateTime & GetDate() { return _date;}

    virtual bool openKeyPad();
};


/*!
 * \brief The TimeEntry class
 * Edit the time part of a date-time
 */
class TimeEntry : public TextEntry
{
    wxDateTime _time;
public:
    TimeEntry(wxWindow* parent, wxWindowID id = wxID_ANY) : TextEntry(parent,id)
    {
        _time = wxDateTime::Now();
    }
    virtual void SetTime(const wxDateTime &dt)
    {
        _time = dt;
        SetValue(wxString::Format("%02d:%02d:%02d",_time.GetHour(),_time.GetMinute(),_time.GetSecond()));
    }

    wxTimeSpan GetSpan()
    {
        wxTimeSpan t(_time.GetHour(),_time.GetMinute(),_time.GetSecond());
        return t;
    }
    wxDateTime & GetTime() { return _time;}
    virtual bool openKeyPad();
};
/*!
 * \brief The IntEntry class
 */
class IntEntry : public wxTextCtrl
{
    int _minimum = 0;
    int _maximum = 100;
    int _value = 0;
public:
    IntEntry(wxWindow* parent, wxWindowID id = wxID_ANY) : wxTextCtrl(parent,id)
    {
        Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(IntEntry::onClick), NULL, this);
    }
    ~IntEntry()
    {
        Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(IntEntry::onClick), NULL, this);
    }
    virtual void onClick(wxMouseEvent& ) // text field has been clicked open the entry dialog
    {
        SetFocus();
        openKeyPad();
        SetFocus();
    }
    void SetMinimum(int v) { _minimum = v;}
    void SetMaximum(int v) {_maximum = v;}
    void SetValue(int v) {
        wxString s = wxString::Format("%d",v);
        wxTextCtrl::SetValue(s);
        _value = v;
    }
    int Value() const  { return _value;}
    virtual bool openKeyPad();
};

/*!
 * \brief The DoubleEntry class
 */
class DoubleEntry : public wxTextCtrl
{
   double _minimum = 0.0;
   double _maximum = 1.0;
   wxString _format = "%8.3f";
   double _value = 0.0;
public:
    DoubleEntry(wxWindow* parent, wxWindowID id = wxID_ANY) : wxTextCtrl(parent,id)
    {
        Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleEntry::onClick), NULL, this);
    }
    ~DoubleEntry()
    {
        Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleEntry::onClick), NULL, this);
    }
    virtual void onClick(wxMouseEvent & ) // text field has been clicked open the entry dialog
    {
        SetFocus();
        openKeyPad();
        SetFocus();
    }
    void SetMinimum(double v) { _minimum = v;}
    void SetMaximum(double v) {_maximum = v;}
    void SetFormat(const wxString &v) { _format = v;}
    virtual void SetValue(double v) {
        wxString s = wxString::Format(_format,v);
        wxTextCtrl::SetValue(s);
        _value = v;
    }
    int Value() const { return _value;}
    virtual bool openKeyPad();
};




#endif // TEXTENTRY_H
