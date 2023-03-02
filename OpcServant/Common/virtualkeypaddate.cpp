#include "virtualkeypaddate.h"
/*!
 * \brief VirtualKeypadDate::VirtualKeypadDate
 * \param parent
 */
VirtualKeypadDate::VirtualKeypadDate(wxWindow* parent)
    : VirtualKeypadDateBase(parent)
{
    _d = wxDateTime::Now();
    update();
}
/*!
 * \brief VirtualKeypadDate::~VirtualKeypadDate
 */
VirtualKeypadDate::~VirtualKeypadDate()
{
}

/*!
 * \brief VirtualKeypadDate::onDayDown
 */
void VirtualKeypadDate::onDayDown(wxCommandEvent& /*event*/)
{
    int d = _d.GetDay();
    d--;
    if(d < 1)
    {
        _d.SetDay(wxDateTime::GetNumberOfDays( _d.GetMonth(), _d.GetYear()));
    }
    else
    {
        _d.SetDay(d);
    }
    update();
}
/*!
 * \brief VirtualKeypadDate::onDayUp
 */
void VirtualKeypadDate::onDayUp(wxCommandEvent& /*event*/)
{
    int d = _d.GetDay();
    d++;
    if(d > wxDateTime::GetNumberOfDays( _d.GetMonth(), _d.GetYear()))
    {
        _d.SetDay(1);
    }
    else
    {
        _d.SetDay(d);
    }
    update();
}
/*!
 * \brief VirtualKeypadDate::onMonthDown
 */
void VirtualKeypadDate::onMonthDown(wxCommandEvent& /*event*/)
{
    wxDateTime::Month m = _d.GetMonth();
    switch(m)
    {
    case wxDateTime::Jan:
        m = wxDateTime::Dec;
        break;
    case wxDateTime::Feb:
        m = wxDateTime::Jan;
        break;
    case wxDateTime::Mar:
        m = wxDateTime::Feb;
        break;
    case wxDateTime::Apr:
        m = wxDateTime::Mar;
        break;
    case wxDateTime::May:
        m = wxDateTime::Apr;
        break;
    case wxDateTime::Jun:
        m = wxDateTime::May;
        break;
    case wxDateTime::Jul:
        m = wxDateTime::Jun;
        break;
    case wxDateTime::Aug:
        m = wxDateTime::Jul;
        break;
    case wxDateTime::Sep:
        m = wxDateTime::Aug;
        break;
    case wxDateTime::Oct:
        m = wxDateTime::Sep;
        break;
    case wxDateTime::Nov:
        m = wxDateTime::Oct;
        break;
    case wxDateTime::Dec:
        m = wxDateTime::Nov;
        break;
    default:
        break;
    }

    int d = _d.GetDay();
    int n = wxDateTime::GetNumberOfDays( m, _d.GetYear());
    if(d > n)
    {
        _d.SetDay(n);
    }
    _d.SetMonth(m);
    update();
}
/*!
 * \brief VirtualKeypadDate::onMonthUp
 */
void VirtualKeypadDate::onMonthUp(wxCommandEvent& /*event*/)
{
    wxDateTime::Month m = _d.GetMonth();
    switch(m)
    {
    case wxDateTime::Jan:
        m = wxDateTime::Feb;
        break;
    case wxDateTime::Feb:
        m = wxDateTime::Mar;
        break;
    case wxDateTime::Mar:
        m = wxDateTime::Apr;
        break;
    case wxDateTime::Apr:
        m = wxDateTime::May;
        break;
    case wxDateTime::May:
        m = wxDateTime::Jun;
        break;
    case wxDateTime::Jun:
        m = wxDateTime::Jul;
        break;
    case wxDateTime::Jul:
        m = wxDateTime::Aug;
        break;
    case wxDateTime::Aug:
        m = wxDateTime::Sep;
        break;
    case wxDateTime::Sep:
        m = wxDateTime::Oct;
        break;
    case wxDateTime::Oct:
        m = wxDateTime::Nov;
        break;
    case wxDateTime::Nov:
        m = wxDateTime::Dec;
        break;
    case wxDateTime::Dec:
        m = wxDateTime::Jan;
        break;
    default:
        break;
    }
    // clip the days
    int d = _d.GetDay();
    int n = wxDateTime::GetNumberOfDays( m, _d.GetYear());
    if(d > n)
    {
        _d.SetDay(n);
    }
    _d.SetMonth(m);
    update();
}
/*!
 * \brief VirtualKeypadDate::onYearDown
 */
void VirtualKeypadDate::onYearDown(wxCommandEvent& /*event*/)
{
    _d.SetYear(_d.GetYear() - 1);
    if(_d.GetMonth() == wxDateTime::Feb)
    {
        int d = _d.GetDay();
        int n = wxDateTime::GetNumberOfDays( _d.GetMonth(), _d.GetYear());
        if(d > n)
        {
            _d.SetDay(n);
        }
    }
    update();
}
/*!
 * \brief VirtualKeypadDate::onYearUp
 */
void VirtualKeypadDate::onYearUp(wxCommandEvent& /*event*/)
{
    _d.SetYear(_d.GetYear() + 1);
    update();
}
