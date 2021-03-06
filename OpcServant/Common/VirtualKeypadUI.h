//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: VirtualKeypadUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _OPCSERVANT_COMMON_VIRTUALKEYPADUI_BASE_CLASSES_H
#define _OPCSERVANT_COMMON_VIRTUALKEYPADUI_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/imaglist.h>
#include <wx/bitmap.h>
#include <map>
#include <wx/icon.h>
#include <wx/panel.h>
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


class VirtualKeypadBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer3;
    wxFlexGridSizer* flexGridSizer7;
    wxTextCtrl* m_textEntry;
    wxButton* m_button13;
    wxGridSizer* gridSizer5;
    wxButton* m_button37123;
    wxButton* m_button3712;
    wxButton* m_button371;
    wxButton* m_button37456;
    wxButton* m_button3745;
    wxButton* m_button374;
    wxButton* m_button3778910;
    wxButton* m_button37789;
    wxButton* m_button3778;
    wxButton* m_button37711;
    wxButton* m_button377;
    wxButton* m_button37;
    wxGridSizer* gridSizer9;
    wxButton* m_button50;
    wxButton* m_button54;

protected:
    virtual void onBack(wxCommandEvent& event) { event.Skip(); }
    virtual void on_1(wxCommandEvent& event) { event.Skip(); }
    virtual void on_2(wxCommandEvent& event) { event.Skip(); }
    virtual void on_3(wxCommandEvent& event) { event.Skip(); }
    virtual void on_4(wxCommandEvent& event) { event.Skip(); }
    virtual void on_5(wxCommandEvent& event) { event.Skip(); }
    virtual void on_6(wxCommandEvent& event) { event.Skip(); }
    virtual void on_7(wxCommandEvent& event) { event.Skip(); }
    virtual void on_8(wxCommandEvent& event) { event.Skip(); }
    virtual void on_9(wxCommandEvent& event) { event.Skip(); }
    virtual void on_dp(wxCommandEvent& event) { event.Skip(); }
    virtual void on_0(wxCommandEvent& event) { event.Skip(); }
    virtual void on_minus(wxCommandEvent& event) { event.Skip(); }
    virtual void onOK(wxCommandEvent& event) { event.Skip(); }
    virtual void onCancel(wxCommandEvent& event) { event.Skip(); }

public:
    wxTextCtrl* GetTextEntry() { return m_textEntry; }
    wxButton* GetButton13() { return m_button13; }
    wxButton* GetButton37123() { return m_button37123; }
    wxButton* GetButton3712() { return m_button3712; }
    wxButton* GetButton371() { return m_button371; }
    wxButton* GetButton37456() { return m_button37456; }
    wxButton* GetButton3745() { return m_button3745; }
    wxButton* GetButton374() { return m_button374; }
    wxButton* GetButton3778910() { return m_button3778910; }
    wxButton* GetButton37789() { return m_button37789; }
    wxButton* GetButton3778() { return m_button3778; }
    wxButton* GetButton37711() { return m_button37711; }
    wxButton* GetButton377() { return m_button377; }
    wxButton* GetButton37() { return m_button37; }
    wxButton* GetButton50() { return m_button50; }
    wxButton* GetButton54() { return m_button54; }
    VirtualKeypadBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Keypad"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,400), long style = wxBORDER_SIMPLE);
    virtual ~VirtualKeypadBase();
};


class ButtonImages : public wxImageList
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
    ButtonImages();
    const wxBitmap& Bitmap(const wxString &name) const {
        if ( !m_bitmaps.count(name + m_resolution) )
            return wxNullBitmap;
        return m_bitmaps.find(name + m_resolution)->second;
    }

    void SetBitmapResolution(const wxString &res = wxEmptyString) {
        m_resolution = res;
    }

    virtual ~ButtonImages();
};


class VkTextControlBase : public wxPanel
{
protected:
    wxFlexGridSizer* flexGridSizer63;
    wxTextCtrl* m_text;
    wxButton* m_button67;

protected:
    virtual void onOpen(wxCommandEvent& event) { event.Skip(); }

public:
    wxTextCtrl* GetText() { return m_text; }
    wxButton* GetButton67() { return m_button67; }
    VkTextControlBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(128,48), long style = wxTAB_TRAVERSAL);
    virtual ~VkTextControlBase();
};

#endif
