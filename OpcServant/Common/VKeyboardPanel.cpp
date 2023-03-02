#include "VKeyboardPanel.h"



/*!
 * \brief VKeyboardPanel::VKeyboardPanel
 * \param parent
 */
VKeyboardPanel::VKeyboardPanel(wxWindow* parent)
    : VKeyboardPanelBase(parent)
{
    memset(_inbuffer,0,sizeof(_inbuffer));
}
/*!
 * \brief VKeyboardPanel::~VKeyboardPanel
 */
VKeyboardPanel::~VKeyboardPanel()
{
}
/*!
 * \brief VKeyboardPanel::onChar
 * \param event
 */
void VKeyboardPanel::onChar(wxCommandEvent& event)
{
    static const char * keys = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ.-_/ ";
    int i = event.GetId() - 20000;
    char c = keys[i];
    if(_in < MaxBufferLen)
    {
        _inbuffer[_in++] = c;
        GetTextEntry()->SetValue(_inbuffer);
    }
}
/*!
 * \brief VKeyboardPanel::onDelete
 */
void VKeyboardPanel::onDelete(wxCommandEvent& /*event*/)
{
    if(_in > 0)
    {
        _inbuffer[_in--] = 0;
        GetTextEntry()->SetValue(_inbuffer);
    }
}

