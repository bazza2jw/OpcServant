#include "mimiccontrols.h"

#include <wx/image.h>
#include <math.h>
#include <wx/log.h>
#include "mimictype.h"
#include "mimiccontrols.h"


DigitalLEDBright * MIMIC::LedDisplayObject::_ledImageList = nullptr;
DigitalNixie *  MIMIC::LedDisplayObject::_nixieImageList = nullptr;
std::vector<wxBitmap> MIMIC::LedDisplayObject::_ledBitmap;
std::vector<wxBitmap> MIMIC::LedDisplayObject::_nixieBitmap;

void loadControlObjects()
{
 (void) new MIMIC::MimicObjectType<MIMIC::LedDisplayObject>("LedDisplay");
 (void) new MIMIC::MimicObjectType<MIMIC::TrafficLightObject>("TrafficLight");
 (void) new MIMIC::MimicObjectType<MIMIC::LedObject>("Led");
}


/*!
 * \brief MIMIC::TrafficLightObject::draw
 * \param dc
 */
void MIMIC::TrafficLightObject::draw(wxDC &dc)
{
    IconObject::draw(dc);
    wxPoint cp(rect().GetLeftBottom().x, rect().GetLeftBottom().y+ lightSize +1); // circle position and size
    //
    // draw green
    wxDCPenChanger pc(dc,*wxThePenList->FindOrCreatePen(textColour(), 1, wxPENSTYLE_SOLID));
    wxDCBrushChanger bc(dc,*wxTheBrushList->FindOrCreateBrush(colour(),wxBRUSHSTYLE_SOLID));
    //
    // draw each of the lamps on
    if(_state == Ok)
    {
        dc.SetBrush(*wxGREEN_BRUSH);
    }
    else
    {
        wxColour c = *wxGREEN;
        c.MakeDisabled(128);
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(c,wxBRUSHSTYLE_SOLID));
    }
    dc.DrawCircle(cp,lightSize);
    cp.x += 2*(lightSize + 1);
    //
    if(_state == Alert)
    {
        dc.SetBrush(*wxYELLOW_BRUSH);
    }
    else
    {
        wxColour c = *wxYELLOW;
        c.MakeDisabled(128);
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(c,wxBRUSHSTYLE_SOLID));
    }
    dc.DrawCircle(cp,lightSize);
    cp.x += 2*(lightSize + 1);
    //
    if(_state == Action)
    {
        dc.SetBrush(*wxRED);
    }
    else
    {
        wxColour c = *wxRED;
        c.MakeDisabled(128);
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(c,wxBRUSHSTYLE_SOLID));
    }
    dc.DrawCircle(cp,lightSize);
    cp.x += 2*(lightSize + 1);
    if(_ackOn)
    {
        dc.SetBrush(*wxBLUE_BRUSH);
    }
    else
    {
        wxColour c = *wxBLUE;
        c.MakeDisabled(128);
        dc.SetBrush(*wxTheBrushList->FindOrCreateBrush(c,wxBRUSHSTYLE_SOLID));
    }
    dc.DrawCircle(cp,lightSize);
    //
    //
    switch(_state)
    {
    case Ok:
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.SetPen(*wxWHITE_PEN);
        break;
    case Alert:
        dc.SetBrush(*wxYELLOW_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        break;
    case Action:
        dc.SetBrush(*wxRED_BRUSH);
        dc.SetPen(*wxWHITE_PEN);
        break;
    default:
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        break;
    }
    wxDCFontChanger fc(dc,*wxSMALL_FONT); // set the font - TBD make this editable

    wxPoint tp = rect().GetLeftTop();
    tp.y -= wxSMALL_FONT->GetPixelSize().GetHeight();
    dc.SetTextForeground(textColour());
    dc.SetTextBackground(colour());
    dc.DrawText(_text,tp);
}


/*!
 * \brief MIMIC::LedDisplayObject::LedDisplayObject
 * \param i
 * \param t
 */
MIMIC::LedDisplayObject::LedDisplayObject(unsigned i , unsigned t ) : RectObject(i,t)
{
    if(!_ledImageList)
    {
        _ledImageList = new DigitalLEDBright();
        _ledBitmap.resize(13);
        // get to useful list
        _ledBitmap[0] = _ledImageList->Bitmap("m_LED0");
        _ledBitmap[1] = _ledImageList->Bitmap("m_LED1");
        _ledBitmap[2] = _ledImageList->Bitmap("m_LED2");
        _ledBitmap[3] = _ledImageList->Bitmap("m_LED3");
        _ledBitmap[4] = _ledImageList->Bitmap("m_LED4");
        _ledBitmap[5] = _ledImageList->Bitmap("m_LED5");
        _ledBitmap[6] = _ledImageList->Bitmap("m_LED6");
        _ledBitmap[7] = _ledImageList->Bitmap("m_LED7");
        _ledBitmap[8] = _ledImageList->Bitmap("m_LED8");
        _ledBitmap[9] = _ledImageList->Bitmap("m_LED9");
        _ledBitmap[10] = _ledImageList->Bitmap("m_LEDSP");//sp
        _ledBitmap[11] = _ledImageList->Bitmap("m_LEDMinus");// -
        _ledBitmap[12] = _ledImageList->Bitmap("m_LEDDP");// -

    }

    if(!_nixieImageList)
    {
        _nixieImageList = new DigitalNixie();
        _nixieBitmap.resize(13);
        _nixieBitmap[0] = _nixieImageList->Bitmap("m_Nixie0");
        _nixieBitmap[1] = _nixieImageList->Bitmap("m_Nixie1");
        _nixieBitmap[2] = _nixieImageList->Bitmap("m_Nixie2");
        _nixieBitmap[3] = _nixieImageList->Bitmap("m_Nixie3");
        _nixieBitmap[4] = _nixieImageList->Bitmap("m_Nixie4");
        _nixieBitmap[5] = _nixieImageList->Bitmap("m_Nixie5");
        _nixieBitmap[6] = _nixieImageList->Bitmap("m_Nixie6");
        _nixieBitmap[7] = _nixieImageList->Bitmap("m_Nixie7");
        _nixieBitmap[8] = _nixieImageList->Bitmap("m_Nixie8");
        _nixieBitmap[9] = _nixieImageList->Bitmap("m_Nixie9");
        _nixieBitmap[10] = _nixieImageList->Bitmap("m_NixieSP");//sp
        _nixieBitmap[11] = _nixieImageList->Bitmap("m_NixieMinus");// -
        _nixieBitmap[12] = _nixieImageList->Bitmap("m_NixieDP");// -

    }

}
