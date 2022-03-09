//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: NodeEditor.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _NODEFLOW_NODEFLOW_NODEEDITOR_BASE_CLASSES_H
#define _NODEFLOW_NODEFLOW_NODEEDITOR_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include "wx/mdi.h"
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/toolbar.h>
#include <wx/tglbtn.h>
#include <wx/spinctrl.h>
#include <wx/timer.h>
#include <wx/dialog.h>
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


class NodeEditorMainframeBase : public wxMDIParentFrame
{
protected:
    wxMenuBar* m_menuBar;
    wxMenu* m_menuFile;
    wxMenuItem* m_menuItem153;
    wxMenuItem* m_menuItemExit;

protected:
    virtual void onNew(wxCommandEvent& event) { event.Skip(); }
    virtual void onExit(wxCommandEvent& event) { event.Skip(); }

public:
    wxMenuBar* GetMenuBar() { return m_menuBar; }
    NodeEditorMainframeBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Node Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_FRAME_STYLE|wxCAPTION|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxCLOSE_BOX);
    virtual ~NodeEditorMainframeBase();
};


class NodeEditorPanelBase : public wxPanel
{
protected:
    wxBoxSizer* boxSizer67;
    wxSplitterWindow* m_splitter;
    wxPanel* m_splitterLeft;
    wxBoxSizer* boxSizer85;
    wxTreeCtrl* m_nodeTypes;
    wxListBox* m_listTrace;
    wxButton* m_button237;
    wxPanel* m_splitterRight;
    wxBoxSizer* canvasSizer;
    wxToolBar* m_toolbar173;
    wxToggleButton* m_toggleEdit;
    wxButton* m_buttonStart;
    wxButton* m_buttonStep;
    wxButton* m_buttonTrigger;
    wxButton* m_buttonRun;
    wxSpinCtrl* m_timerInterval;
    wxButton* m_buttonInject;
    wxTimer* m_timerStep;

protected:
    virtual void onClear(wxCommandEvent& event) { event.Skip(); }
    virtual void onOpen(wxCommandEvent& event) { event.Skip(); }
    virtual void onSave(wxCommandEvent& event) { event.Skip(); }
    virtual void onSaveAs(wxCommandEvent& event) { event.Skip(); }
    virtual void onEditRun(wxCommandEvent& event) { event.Skip(); }
    virtual void onStartSet(wxCommandEvent& event) { event.Skip(); }
    virtual void onStep(wxCommandEvent& event) { event.Skip(); }
    virtual void onTrigger(wxCommandEvent& event) { event.Skip(); }
    virtual void onRun(wxCommandEvent& event) { event.Skip(); }
    virtual void onInject(wxCommandEvent& event) { event.Skip(); }
    virtual void onTimer(wxTimerEvent& event) { event.Skip(); }

public:
    wxTreeCtrl* GetNodeTypes() { return m_nodeTypes; }
    wxListBox* GetListTrace() { return m_listTrace; }
    wxButton* GetButton237() { return m_button237; }
    wxPanel* GetSplitterLeft() { return m_splitterLeft; }
    wxPanel* GetSplitterRight() { return m_splitterRight; }
    wxSplitterWindow* GetSplitter() { return m_splitter; }
    wxToggleButton* GetToggleEdit() { return m_toggleEdit; }
    wxButton* GetButtonStart() { return m_buttonStart; }
    wxButton* GetButtonStep() { return m_buttonStep; }
    wxButton* GetButtonTrigger() { return m_buttonTrigger; }
    wxButton* GetButtonRun() { return m_buttonRun; }
    wxSpinCtrl* GetTimerInterval() { return m_timerInterval; }
    wxButton* GetButtonInject() { return m_buttonInject; }
    wxToolBar* GetToolbar173() { return m_toolbar173; }
    wxTimer* GetTimerStep() { return m_timerStep; }
    NodeEditorPanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxTAB_TRAVERSAL);
    virtual ~NodeEditorPanelBase();
};


class NodeFlowEditorDialogBase : public wxDialog
{
protected:
    wxBoxSizer* PanelSizer;
    wxToolBar* m_toolbar195;
    wxStdDialogButtonSizer* m_stdBtnSizer189;
    wxButton* m_button191;
    wxButton* m_button193;

protected:
    virtual void onOpen(wxCommandEvent& event) { event.Skip(); }
    virtual void onSave(wxCommandEvent& event) { event.Skip(); }
    virtual void onSaveAs(wxCommandEvent& event) { event.Skip(); }
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxToolBar* GetToolbar195() { return m_toolbar195; }
    NodeFlowEditorDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Node Flow Set"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX);
    virtual ~NodeFlowEditorDialogBase();
};

#endif