#ifndef PINENTRYDIALOG_H
#define PINENTRYDIALOG_H
#include "CommonGUI.h"
/*!
 * \brief The PinEntryDialog class
 */
class PinEntryDialog : public PinEntryDialogBase
{
public:
    PinEntryDialog(wxWindow* parent);
    virtual ~PinEntryDialog();
protected:
    virtual void on0(wxCommandEvent& event);
    virtual void on1(wxCommandEvent& event);
    virtual void on2(wxCommandEvent& event);
    virtual void on3(wxCommandEvent& event);
    virtual void on4(wxCommandEvent& event);
    virtual void on5(wxCommandEvent& event);
    virtual void on6(wxCommandEvent& event);
    virtual void on7(wxCommandEvent& event);
    virtual void on8(wxCommandEvent& event);
    virtual void on9(wxCommandEvent& event);
    virtual void onDP(wxCommandEvent& event);
    virtual void onOk(wxCommandEvent& event);
    void addChar(char c)
    {
        wxString s = GetText()->GetValue();
        s += c;
        GetText()->SetValue(s);

    }
};
#endif // PINENTRYDIALOG_H
