//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: BUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef _COMMON_GUI_BUI_BASE_CLASSES_H
#define _COMMON_GUI_BUI_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/dialog.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/advprops.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/imaglist.h>
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/arrstr.h>
#include <wx/timectrl.h>
#include <wx/dateevt.h>
#include <wx/datectrl.h>
#include <wx/statbmp.h>
#include <wx/dataview.h>
#include <wx/checklst.h>
#include <wx/scrolbar.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
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


class SystemPropertiesDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer55;
    wxPropertyGridManager* m_properties;
    wxPGProperty* m_siteName;
    wxPGProperty* m_enableGui;
    wxPGProperty* m_enableWeb;
    wxPGProperty* m_enableOpc;
    wxPGProperty* m_mainTab;
    wxPGProperty* m_autoPurge;
    wxPGProperty* m_enableVK;
    wxPGProperty* m_screenLock;
    wxPGProperty* m_screenLockPin;
    wxPGProperty* m_homePage;
    wxPGProperty* m_pgProp87;
    wxPGProperty* m_opcPort;
    wxPGProperty* m_opcNamespace;
    wxPGProperty* m_loginRequired;
    wxPGProperty* m_opcUsername;
    wxPGProperty* m_opcPassword;
    wxGridSizer* gridSizer161;
    wxButton* m_button163;
    wxButton* m_buttonUsers;
    wxStdDialogButtonSizer* m_stdBtnSizer57;
    wxButton* m_button59;
    wxButton* m_button61;

protected:
    virtual void onFactoryReset(wxCommandEvent& event) { event.Skip(); }
    virtual void onUsers(wxCommandEvent& event) { event.Skip(); }
    virtual void OnOK(wxCommandEvent& event) { event.Skip(); }

public:
    wxPropertyGridManager* GetProperties() { return m_properties; }
    wxButton* GetButton163() { return m_button163; }
    wxButton* GetButtonUsers() { return m_buttonUsers; }
    SystemPropertiesDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("System Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,450), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX);
    virtual ~SystemPropertiesDialogBase();
};


class ReportGeneratorPanelBase : public wxPanel
{
protected:
    wxBoxSizer* boxSizer185;
    wxNotebook* m_notebook;
    wxPanel* m_panelSetup;
    wxBoxSizer* boxSizer197;
    wxBoxSizer* boxSizer215;
    wxListBox* m_listDataPoints;
    wxFlexGridSizer* flexGridSizer199;
    wxStaticText* m_staticText271;
    wxChoice* m_choiceTimeFrame;
    wxStaticText* m_staticText223;
    wxTimePickerCtrl* m_timeStart;
    wxStaticText* m_staticText275;
    wxDatePickerCtrl* m_dateStart;
    wxStaticText* m_staticText227;
    wxTimePickerCtrl* m_timeEnd;
    wxStaticText* m_staticText279;
    wxDatePickerCtrl* m_dateEnd;
    wxGridSizer* gridSizer207;
    wxButton* m_buttonLoad;
    wxButton* m_buttonSave;
    wxButton* m_buttonRemove;
    wxButton* m_buttonMake;
    wxButton* m_buttonNew;
    wxButton* m_buttonPrint;
    wxButton* m_buttonExport;
    wxPanel* m_panelTable;
    wxBoxSizer* boxSizer233;
    wxNotebook* m_reportTables;
    wxPanel* m_panelGraph;
    wxBoxSizer* boxSizerGraph;
    wxStaticBitmap* m_graphBitmap;
    wxPanel* m_panelStatistics;
    wxBoxSizer* boxSizer251;
    wxDataViewListCtrl* m_statsTable;
    wxPanel* m_panelDataPoints;
    wxBoxSizer* boxSizer265;
    wxBoxSizer* boxSizer267;
    wxStaticText* m_staticText487;
    wxCheckListBox* m_valueList;
    wxStaticText* m_staticText489;
    wxCheckListBox* m_AliasList;
    wxButton* m_buttonRefreshValues;

protected:
    virtual void OnTimeframeChoice(wxCommandEvent& event) { event.Skip(); }
    virtual void onLoadReport(wxCommandEvent& event) { event.Skip(); }
    virtual void onSaveReport(wxCommandEvent& event) { event.Skip(); }
    virtual void onRemoveReport(wxCommandEvent& event) { event.Skip(); }
    virtual void onMakeReport(wxCommandEvent& event) { event.Skip(); }
    virtual void OnNew(wxCommandEvent& event) { event.Skip(); }
    virtual void onPrint(wxCommandEvent& event) { event.Skip(); }
    virtual void onExport(wxCommandEvent& event) { event.Skip(); }
    virtual void onRefreshValues(wxCommandEvent& event) { event.Skip(); }

public:
    wxListBox* GetListDataPoints() { return m_listDataPoints; }
    wxStaticText* GetStaticText271() { return m_staticText271; }
    wxChoice* GetChoiceTimeFrame() { return m_choiceTimeFrame; }
    wxStaticText* GetStaticText223() { return m_staticText223; }
    wxTimePickerCtrl* GetTimeStart() { return m_timeStart; }
    wxStaticText* GetStaticText275() { return m_staticText275; }
    wxDatePickerCtrl* GetDateStart() { return m_dateStart; }
    wxStaticText* GetStaticText227() { return m_staticText227; }
    wxTimePickerCtrl* GetTimeEnd() { return m_timeEnd; }
    wxStaticText* GetStaticText279() { return m_staticText279; }
    wxDatePickerCtrl* GetDateEnd() { return m_dateEnd; }
    wxButton* GetButtonLoad() { return m_buttonLoad; }
    wxButton* GetButtonSave() { return m_buttonSave; }
    wxButton* GetButtonRemove() { return m_buttonRemove; }
    wxButton* GetButtonMake() { return m_buttonMake; }
    wxButton* GetButtonNew() { return m_buttonNew; }
    wxButton* GetButtonPrint() { return m_buttonPrint; }
    wxButton* GetButtonExport() { return m_buttonExport; }
    wxPanel* GetPanelSetup() { return m_panelSetup; }
    wxNotebook* GetReportTables() { return m_reportTables; }
    wxPanel* GetPanelTable() { return m_panelTable; }
    wxStaticBitmap* GetGraphBitmap() { return m_graphBitmap; }
    wxPanel* GetPanelGraph() { return m_panelGraph; }
    wxDataViewListCtrl* GetStatsTable() { return m_statsTable; }
    wxPanel* GetPanelStatistics() { return m_panelStatistics; }
    wxStaticText* GetStaticText487() { return m_staticText487; }
    wxCheckListBox* GetValueList() { return m_valueList; }
    wxStaticText* GetStaticText489() { return m_staticText489; }
    wxCheckListBox* GetAliasList() { return m_AliasList; }
    wxButton* GetButtonRefreshValues() { return m_buttonRefreshValues; }
    wxPanel* GetPanelDataPoints() { return m_panelDataPoints; }
    wxNotebook* GetNotebook() { return m_notebook; }
    ReportGeneratorPanelBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(700,500), long style = wxTAB_TRAVERSAL);
    virtual ~ReportGeneratorPanelBase();
};


