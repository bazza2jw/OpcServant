#include "togglebutton.h"

/*!
 * \brief ToggleButton::ToggleButton
 * \param parent
 * \param id
 * \param label
 * \param pos
 * \param size
 * \param style
 * \param val
 * \param name
 */

#include "TsSpinBase.h"

static ButtonImages *_images = nullptr;
/*!
 * \brief ToggleButton::ToggleButton
 * \param parent
 * \param id
 * \param label
 * \param pos
 * \param size
 * \param style
 * \param val
 * \param name
 */
ToggleButton::ToggleButton(wxWindow *parent, wxWindowID id)
{
        if(!_images) _images = new ButtonImages();
        Create(parent, id, _images->Bitmap("Off"));
        SetValue(false);
        Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(ToggleButton::toggle), NULL, this);
}

/*!
 * \brief ToggleButton::SetValue
 * \param f
 */
void ToggleButton::SetValue(bool f)
{
    wxBitmapToggleButton::SetValue(f);
    if(f)
    {
        SetBitmap(_images->Bitmap("On"));
    }
    else
    {
        SetBitmap(_images->Bitmap("Off"));
    }
}

void ToggleButton::toggle(wxCommandEvent& e)
{
    if(e.GetInt())
    {
        SetBitmap(_images->Bitmap("On"));
    }
    else
    {
        SetBitmap(_images->Bitmap("Off"));
    }
}


