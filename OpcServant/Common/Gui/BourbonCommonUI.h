//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: BourbonCommonUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _COMMON_GUI_BOURBONCOMMONUI_BASE_CLASSES_H
#define _COMMON_GUI_BOURBONCOMMONUI_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/arrstr.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/imaglist.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/grid.h>
#include <wx/timer.h>
#include <wx/checkbox.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/advprops.h>
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


class AddObjectDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer3;
    wxFlexGridSizer* flexGridSizer5;
    wxStaticText* m_staticText13;
    wxComboBox* m_objectName;
    wxStaticText* m_staticText17;
    wxChoice* m_type;
    wxStdDialogButtonSizer* m_stdBtnSizer7;
    wxButton* m_buttonOk;
    wxButton* m_buttonCancel;

protected:
    virtual void OnEditChanged(wxCommandEvent& event) { event.Skip(); }
    virtual void OnOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText13() { return m_staticText13; }
    wxComboBox* GetObjectName() { return m_objectName; }
    wxStaticText* GetStaticText17() { return m_staticText17; }
    wxChoice* GetType() { return m_type; }
    AddObjectDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add Object"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~AddObjectDialogBase();
};


class EditObjectDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer23;
    wxNotebook* m_notebook31;
    wxPanel* m_panelCommon;
    wxBoxSizer* CommonSizer;
    wxFlexGridSizer* flexGridSizer59;
    wxStaticText* m_staticText61;
    wxStaticText* m_objectName;
    wxStaticText* m_staticText65;
    wxStaticText* m_objectType;
    wxPanel* m_panelRecipe;
    wxBoxSizer* RecipeSizer;
    wxFlexGridSizer* flexGridSizer41;
    wxStaticText* m_staticText43;
    wxComboBox* m_recipe;
    wxPanel* m_panelNotes;
    wxBoxSizer* boxSizer47;
    wxTextCtrl* m_notes;
    wxStdDialogButtonSizer* m_stdBtnSizer25;
    wxButton* m_button29;
    wxButton* m_button27;

protected:
    virtual void OnRecipeSelectionChanged(wxCommandEvent& event) { event.Skip(); }
    virtual void OnOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText61() { return m_staticText61; }
    wxStaticText* GetObjectName() { return m_objectName; }
    wxStaticText* GetStaticText65() { return m_staticText65; }
    wxStaticText* GetObjectType() { return m_objectType; }
    wxPanel* GetPanelCommon() { return m_panelCommon; }
    wxStaticText* GetStaticText43() { return m_staticText43; }
    wxComboBox* GetRecipe() { return m_recipe; }
    wxPanel* GetPanelRecipe() { return m_panelRecipe; }
    wxTextCtrl* GetNotes() { return m_notes; }
    wxPanel* GetPanelNotes() { return m_panelNotes; }
    wxNotebook* GetNotebook31() { return m_notebook31; }
    EditObjectDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit Object"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX);
    virtual ~EditObjectDialogBase();
};


class ConfigureI2cDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer81;
    wxGridSizer* gridSizer83;
    wxStaticText* m_staticText85;
    wxChoice* m_i2cType;
    wxStaticText* m_staticText89;
    wxComboBox* m_i2cDevice;
    wxStaticText* m_staticText93;
    wxSpinCtrl* m_i2cPort;
    wxStdDialogButtonSizer* m_stdBtnSizer97;
    wxButton* m_button99;
    wxButton* m_button101;

protected:
    virtual void onTypeChanged(wxCommandEvent& event) { event.Skip(); }
    virtual void onOK(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText85() { return m_staticText85; }
    wxChoice* GetI2cType() { return m_i2cType; }
    wxStaticText* GetStaticText89() { return m_staticText89; }
    wxComboBox* GetI2cDevice() { return m_i2cDevice; }
    wxStaticText* GetStaticText93() { return m_staticText93; }
    wxSpinCtrl* GetI2cPort() { return m_i2cPort; }
    ConfigureI2cDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Configure I2C"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~ConfigureI2cDialogBase();
};


class GridPanelBase : public wxPanel
{
protected:
    wxBoxSizer* Sizer;
    wxGrid* m_grid;
    wxTimer* m_updateTimer;

protected:
    virtual void onTimer(wxTimerEvent& event) { event.Skip(); }

public:
    wxGrid* GetGrid() { return m_grid; }
    wxTimer* GetUpdateTimer() { return m_updateTimer; }
    GridPanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxTAB_TRAVERSAL);
    virtual ~GridPanelBase();
};


class SetupDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer117;
    wxNotebook* m_notebook;
    wxPanel* m_panel121;
    wxFlexGridSizer* setupSizer;
    wxStaticText* m_staticText131;
    wxSpinCtrl* m_measureInterval;
    wxStaticText* m_staticText135;
    wxSpinCtrl* m_publishInterval;
    wxCheckBox* m_Enabled;
    wxCheckBox* m_enableTabView;
    wxStdDialogButtonSizer* m_stdBtnSizer123;
    wxButton* m_button125;
    wxButton* m_button127;

protected:
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText131() { return m_staticText131; }
    wxSpinCtrl* GetMeasureInterval() { return m_measureInterval; }
    wxStaticText* GetStaticText135() { return m_staticText135; }
    wxSpinCtrl* GetPublishInterval() { return m_publishInterval; }
    wxCheckBox* GetEnabled() { return m_Enabled; }
    wxCheckBox* GetEnableTabView() { return m_enableTabView; }
    wxPanel* GetPanel121() { return m_panel121; }
    wxNotebook* GetNotebook() { return m_notebook; }
    SetupDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(600,300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX);
    virtual ~SetupDialogBase();
};


