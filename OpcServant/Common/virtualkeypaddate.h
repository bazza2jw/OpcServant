#ifndef VIRTUALKEYPADDATE_H
#define VIRTUALKEYPADDATE_H
#include "VirtualKeypadUI.h"

class VirtualKeypadDate : public VirtualKeypadDateBase
{
    wxDateTime _d;
public:
    VirtualKeypadDate(wxWindow* parent);
    virtual ~VirtualKeypadDate();

    void update()
    {
        GetYear()->SetValue(wxString::Format("%04d",_d.GetYear()));
        GetMonth()->SetValue(wxDateTime::GetMonthName(_d.GetMonth(),wxDateTime::Name_Abbr));
        GetDay()->SetValue(wxString::Format("%02d",_d.GetDay()));
    }
    /*!
     * \brief setDate
     * \param dt
     */
    void setDate(const wxDateTime &dt)
    {
        _d = dt;
        update();
    }
    /*!
     * \brief getDate
     * \param dt
     */
    const wxDateTime & getDate() // set the date part of the structure
    {
        return _d;
    }

protected:
    virtual void onDayDown(wxCommandEvent& event);
    virtual void onDayUp(wxCommandEvent& event);
    virtual void onMonthDown(wxCommandEvent& event);
    virtual void onMonthUp(wxCommandEvent& event);
    virtual void onYearDown(wxCommandEvent& event);
    virtual void onYearUp(wxCommandEvent& event);
};
#endif // VIRTUALKEYPADDATE_H
