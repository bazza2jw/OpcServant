#ifndef VKEYBOARDPANEL_H
#define VKEYBOARDPANEL_H

#include "VirtualKeypadUI.h"
#include <wx/textctrl.h>

/*!
 * \brief The VKeyboardPanel class
 */
class VKeyboardPanel : public VKeyboardPanelBase
{
    int _in = 0;
    constexpr static int MaxBufferLen = 100;
    char _inbuffer[MaxBufferLen+1];
    wxTextCtrl * _control = nullptr;
    static VKeyboardPanel * _keyboard;
public:
    VKeyboardPanel(wxWindow* parent);
    virtual ~VKeyboardPanel();
    /*!
     * \brief setControl
     * \param w
     */
    void setControl(wxTextCtrl *w)
    {
         memset(_inbuffer,0,sizeof(_inbuffer));
         _in = 0;
        _control = w;
        if(_control)
        {
            GetTextEntry()->SetValue(_control->GetValue());
            GetTextEntry()->SetValidator(*_control->GetValidator());
        }
        else
        {
            GetTextEntry()->Clear();
        }
    }

    /*!
     * \brief keyboard
     * \return
     */
    static VKeyboardPanel * keyboard()
    {
        if(!_keyboard)
        {
            _keyboard = new VKeyboardPanel(wxApp::GetMainTopWindow());
        }
        return  _keyboard;
    }
    /*!
     * \brief showKeyboard
     */
    static void showKeyboard()
    {
        keyboard()->Show(true);
        keyboard()->Raise();
    }

    /*!
     * \brief hideKeyboard
     */
    static void hideKeyboard()
    {
        keyboard()->Show(false);
    }


protected:
    virtual void onCancel(wxCommandEvent& event);
    virtual void onOK(wxCommandEvent& event);
    virtual void onDelete(wxCommandEvent& event);
    virtual void onChar(wxCommandEvent& event);
};
#endif // VKEYBOARDPANEL_H
