#include "VKeyboardPanel.h"

VKeyboardPanel * VKeyboardPanel::_keyboard = nullptr;


VKeyboardPanel::VKeyboardPanel(wxWindow* parent)
    : VKeyboardPanelBase(parent)
{
    memset(_inbuffer,0,sizeof(_inbuffer));
    _keyboard = this;
}

VKeyboardPanel::~VKeyboardPanel()
{
}

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

void VKeyboardPanel::onDelete(wxCommandEvent& /*event*/)
{
    if(_in > 0)
    {
        _inbuffer[_in--] = 0;
        GetTextEntry()->SetValue(_inbuffer);
    }
}

void VKeyboardPanel::onCancel(wxCommandEvent& /*event*/)
{
    _control = nullptr;
    GetTextEntry()->Clear();
    Show(false);
}

void VKeyboardPanel::onOK(wxCommandEvent& /*event*/)
{
    _control->SetValue(GetTextEntry()->GetValue());
    _control = nullptr;
    GetTextEntry()->Clear();
    Show(false);
}