class I2cConfigurePanelBase : public wxPanel
{
protected:
    wxBoxSizer* boxSizer149;
    wxFlexGridSizer* flexGridSizer151;
    wxStaticText* m_staticText153;
    wxChoice* m_i2cType;
    wxStaticText* m_staticText157;
    wxComboBox* m_i2cDevice;
    wxStaticText* m_staticText161;
    wxSpinCtrl* m_i2cPort;

protected:

public:
    wxStaticText* GetStaticText153() { return m_staticText153; }
    wxChoice* GetI2cType() { return m_i2cType; }
    wxStaticText* GetStaticText157() { return m_staticText157; }
    wxComboBox* GetI2cDevice() { return m_i2cDevice; }
    wxStaticText* GetStaticText161() { return m_staticText161; }
    wxSpinCtrl* GetI2cPort() { return m_i2cPort; }
    I2cConfigurePanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxTAB_TRAVERSAL);
    virtual ~I2cConfigurePanelBase();
};


class SerialConfigurePanelBase : public wxPanel
{
protected:
    wxBoxSizer* boxSizer173;
    wxFlexGridSizer* flexGridSizer175;
    wxStaticText* m_staticText177;
    wxChoice* m_device;
    wxStaticText* m_staticText181;
    wxChoice* m_baudrate;
    wxStaticText* m_nodeLabel;
    wxSpinCtrl* m_nodeAddress;

protected:

public:
    wxStaticText* GetStaticText177() { return m_staticText177; }
    wxChoice* GetDevice() { return m_device; }
    wxStaticText* GetStaticText181() { return m_staticText181; }
    wxChoice* GetBaudrate() { return m_baudrate; }
    wxStaticText* GetNodeLabel() { return m_nodeLabel; }
    wxSpinCtrl* GetNodeAddress() { return m_nodeAddress; }
    SerialConfigurePanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(300,300), long style = wxTAB_TRAVERSAL);
    virtual ~SerialConfigurePanelBase();
};


class NetworkConfigurePanelBase : public wxPanel
{
protected:
    wxFlexGridSizer* flexGridSizer193;
    wxStaticText* m_staticText195;
    wxTextCtrl* m_address;
    wxStaticText* m_staticText199;
    wxSpinCtrl* m_port;
    wxButton* m_button203;

protected:
    virtual void onTest(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText195() { return m_staticText195; }
    wxTextCtrl* GetAddress() { return m_address; }
    wxStaticText* GetStaticText199() { return m_staticText199; }
    wxSpinCtrl* GetPort() { return m_port; }
    wxButton* GetButton203() { return m_button203; }
    NetworkConfigurePanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(300,200), long style = wxTAB_TRAVERSAL);
    virtual ~NetworkConfigurePanelBase();
};


class DatabaseConfigurePanelBase : public wxPanel
{
protected:
    wxFlexGridSizer* flexGridSizer207;
    wxStaticText* m_staticText209;
    wxTextCtrl* m_host;
    wxStaticText* m_staticText213;
    wxTextCtrl* m_database;
    wxStaticText* m_staticText217;
    wxSpinCtrl* m_port;
    wxStaticText* m_staticText221;
    wxTextCtrl* m_username;
    wxStaticText* m_staticText225;
    wxTextCtrl* m_password;
    wxButton* m_button229;

protected:
    virtual void onTest(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText209() { return m_staticText209; }
    wxTextCtrl* GetHost() { return m_host; }
    wxStaticText* GetStaticText213() { return m_staticText213; }
    wxTextCtrl* GetDatabase() { return m_database; }
    wxStaticText* GetStaticText217() { return m_staticText217; }
    wxSpinCtrl* GetPort() { return m_port; }
    wxStaticText* GetStaticText221() { return m_staticText221; }
    wxTextCtrl* GetUsername() { return m_username; }
    wxStaticText* GetStaticText225() { return m_staticText225; }
    wxTextCtrl* GetPassword() { return m_password; }
    wxButton* GetButton229() { return m_button229; }
    DatabaseConfigurePanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxTAB_TRAVERSAL);
    virtual ~DatabaseConfigurePanelBase();
};


class ScaleOffsetConfigurePanelBase : public wxPanel
{
protected:
    wxBoxSizer* boxSizer237;
    wxGrid* m_grid;

protected:

public:
    wxGrid* GetGrid() { return m_grid; }
    ScaleOffsetConfigurePanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxTAB_TRAVERSAL);
    virtual ~ScaleOffsetConfigurePanelBase();
};


class ConfigurationPropertySheetBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer247;
    wxPropertyGridManager* m_PropertySheet;
    wxPGProperty* m_pgProp257;
    wxStdDialogButtonSizer* m_stdBtnSizer251;
    wxButton* m_button253;
    wxButton* m_button255;

protected:
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxPropertyGridManager* GetPropertySheet() { return m_PropertySheet; }
    ConfigurationPropertySheetBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~ConfigurationPropertySheetBase();
};


class CommonImageList : public wxImageList
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
    CommonImageList();
    const wxBitmap& Bitmap(const wxString &name) const {
        if ( !m_bitmaps.count(name + m_resolution) )
            return wxNullBitmap;
        return m_bitmaps.find(name + m_resolution)->second;
    }

    void SetBitmapResolution(const wxString &res = wxEmptyString) {
        m_resolution = res;
    }

    virtual ~CommonImageList();
};

#endif
