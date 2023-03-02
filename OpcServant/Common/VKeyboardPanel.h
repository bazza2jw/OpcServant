#ifndef VKEYBOARDPANEL_H
#define VKEYBOARDPANEL_H

#include "Common/VirtualKeypadUI.h"
#include <wx/textctrl.h>

/*!
 * \brief The VKeyboardPanel class
 */
class VKeyboardPanel : public VKeyboardPanelBase
{
    int _in = 0;
    constexpr static int MaxBufferLen = 100;
    char _inbuffer[MaxBufferLen+1];
public:
    VKeyboardPanel(wxWindow* parent);
    virtual ~VKeyboardPanel();
    /*!
     * \brief setControl
     * \param w
     */


protected:
    virtual void onDelete(wxCommandEvent& event);
    virtual void onChar(wxCommandEvent& event);
};
#endif // VKEYBOARDPANEL_H
