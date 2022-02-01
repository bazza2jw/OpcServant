//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: BUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _SERVANT_GUI_BUI_BASE_CLASSES_H
#define _SERVANT_GUI_BUI_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/imaglist.h>
#include <wx/splitter.h>
#include <wx/dataview.h>
#include <wx/button.h>
#include <wx/html/htmlwin.h>
#include <wx/listbox.h>
#include <wx/timer.h>
#include <wx/bitmap.h>
#include <map>
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


class MainframeBase : public wxFrame
{
protected:
    wxBoxSizer* boxSizer15;
    wxNotebook* m_notebook;
    wxPanel* m_navigationTab;
    wxBoxSizer* boxSizer25;
    wxSplitterWindow* m_splitter111;
    wxPanel* m_splitterPage115;
    wxBoxSizer* boxSizer121;
    wxDataViewTreeCtrl* m_Navigation;
    wxGridSizer* gridSizer175;
    wxButton* m_unlockNavigator;
    wxButton* m_buttonSysProps;
    wxButton* m_aliasConfigure;
    wxButton* m_visualCalc;
    wxPanel* m_diagnosticPage;
    wxBoxSizer* boxSizer125;
    wxNotebook* m_notebook129;
    wxPanel* m_panelConfiguration;
    wxBoxSizer* boxSizer137;
    wxHtmlWindow* m_Configuration;
    wxPanel* m_panelMeasure;
    wxBoxSizer* boxSizer141;
    wxHtmlWindow* m_Measure;
    wxPanel* m_panelPublish;
    wxBoxSizer* boxSizer145;
    wxHtmlWindow* m_Publish;
    wxPanel* m_messagePanel;
    wxBoxSizer* boxSizer31;
    wxListBox* m_messages;
    wxGridSizer* gridSizer35;
    wxButton* m_buttonClearMessages;
    wxTimer* m_timer77;
    wxTimer* m_timerPeriodic;

protected:
    virtual void onClose(wxCloseEvent& event) { event.Skip(); }
    virtual void OnContextMenu(wxDataViewEvent& event) { event.Skip(); }
    virtual void OnSelChanged(wxDataViewEvent& event) { event.Skip(); }
    virtual void onUnlockNavigator(wxCommandEvent& event) { event.Skip(); }
    virtual void onProperties(wxCommandEvent& event) { event.Skip(); }
    virtual void OnAliasConfigure(wxCommandEvent& event) { event.Skip(); }
    virtual void onVisualCalc(wxCommandEvent& event) { event.Skip(); }
    virtual void OnClearMessages(wxCommandEvent& event) { event.Skip(); }
    virtual void OnStartupTimer(wxTimerEvent& event) { event.Skip(); }
    virtual void onPeriodicTimer(wxTimerEvent& event) { event.Skip(); }

public:
    wxDataViewTreeCtrl* GetNavigation() { return m_Navigation; }
    wxButton* GetUnlockNavigator() { return m_unlockNavigator; }
    wxButton* GetButtonSysProps() { return m_buttonSysProps; }
    wxButton* GetAliasConfigure() { return m_aliasConfigure; }
    wxButton* GetVisualCalc() { return m_visualCalc; }
    wxPanel* GetSplitterPage115() { return m_splitterPage115; }
    wxHtmlWindow* GetConfiguration() { return m_Configuration; }
    wxPanel* GetPanelConfiguration() { return m_panelConfiguration; }
    wxHtmlWindow* GetMeasure() { return m_Measure; }
    wxPanel* GetPanelMeasure() { return m_panelMeasure; }
    wxHtmlWindow* GetPublish() { return m_Publish; }
    wxPanel* GetPanelPublish() { return m_panelPublish; }
    wxNotebook* GetNotebook129() { return m_notebook129; }
    wxPanel* GetDiagnosticPage() { return m_diagnosticPage; }
    wxSplitterWindow* GetSplitter111() { return m_splitter111; }
    wxPanel* GetNavigationTab() { return m_navigationTab; }
    wxListBox* GetMessages() { return m_messages; }
    wxButton* GetButtonClearMessages() { return m_buttonClearMessages; }
    wxPanel* GetMessagePanel() { return m_messagePanel; }
    wxNotebook* GetNotebook() { return m_notebook; }
    wxTimer* GetTimer77() { return m_timer77; }
    wxTimer* GetTimerPeriodic() { return m_timerPeriodic; }
    MainframeBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Navigator"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(700,480), long style = wxRESIZE_BORDER|wxBORDER_NONE);
    virtual ~MainframeBase();
};


class ImageList : public wxImageList
{
protected:
    // Maintain a map of all bitmaps representd by their name
    std::map<wxString, wxBitmap> m_bitmaps;
    // The requested image resolution (can be one of @2x, @1.5x, @1.25x or an empty string (the default)
    wxString m_resolution;
    int m_imagesWidth;
    int m_imagesHeight;


protected:

public:
    ImageList();
    const wxBitmap& Bitmap(const wxString &name) const {
        if ( !m_bitmaps.count(name + m_resolution) )
            return wxNullBitmap;
        return m_bitmaps.find(name + m_resolution)->second;
    }

    void SetBitmapResolution(const wxString &res = wxEmptyString) {
        m_resolution = res;
    }

    virtual ~ImageList();
};

#endif