class ReportTablePageBase : public wxPanel
{
protected:
    wxBoxSizer* boxSizer341;
    wxStaticText* m_itemName;
    wxBoxSizer* boxSizer349;
    wxDataViewListCtrl* m_Table;
    wxScrollBar* m_tableScrollBar;

protected:
    virtual void onSize(wxSizeEvent& event) { event.Skip(); }
    virtual void onScrollChanged(wxScrollEvent& event) { event.Skip(); }

public:
    wxStaticText* GetItemName() { return m_itemName; }
    wxDataViewListCtrl* GetTable() { return m_Table; }
    wxScrollBar* GetTableScrollBar() { return m_tableScrollBar; }
    ReportTablePageBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(700,500), long style = wxTAB_TRAVERSAL);
    virtual ~ReportTablePageBase();
};


class ReportPrintOptionsBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer357;
    wxCheckBox* m_printTable;
    wxCheckBox* m_printGraph;
    wxCheckBox* m_printStats;
    wxStdDialogButtonSizer* m_stdBtnSizer359;
    wxButton* m_button373;
    wxButton* m_button371;

protected:
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxCheckBox* GetPrintTable() { return m_printTable; }
    wxCheckBox* GetPrintGraph() { return m_printGraph; }
    wxCheckBox* GetPrintStats() { return m_printStats; }
    ReportPrintOptionsBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Print Report"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE);
    virtual ~ReportPrintOptionsBase();
};


class AliasConfigurationDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer407;
    wxBoxSizer* boxSizer471;
    wxDataViewListCtrl* m_aliasList;
    wxBoxSizer* boxSizer477;
    wxButton* m_button479;
    wxButton* m_button481;
    wxButton* m_button483;
    wxStdDialogButtonSizer* m_stdBtnSizer413;
    wxButton* m_button415;
    wxButton* m_button417;

protected:
    virtual void onAdd(wxCommandEvent& event) { event.Skip(); }
    virtual void onEdit(wxCommandEvent& event) { event.Skip(); }
    virtual void onDelete(wxCommandEvent& event) { event.Skip(); }
    virtual void onOK(wxCommandEvent& event) { event.Skip(); }

public:
    wxDataViewListCtrl* GetAliasList() { return m_aliasList; }
    wxButton* GetButton479() { return m_button479; }
    wxButton* GetButton481() { return m_button481; }
    wxButton* GetButton483() { return m_button483; }
    AliasConfigurationDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Alias Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxMAXIMIZE_BOX);
    virtual ~AliasConfigurationDialogBase();
};


class SelectValueDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer433;
    wxListBox* m_listAlias;
    wxStdDialogButtonSizer* m_stdBtnSizer437;
    wxButton* m_button439;
    wxButton* m_button441;

protected:
    virtual void onSelect(wxCommandEvent& event) { event.Skip(); }
    virtual void onOK(wxCommandEvent& event) { event.Skip(); }

