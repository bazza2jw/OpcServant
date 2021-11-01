#include "roundedbutton.h"
/*!
 * \brief RoundedButton::RoundedButton
 */
RoundedButton::RoundedButton(wxWindow *parent, wxWindowID id, const wxPoint &pos,
                             const wxSize &size, long style, const wxValidator &validator,
              const wxString &name) :
    wxControl(parent,id, pos, size,  style, validator, name) ,
  _buttonColour(*wxBLUE),
  _textColour(*wxWHITE)

{
        this->Connect(wxEVT_PAINT, wxPaintEventHandler(RoundedButton::OnPaint), NULL, this);
        this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(RoundedButton::OnLeftDown), NULL, this);
        this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(RoundedButton::OnLeftUp), NULL, this);
        //SetBackgroundStyle(wxBG_STYLE_TRANSPARENT); // transparent background

}

/*!
 * \brief RoundedButton::RoundedButton
 * \param parent
 * \param id
 * \param label
 * \param pos
 * \param size
 * \param style
 * \param validator
 * \param name
 */
RoundedButton::RoundedButton(wxWindow *parent, wxWindowID id, const wxString &label,
              const wxPoint &pos, const wxSize &size,
              long style, const wxValidator &validator,
              const wxString &name) :
              wxControl(parent,id, pos, size,  style, validator, name),
              _buttonColour(*wxBLUE),
              _textColour(*wxWHITE)
{
    _label = label;
    this->Connect(wxEVT_PAINT, wxPaintEventHandler(RoundedButton::OnPaint), NULL, this);
    this->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(RoundedButton::OnLeftDown), NULL, this);
    this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(RoundedButton::OnLeftUp), NULL, this);
}

/*!
 * \brief RoundedButton::Create
 * \param parent
 * \param id
 * \param pos
 * \param size
 * \param style
 * \param validator
 * \param name
 * \return
 */
bool RoundedButton::Create (wxWindow *parent, wxWindowID id, const wxPoint &pos,
                            const wxSize &size, long style, const wxValidator &validator, const wxString &name)
{
    return wxControl::Create(parent,id,pos,size,style,validator,name);
}


/*!
 * \brief RoundedButton::OnPaint
 * \param event
 */
void RoundedButton::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    Draw(dc);
}
/*!
 * \brief RoundedButton::OnLeftDown
 * \param event
 */
void RoundedButton::OnLeftDown(wxMouseEvent& event)
{
    _pressed = true;
    wxClientDC dc(this);
    Draw(dc);
}
/*!
 * \brief RoundedButton::OnLeftUp wxPen tp(_textColour);
 * \param event
 */
void RoundedButton::OnLeftUp(wxMouseEvent& event)
{
    if(_pressed)
    {
        // fire a clicked event
        wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED,GetId());
        ProcessEvent(event);
    }
    _pressed = false;
    wxClientDC dc(this);
    Draw(dc);
}
/*!
 * \brief RoundedButton::Draw
 * \param dc
 */
void RoundedButton::Draw(wxDC &dc)
{
    // Draw the rectangle with rounded ends
    wxRect cr = GetClientRect();
    wxColour c = _buttonColour;
    if(_pressed)
    {
        // darken colour if pressed
        unsigned char r = c.Red();
        unsigned char g = c.Green();
        unsigned char b = c.Blue();
        wxColour::ChangeLightness (&r, &g, &b, 70);
        wxColour cp(r,g,b);
        wxBrush br(cp);
        wxPen p(c);
        dc.SetPen(p);
        dc.SetBrush(br);
        dc.DrawRoundedRectangle(cr,-0.2);
    }
    else
    {
        wxBrush br(c);
        dc.SetBrush(br);
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRoundedRectangle(cr,-0.2);
    }
    //
    // Now draw text
    //

    if(!_label.IsEmpty())
    {
        dc.SetTextForeground(_textColour);
        dc.DrawLabel(_label,cr,wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL);
    }
}



