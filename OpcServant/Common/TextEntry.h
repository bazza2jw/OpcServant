#ifndef TEXTENTRY_H
#define TEXTENTRY_H
#include "VirtualKeypadUI.h"
#include <wx/spinctrl.h>
//
// Fields that have virtual key pads tailored to the imput type. Independent of the
// These are intended for instrument UIs that have a (small) touch screen. "Chunky Monkey" for ease of use.
//
class TextEntry : public wxTextCtrl
{
public:
    TextEntry(wxWindow* parent, wxWindowID id = wxID_ANY);
    virtual ~TextEntry();
    //
    //
    virtual void openKeyPad();
protected:
    virtual void onClick(wxMouseEvent& ) // text field has been clicked open the entry dialog
    {
        SetFocus();
        openKeyPad();
        SetFocus();
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

    virtual void openKeyPad();
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

    wxDateTime & GetTime() { return _time;}
    virtual void openKeyPad();
};
/*!
 * \brief The IntEntry class
 */
class IntEntry : public wxSpinCtrl
{
public:
    IntEntry(wxWindow* parent, wxWindowID id = wxID_ANY) : wxSpinCtrl(parent,id)
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

    virtual void openKeyPad();
};

/*!
 * \brief The DoubleEntry class
 */
class DoubleEntry : public wxSpinCtrlDouble
{
public:
    DoubleEntry(wxWindow* parent, wxWindowID id = wxID_ANY) : wxSpinCtrlDouble(parent,id)
    {
        Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleEntry::onClick), NULL, this);
    }
    ~DoubleEntry()
    {
        Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DoubleEntry::onClick), NULL, this);
    }
    virtual void onClick(wxMouseEvent& ) // text field has been clicked open the entry dialog
    {
        SetFocus();
        openKeyPad();
        SetFocus();
    }
    virtual void openKeyPad();
};




#endif // TEXTENTRY_H
