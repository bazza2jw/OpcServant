#include "virtualkeypadtime.h"
/*!
 * \brief VirtualKeypadTime::VirtualKeypadTime
 * \param parent
 */
VirtualKeypadTime::VirtualKeypadTime(wxWindow* parent)
    : VirtualKeypadTimeBase(parent)
{
    update();
}
/*!
 * \brief VirtualKeypadTime::~VirtualKeypadTime
 */
VirtualKeypadTime::~VirtualKeypadTime()
{
}

/*!
 * \brief VirtualKeypadTime::update
 */
void VirtualKeypadTime::update()
{
   GetHour()->SetValue(wxString::Format("%02d",_d.GetHour()));
   GetMinute()->SetValue(wxString::Format("%02d",_d.GetMinute()));
   GetSecond()->SetValue(wxString::Format("%02d",_d.GetSecond()));
}


/*!
 * \brief VirtualKeypadTime::onHourDown
 * \param event
 */
void VirtualKeypadTime::onHourDown(wxCommandEvent& event)
{
    int h = _d.GetHour();
    if(--h < 0) h = 23;
    _d.SetHour(h);
    update();

}
/*!
 * \brief VirtualKeypadTime::onHourUp
 * \param event
 */
void VirtualKeypadTime::onHourUp(wxCommandEvent& event)
{
    int h = _d.GetHour();
    if(++h > 23) h = 0;
    _d.SetHour(h);
    update();
}
/*!
 * \brief VirtualKeypadTime::onMinDown
 * \param event
 */
void VirtualKeypadTime::onMinDown(wxCommandEvent& event)
{
    int m = _d.GetMinute();
    if(--m < 0 ) m = 59;
    _d.SetMinute(m);
    update();
}
/*!
 * \brief VirtualKeypadTime::onMinUp
 * \param event
 */
void VirtualKeypadTime::onMinUp(wxCommandEvent& event)
{
    int m = _d.GetMinute();
    if(++m > 59 ) m = 0;
    _d.SetMinute(m);
    update();
}
/*!
 * \brief VirtualKeypadTime::onSecDown
 * \param event
 */
void VirtualKeypadTime::onSecDown(wxCommandEvent& event)
{
    int s = _d.GetSecond();
    if(--s < 0) s = 59;
    _d.SetSecond(s);
    update();
}
/*!
 * \brief VirtualKeypadTime::onSecUp
 * \param event
 */
void VirtualKeypadTime::onSecUp(wxCommandEvent& event)
{
    int s = _d.GetSecond();
    if(++s > 59) s = 0;
    _d.SetSecond(s);
    update();
}