public:
    wxListBox* GetListAlias() { return m_listAlias; }
    SelectValueDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select Value"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~SelectValueDialogBase();
};


class AddAliasDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer449;
    wxFlexGridSizer* flexGridSizer451;
    wxStaticText* m_staticText459;
    wxTextCtrl* m_alias;
    wxStaticText* m_staticText463;
    wxTextCtrl* m_valueString;
    wxButton* m_button467;
    wxStdDialogButtonSizer* m_stdBtnSizer453;
    wxButton* m_button455;
    wxButton* m_button457;

protected:
    virtual void onSelect(wxCommandEvent& event) { event.Skip(); }
    virtual void onOK(wxCommandEvent& event) { event.Skip(); }

public:
    wxStaticText* GetStaticText459() { return m_staticText459; }
    wxTextCtrl* GetAlias() { return m_alias; }
    wxStaticText* GetStaticText463() { return m_staticText463; }
    wxTextCtrl* GetValueString() { return m_valueString; }
    wxButton* GetButton467() { return m_button467; }
    AddAliasDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add Alias"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~AddAliasDialogBase();
};


class UserConfigurationDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer515;
    wxPanel* m_panel517;
    wxBoxSizer* boxSizer525;
    wxListBox* m_listUsers;
    wxPanel* m_panel529;
    wxBoxSizer* boxSizer533;
    wxFlexGridSizer* flexGridSizer535;
    wxStaticText* m_staticText545;
    wxTextCtrl* m_password;
    wxCheckBox* m_Enabled;
    wxCheckBox* m_Admin;
    wxGridSizer* gridSizer537;
    wxButton* m_buttonNew;
    wxButton* m_buttonApply;
    wxButton* m_buttonDelete;
    wxStdDialogButtonSizer* m_stdBtnSizer519;
    wxButton* m_button521;
    wxButton* m_button523;

protected:
    virtual void onSelection(wxCommandEvent& event) { event.Skip(); }
    virtual void onNew(wxCommandEvent& event) { event.Skip(); }
    virtual void onApply(wxCommandEvent& event) { event.Skip(); }
    virtual void onDelete(wxCommandEvent& event) { event.Skip(); }
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxListBox* GetListUsers() { return m_listUsers; }
    wxStaticText* GetStaticText545() { return m_staticText545; }
    wxTextCtrl* GetPassword() { return m_password; }
    wxCheckBox* GetEnabled() { return m_Enabled; }
    wxCheckBox* GetAdmin() { return m_Admin; }
    wxButton* GetButtonNew() { return m_buttonNew; }
    wxButton* GetButtonApply() { return m_buttonApply; }
    wxButton* GetButtonDelete() { return m_buttonDelete; }
    wxPanel* GetPanel529() { return m_panel529; }
    wxPanel* GetPanel517() { return m_panel517; }
    UserConfigurationDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("User Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,500), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~UserConfigurationDialogBase();
};


class PinEntryDialogBase : public wxDialog
{
protected:
    wxBoxSizer* boxSizer561;
    wxTextCtrl* m_text;
    wxGridSizer* gridSizer563;
    wxButton* m_button567;
    wxButton* m_button569;
    wxButton* m_button571;
    wxButton* m_button573;
    wxButton* m_button575;
    wxButton* m_button577;
    wxButton* m_button579;
    wxButton* m_button581;
    wxButton* m_button583;
    wxButton* m_button585;
    wxButton* m_button587;
    wxButton* m_button589;

protected:
    virtual void on1(wxCommandEvent& event) { event.Skip(); }
    virtual void on2(wxCommandEvent& event) { event.Skip(); }
    virtual void on3(wxCommandEvent& event) { event.Skip(); }
    virtual void on4(wxCommandEvent& event) { event.Skip(); }
    virtual void on5(wxCommandEvent& event) { event.Skip(); }
    virtual void on6(wxCommandEvent& event) { event.Skip(); }
    virtual void on7(wxCommandEvent& event) { event.Skip(); }
    virtual void on8(wxCommandEvent& event) { event.Skip(); }
    virtual void on9(wxCommandEvent& event) { event.Skip(); }
    virtual void onDP(wxCommandEvent& event) { event.Skip(); }
    virtual void on0(wxCommandEvent& event) { event.Skip(); }
    virtual void onOk(wxCommandEvent& event) { event.Skip(); }

public:
    wxTextCtrl* GetText() { return m_text; }
    wxButton* GetButton567() { return m_button567; }
    wxButton* GetButton569() { return m_button569; }
    wxButton* GetButton571() { return m_button571; }
    wxButton* GetButton573() { return m_button573; }
    wxButton* GetButton575() { return m_button575; }
    wxButton* GetButton577() { return m_button577; }
    wxButton* GetButton579() { return m_button579; }
    wxButton* GetButton581() { return m_button581; }
    wxButton* GetButton583() { return m_button583; }
    wxButton* GetButton585() { return m_button585; }
    wxButton* GetButton587() { return m_button587; }
    wxButton* GetButton589() { return m_button589; }
    PinEntryDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("PIN Entry"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,500), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~PinEntryDialogBase();
};

#endif
