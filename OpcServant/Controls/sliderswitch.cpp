#include "sliderswitch.h"

/*!
 * \brief SliderSwitch::SliderSwitch
 */
SliderSwitch::SliderSwitch(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                             const wxSize &size, long style, const wxValidator &validator,
              const wxString &name) :
    wxControl(parent,id, pos, size,  style, validator, name) ,
  _onColour(*wxGREEN),
  _offColour(*wxRED)
{
        this->Connect(wxEVT_PAINT, wxPaintEventHandler(SliderSwitch::OnPaint), NULL, this);
        this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(SliderSwitch::OnLeftDown), NULL, this);
        this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(SliderSwitch::OnLeftUp), NULL, this);
        //
        _darkOnColour = _onColour;
        _darkOffColour = _offColour;
        _darkOffColour.MakeDisabled(70);
        _darkOnColour.MakeDisabled(70);
        //
}

/*!
 * \brief SliderSwitch::SliderSwitch
 * \param parent
 * \param id
 * \param label
 * \param pos
 * \param size
 * \param style
 * \param validator
 * \param name
 */
SliderSwitch::SliderSwitch(wxWindow *parent, wxWindowID id, const wxString &label,
              const wxPoint &pos, const wxSize &size,
              long style, const wxValidator &validator,
              const wxString &name) :
              wxControl(parent,id, pos, size,  style, validator, name),
              _onColour(*wxGREEN),
              _offColour(*wxRED)
{
    _label = label;
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(SliderSwitch::OnPaint), NULL, this);
    this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(SliderSwitch::OnLeftDown), NULL, this);
    this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(SliderSwitch::OnLeftUp), NULL, this);
    _darkOnColour = _onColour;
    _darkOffColour = _offColour;
    _darkOffColour.MakeDisabled(70);
    _darkOnColour.MakeDisabled(70);
}

/*!
 * \brief SliderSwitch::Create
 * \param parent
 * \param id
 * \param pos
 * \param size
 * \param style
 * \param validator
 * \param name
 * \return
 */
bool SliderSwitch::Create (wxWindow *parent, wxWindowID id, const wxPoint &pos,
                            const wxSize &size, long style, const wxValidator &validator, const wxString &name)
{
    wxControl::Create(parent,id,pos,size,style,validator,name);
}


/*!
 * \brief SliderSwitch::OnPaint
 * \param event
 */
void SliderSwitch::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    Draw(dc);
}
/*!
 * \brief SliderSwitch::OnLeftDown
 * \param event
 */
void SliderSwitch::OnLeftDown(wxMouseEvent& event)
{
    _checked = !_checked;
    wxClientDC dc(this);
    Draw(dc);
}
/*!
 * \brief SliderSwitch::OnLeftUp wxPen tp(_textColour);
 * \param event
 */
void SliderSwitch::OnLeftUp(wxMouseEvent& event)
{
    if(_checked)
    {
        // fire a clicked event
        wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,GetId());
        ProcessEvent(event);
    }

    wxClientDC dc(this);
    Draw(dc);
}
/*!
 * \brief SliderSwitch::Draw
 * \param dc
 */
void SliderSwitch::Draw(wxDC &dc)
{
    // Draw the rectangle with rounded ends
    wxRect cr = GetClientRect();
    //
    wxRect l(0,0,cr.GetWidth()/2,cr.GetHeight()) ;
    wxRect r(cr.GetWidth()/2,0,cr.GetWidth()/2,cr.GetHeight()) ;
    //

    //
    if(_checked)
    {
        wxBrush lb(_darkOffColour);
        wxBrush rb(_onColour);
        dc.SetBrush(lb);
        dc.DrawRectangle(l);
        dc.SetBrush(rb);
        dc.DrawRectangle(r);
    }
    else
    {
        wxBrush lb(_offColour);
        wxBrush rb(_darkOnColour);
        dc.SetBrush(lb);
        dc.DrawRectangle(l);
        dc.SetBrush(rb);
        dc.DrawRectangle(r);
    }
    //
    // Now draw text
    //

    if(!_label.IsEmpty())
    {
        dc.DrawLabel(_label,cr,wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL);
    }
}



