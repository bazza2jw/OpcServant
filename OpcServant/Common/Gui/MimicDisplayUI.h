//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: MimicDisplayUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _MODULES_MIMICDISPLAY_MIMICDISPLAYUI_BASE_CLASSES_H
#define _MODULES_MIMICDISPLAY_MIMICDISPLAYUI_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/panel.h>
#include "Mimic/Mimic/MimicEditorPanel.h"
#include <wx/toolbar.h>
#include <wx/timer.h>
#include <wx/notebook.h>
#include <wx/imaglist.h>
#include <wx/richtext/richtextctrl.h>
#include "Common/graphwindow.h"
#include <wx/listbox.h>
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


class MimicDisplayConfigurationDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer3;
    wxGridSizer* gridSizer5;
    wxStaticText* m_staticText7;
    wxSpinCtrl* m_updateInterval;
    wxStaticText* m_staticText43;
    wxChoice* m_fileName;
    wxCheckBox* m_enabled;
    wxCheckBox* m_enableTabView;
    wxButton* m_button21;
    wxStdDialogButtonSizer* m_stdBtnSizer15;
    wxButton* m_button17;
    wxButton* m_button19;

protected:
    virtual void onEdit(wxCommandEvent& event) { event.Skip(); }
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText7() { return m_staticText7; }
    wxSpinCtrl* GetUpdateInterval() { return m_updateInterval; }
    wxStaticText* GetStaticText43() { return m_staticText43; }
    wxChoice* GetFileName() { return m_fileName; }
    wxCheckBox* GetEnabled() { return m_enabled; }
    wxCheckBox* GetEnableTabView() { return m_enableTabView; }
    wxButton* GetButton21() { return m_button21; }
    MimicDisplayConfigurationDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Mimic Display Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~MimicDisplayConfigurationDialogBase();
};


class MimicEditorDialogBase : public wxDialog
{
protected:
    wxBoxSizer* canvasSizer;
    MimicEditorPanel* m_panelEditor;
    wxStdDialogButtonSizer* m_stdBtnSizer29;
    wxButton* m_button31;
    wxButton* m_button33;

protected:
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    MimicEditorPanel* GetPanelEditor() { return m_panelEditor; }
    MimicEditorDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Mimic Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX);
    virtual ~MimicEditorDialogBase();
};


class MimicTabDisplayBase : public wxPanel
{
protected:
    wxBoxSizer* canvasSizer;
    wxToolBar* m_toolbar47;
    wxTimer* m_timer;

protected:
    virtual void onHome(wxCommandEvent& event) { event.Skip(); }
    virtual void onBack(wxCommandEvent& event) { event.Skip(); }
    virtual void onTimer(wxTimerEvent& event) { event.Skip(); }

public:
    wxToolBar* GetToolbar47() { return m_toolbar47; }
    wxTimer* GetTimer() { return m_timer; }
    MimicTabDisplayBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(600,400), long style = wxTAB_TRAVERSAL);
    virtual ~MimicTabDisplayBase();
};


class InputViewerBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer55;
    wxNotebook* m_tabs;
    wxPanel* m_panelCurrent;
    wxBoxSizer* boxSizer63;
    wxRichTextCtrl* m_currentStatus;
    wxTimer* m_timer83;

protected:
    virtual void DriveProcess(wxTimerEvent& event) { event.Skip(); }

public:
    wxRichTextCtrl* GetCurrentStatus() { return m_currentStatus; }
    wxPanel* GetPanelCurrent() { return m_panelCurrent; }
    wxNotebook* GetTabs() { return m_tabs; }
    wxTimer* GetTimer83() { return m_timer83; }
    InputViewerBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Input Viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~InputViewerBase();
};


class InputViewerHistoryBase : public wxPanel
{
protected:
    wxBoxSizer* boxSizer75;
    MRL::GraphWindow* m_graph;
    wxListBox* m_history;

protected:

public:
    MRL::GraphWindow* GetGraph() { return m_graph; }
    wxListBox* GetHistory() { return m_history; }
    InputViewerHistoryBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxTAB_TRAVERSAL);
    virtual ~InputViewerHistoryBase();
};

#endif