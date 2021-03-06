//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: MQTTUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "MQTTUI.h"


// Declare the bitmap loading function
extern void wxCrafterhtlM1hInitBitmapResources();

static bool bBitmapLoaded = false;


MqttSetupDialogBase::MqttSetupDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterhtlM1hInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer3 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer3);
    
    flexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer5->SetFlexibleDirection( wxBOTH );
    flexGridSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer5->AddGrowableCol(1);
    
    boxSizer3->Add(flexGridSizer5, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText13 = new wxStaticText(this, wxID_ANY, _("Host"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText13, 0, wxALL, WXC_FROM_DIP(5));
    
    m_host = new wxTextCtrl(this, wxID_ANY, wxT("localhost"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_host->SetHint(wxT(""));
    #endif
    
    flexGridSizer5->Add(m_host, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText17 = new wxStaticText(this, wxID_ANY, _("Ident"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText17, 0, wxALL, WXC_FROM_DIP(5));
    
    m_ident = new wxTextCtrl(this, wxID_ANY, wxT("OPCSERVANT"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_ident->SetHint(wxT(""));
    #endif
    
    flexGridSizer5->Add(m_ident, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText21 = new wxStaticText(this, wxID_ANY, _("Port"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText21, 0, wxALL, WXC_FROM_DIP(5));
    
    m_port = new wxSpinCtrl(this, wxID_ANY, wxT("1883"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_port->SetRange(1025, 32000);
    m_port->SetValue(1883);
    
    flexGridSizer5->Add(m_port, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText31 = new wxStaticText(this, wxID_ANY, _("Username"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText31, 0, wxALL, WXC_FROM_DIP(5));
    
    m_username = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_username->SetHint(wxT(""));
    #endif
    
    flexGridSizer5->Add(m_username, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText35 = new wxStaticText(this, wxID_ANY, _("Password"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText35, 0, wxALL, WXC_FROM_DIP(5));
    
    m_password = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_password->SetHint(wxT(""));
    #endif
    
    flexGridSizer5->Add(m_password, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText25 = new wxStaticText(this, wxID_ANY, _("Filter"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText25, 0, wxALL, WXC_FROM_DIP(5));
    
    m_filter = new wxTextCtrl(this, wxID_ANY, wxT("*"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_filter->SetHint(wxT(""));
    #endif
    
    flexGridSizer5->Add(m_filter, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText39 = new wxStaticText(this, wxID_ANY, _("Command Sub."), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText39, 0, wxALL, WXC_FROM_DIP(5));
    
    m_commandSub = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_commandSub->SetHint(wxT(""));
    #endif
    
    flexGridSizer5->Add(m_commandSub, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_enable = new wxCheckBox(this, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    m_enable->SetValue(false);
    
    flexGridSizer5->Add(m_enable, 0, wxALL, WXC_FROM_DIP(5));
    
    m_stdBtnSizer7 = new wxStdDialogButtonSizer();
    
    boxSizer3->Add(m_stdBtnSizer7, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button9 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer7->AddButton(m_button9);
    
    m_button11 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer7->AddButton(m_button11);
    m_stdBtnSizer7->Realize();
    
    SetName(wxT("MqttSetupDialogBase"));
    SetSize(300,400);
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
    m_button11->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MqttSetupDialogBase::OnOk), NULL, this);
    
}

MqttSetupDialogBase::~MqttSetupDialogBase()
{
    m_button11->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MqttSetupDialogBase::OnOk), NULL, this);
    
}
