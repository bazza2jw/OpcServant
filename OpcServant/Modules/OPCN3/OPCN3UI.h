//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: OPCN3UI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _MODULES_OPCN3_OPCN3UI_BASE_CLASSES_H
#define _MODULES_OPCN3_OPCN3UI_BASE_CLASSES_H

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
#include <wx/panel.h>
#include <wx/timer.h>
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


class OPCN3ConfigurationDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer3;
    wxFlexGridSizer* flexGridSizer5;
    wxStaticText* m_staticText39;
    wxTextCtrl* m_address;
    wxStaticText* m_staticText7;
    wxSpinCtrl* m_waitInterval;
    wxStaticText* m_staticText11;
    wxSpinCtrl* m_sampleInterval;
    wxCheckBox* m_Enabled;
    wxCheckBox* m_enableTabView;
    wxStdDialogButtonSizer* m_stdBtnSizer54;
    wxButton* m_button56;
    wxButton* m_button58;

protected:
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText39() { return m_staticText39; }
    wxTextCtrl* GetAddress() { return m_address; }
    wxStaticText* GetStaticText7() { return m_staticText7; }
    wxSpinCtrl* GetWaitInterval() { return m_waitInterval; }
    wxStaticText* GetStaticText11() { return m_staticText11; }
    wxSpinCtrl* GetSampleInterval() { return m_sampleInterval; }
    wxCheckBox* GetEnabled() { return m_Enabled; }
    wxCheckBox* GetEnableTabView() { return m_enableTabView; }
    OPCN3ConfigurationDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("OPCN3 Particle Counter"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~OPCN3ConfigurationDialogBase();
};


class OPCN3PanelBase : public wxPanel
{
protected:
    wxGridSizer* gridSizer25;
    wxStaticText* m_staticText27;
    wxStaticText* m_pm1;
    wxStaticText* m_staticText31;
    wxStaticText* m_pm2_5;
    wxStaticText* m_staticText45;
    wxStaticText* m_pm10;
    wxStaticText* m_staticText35;
    wxStaticText* m_status;
    wxTimer* m_timer60;

protected:
    virtual void onTimer(wxTimerEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText27() { return m_staticText27; }
    wxStaticText* GetPm1() { return m_pm1; }
    wxStaticText* GetStaticText31() { return m_staticText31; }
    wxStaticText* GetPm2_5() { return m_pm2_5; }
    wxStaticText* GetStaticText45() { return m_staticText45; }
    wxStaticText* GetPm10() { return m_pm10; }
    wxStaticText* GetStaticText35() { return m_staticText35; }
    wxStaticText* GetStatus() { return m_status; }
    wxTimer* GetTimer60() { return m_timer60; }
    OPCN3PanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxTAB_TRAVERSAL);
    virtual ~OPCN3PanelBase();
};

#endif
