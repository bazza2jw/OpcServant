//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: MimicUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _MIMIC_MIMIC_MIMICUI_BASE_CLASSES_H
#define _MIMIC_MIMIC_MIMICUI_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/panel.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include "wx/mdi.h"
#include <wx/menu.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>
#include <map>
#include <wx/icon.h>
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


class MimicEditorPanelBase : public wxPanel
{
protected:
    wxBoxSizer* canvasSizerTop;
    wxToolBar* m_toolbar11;
    wxSplitterWindow* m_splitter;
    wxPanel* m_splitterPage45;
    wxBoxSizer* boxSizer51;
    wxTreeCtrl* m_controlList;
    wxPanel* m_splitterPageCanvas;
    wxBoxSizer* canvasSizer;

protected:
    virtual void onNew(wxCommandEvent& event) { event.Skip(); }
    virtual void onOpen(wxCommandEvent& event) { event.Skip(); }
    virtual void onSave(wxCommandEvent& event) { event.Skip(); }
    virtual void onSaveAs(wxCommandEvent& event) { event.Skip(); }
    virtual void onAddBackground(wxCommandEvent& event) { event.Skip(); }

public:
    wxToolBar* GetToolbar11() { return m_toolbar11; }
    wxTreeCtrl* GetControlList() { return m_controlList; }
    wxPanel* GetSplitterPage45() { return m_splitterPage45; }
    wxPanel* GetSplitterPageCanvas() { return m_splitterPageCanvas; }
    wxSplitterWindow* GetSplitter() { return m_splitter; }
    MimicEditorPanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxTAB_TRAVERSAL);
    virtual ~MimicEditorPanelBase();
};


class MimicEditorFrameBase : public wxMDIParentFrame
{
protected:
    wxMenuBar* m_menuBar;
    wxMenu* m_menu25;
    wxMenuItem* m_menuItem29;
    wxMenuItem* m_menuItem27;

protected:
    virtual void onNew(wxCommandEvent& event) { event.Skip(); }
    virtual void onExit(wxCommandEvent& event) { event.Skip(); }

public:
    wxMenuBar* GetMenuBar() { return m_menuBar; }
    MimicEditorFrameBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Mimic Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_FRAME_STYLE);
    virtual ~MimicEditorFrameBase();
};


class DigitalLEDBright : public wxImageList
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
    DigitalLEDBright();
    const wxBitmap& Bitmap(const wxString &name) const {
        if ( !m_bitmaps.count(name + m_resolution) )
            return wxNullBitmap;
        return m_bitmaps.find(name + m_resolution)->second;
    }

    void SetBitmapResolution(const wxString &res = wxEmptyString) {
        m_resolution = res;
    }

    virtual ~DigitalLEDBright();
};


class DigitalNixie : public wxImageList
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
    DigitalNixie();
    const wxBitmap& Bitmap(const wxString &name) const {
        if ( !m_bitmaps.count(name + m_resolution) )
            return wxNullBitmap;
        return m_bitmaps.find(name + m_resolution)->second;
    }

    void SetBitmapResolution(const wxString &res = wxEmptyString) {
        m_resolution = res;
    }

    virtual ~DigitalNixie();
};

#endif
