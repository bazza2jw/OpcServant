#ifndef VTEXTENTRYDIALOG_H
#define VTEXTENTRYDIALOG_H
#include "VirtualKeypadUI.h"

class VTextEntryDialog : public VTextEntryDialogBase
{
    int _in = 0;
    constexpr static int MaxBufferLen = 100;
    char _inbuffer[MaxBufferLen+1];

public:
    VTextEntryDialog(wxWindow* parent);
    virtual ~VTextEntryDialog();
    void setText(const wxString &s);
    wxString getText();
protected:
    virtual void onCancel(wxCommandEvent& event);
    virtual void onDelete(wxCommandEvent& event);
    virtual void onChar(wxCommandEvent& event);
    virtual void onOK(wxCommandEvent& event);
};
#endif // VTEXTENTRYDIALOG_H
