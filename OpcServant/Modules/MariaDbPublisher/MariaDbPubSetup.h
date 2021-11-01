//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: MariaDbPubSetup.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _MODULES_MARIADBPUBLISHER_MARIADBPUBSETUP_BASE_CLASSES_H
#define _MODULES_MARIADBPUBLISHER_MARIADBPUBSETUP_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#if wxVERSION_NUMBER >= 2900
#include <wx/persist.h>
#include <wx/persist/toplevel.h>
#include <wx/persist/bookctrl.h>
#include <wx/persist/treebook.h>
#endif

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER >= 3100
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif


class MariaDbSetupDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer3;
    wxGridSizer* gridSizer5;
    wxStaticText* m_staticText47;
    wxTextCtrl* m_host;
    wxStaticText* m_staticText13;
    wxTextCtrl* m_database;
    wxStaticText* m_staticText17;
    wxSpinCtrl* m_port;
    wxStaticText* m_staticText21;
    wxTextCtrl* m_username;
    wxStaticText* m_staticText25;
    wxTextCtrl* m_password;
    wxCheckBox* m_enable;
    wxButton* m_button37;
    wxStdDialogButtonSizer* m_stdBtnSizer7;
    wxButton* m_button9;
    wxButton* m_button11;

protected:
    virtual void OnTest(wxCommandEvent& event) { event.Skip(); }
    virtual void OnOK(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText47() { return m_staticText47; }
    wxTextCtrl* GetHost() { return m_host; }
    wxStaticText* GetStaticText13() { return m_staticText13; }
    wxTextCtrl* GetDatabase() { return m_database; }
    wxStaticText* GetStaticText17() { return m_staticText17; }
    wxSpinCtrl* GetPort() { return m_port; }
    wxStaticText* GetStaticText21() { return m_staticText21; }
    wxTextCtrl* GetUsername() { return m_username; }
    wxStaticText* GetStaticText25() { return m_staticText25; }
    wxTextCtrl* GetPassword() { return m_password; }
    wxCheckBox* GetEnable() { return m_enable; }
    wxButton* GetButton37() { return m_button37; }
    MariaDbSetupDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Maria/MySQL Publisher Setup"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~MariaDbSetupDialogBase();
};

#endif
