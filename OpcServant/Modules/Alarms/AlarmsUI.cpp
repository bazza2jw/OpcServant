//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: AlarmsUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "AlarmsUI.h"


// Declare the bitmap loading function
extern void wxCraftermDl6ERInitBitmapResources();

static bool bBitmapLoaded = false;


AlarmConfigurationDialogBase::AlarmConfigurationDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCraftermDl6ERInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer3 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer3);
    
    gridSizer48 = new wxGridSizer(0, 2, 0, 0);
    
    boxSizer3->Add(gridSizer48, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText50 = new wxStaticText(this, wxID_ANY, _("Measure Interval"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer48->Add(m_staticText50, 0, wxALL, WXC_FROM_DIP(5));
    
    m_measureInterval = new wxSpinCtrl(this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_measureInterval->SetRange(0, 100);
    m_measureInterval->SetValue(0);
    
    gridSizer48->Add(m_measureInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText60 = new wxStaticText(this, wxID_ANY, _("Static Text Label"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer48->Add(m_staticText60, 0, wxALL, WXC_FROM_DIP(5));
    
    m_publishInterval = new wxSpinCtrl(this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_publishInterval->SetRange(0, 100);
    m_publishInterval->SetValue(0);
    
    gridSizer48->Add(m_publishInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_enable = new wxCheckBox(this, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    m_enable->SetValue(false);
    
    gridSizer48->Add(m_enable, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_groupListArr;
    m_groupList = new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_groupListArr, wxLB_SINGLE);
    
    boxSizer3->Add(m_groupList, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    boxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer3->Add(boxSizer19, 0, wxALL, WXC_FROM_DIP(5));
    
    gridSizer44 = new wxGridSizer(0, 3, 0, 0);
    
    boxSizer3->Add(gridSizer44, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText27 = new wxStaticText(this, wxID_ANY, _("Action"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer44->Add(m_staticText27, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_actionObjectArr;
    m_actionObject = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_actionObjectArr, 0);
    
    gridSizer44->Add(m_actionObject, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxArrayString m_actionOutputArr;
    m_actionOutput = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_actionOutputArr, 0);
    
    gridSizer44->Add(m_actionOutput, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText33 = new wxStaticText(this, wxID_ANY, _("Alert"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer44->Add(m_staticText33, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_alertObjectArr;
    m_alertObject = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_alertObjectArr, 0);
    
    gridSizer44->Add(m_alertObject, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxArrayString m_alertOutputArr;
    m_alertOutput = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_alertOutputArr, 0);
    
    gridSizer44->Add(m_alertOutput, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText39 = new wxStaticText(this, wxID_ANY, _("Ok"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer44->Add(m_staticText39, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_okObjectArr;
    m_okObject = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_okObjectArr, 0);
    
    gridSizer44->Add(m_okObject, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxArrayString m_okOutputArr;
    m_okOutput = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_okOutputArr, 0);
    
    gridSizer44->Add(m_okOutput, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_stdBtnSizer7 = new wxStdDialogButtonSizer();
    
    boxSizer3->Add(m_stdBtnSizer7, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button9 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer7->AddButton(m_button9);
    
    m_button11 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer7->AddButton(m_button11);
    m_stdBtnSizer7->Realize();
    
    SetName(wxT("AlarmConfigurationDialogBase"));
    SetMinClientSize(wxSize(600,500));
    SetSize(600,500);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    m_actionObject->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(AlarmConfigurationDialogBase::OnActionObject), NULL, this);
    m_alertObject->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(AlarmConfigurationDialogBase::OnAlertObject), NULL, this);
    m_okObject->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(AlarmConfigurationDialogBase::OnOkObject), NULL, this);
    m_button11->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AlarmConfigurationDialogBase::OnOk), NULL, this);
    
}

AlarmConfigurationDialogBase::~AlarmConfigurationDialogBase()
{
    m_actionObject->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(AlarmConfigurationDialogBase::OnActionObject), NULL, this);
    m_alertObject->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(AlarmConfigurationDialogBase::OnAlertObject), NULL, this);
    m_okObject->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(AlarmConfigurationDialogBase::OnOkObject), NULL, this);
    m_button11->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AlarmConfigurationDialogBase::OnOk), NULL, this);
    
}
