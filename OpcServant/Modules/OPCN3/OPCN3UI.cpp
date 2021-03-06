//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: OPCN3UI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "OPCN3UI.h"


// Declare the bitmap loading function
extern void wxCrafterydm7r3InitBitmapResources();

static bool bBitmapLoaded = false;


OPCN3ConfigurationDialogBase::OPCN3ConfigurationDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterydm7r3InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer3 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer3);
    
    flexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer5->SetFlexibleDirection( wxBOTH );
    flexGridSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    boxSizer3->Add(flexGridSizer5, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText39 = new wxStaticText(this, wxID_ANY, _("SPI Device"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText39, 0, wxALL, WXC_FROM_DIP(5));
    
    m_address = new wxTextCtrl(this, wxID_ANY, wxT("spi0.0"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_address->SetHint(wxT(""));
    #endif
    
    flexGridSizer5->Add(m_address, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText7 = new wxStaticText(this, wxID_ANY, _("Wait Interval (mins)"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText7, 0, wxALL, WXC_FROM_DIP(5));
    
    m_waitInterval = new wxSpinCtrl(this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_waitInterval->SetRange(0, 1000);
    m_waitInterval->SetValue(0);
    
    flexGridSizer5->Add(m_waitInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText11 = new wxStaticText(this, wxID_ANY, _("Sample Interval (s)"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText11, 0, wxALL, WXC_FROM_DIP(5));
    
    m_sampleInterval = new wxSpinCtrl(this, wxID_ANY, wxT("30"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_sampleInterval->SetRange(10, 60);
    m_sampleInterval->SetValue(30);
    
    flexGridSizer5->Add(m_sampleInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_Enabled = new wxCheckBox(this, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    m_Enabled->SetValue(false);
    
    flexGridSizer5->Add(m_Enabled, 0, wxALL, WXC_FROM_DIP(5));
    
    m_enableTabView = new wxCheckBox(this, wxID_ANY, _("Enable Tab View"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    m_enableTabView->SetValue(false);
    
    flexGridSizer5->Add(m_enableTabView, 0, wxALL, WXC_FROM_DIP(5));
    
    m_stdBtnSizer54 = new wxStdDialogButtonSizer();
    
    boxSizer3->Add(m_stdBtnSizer54, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button56 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer54->AddButton(m_button56);
    
    m_button58 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer54->AddButton(m_button58);
    m_stdBtnSizer54->Realize();
    
    SetName(wxT("OPCN3ConfigurationDialogBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
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
    m_button56->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OPCN3ConfigurationDialogBase::onOk), NULL, this);
    
}

OPCN3ConfigurationDialogBase::~OPCN3ConfigurationDialogBase()
{
    m_button56->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OPCN3ConfigurationDialogBase::onOk), NULL, this);
    
}

OPCN3PanelBase::OPCN3PanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterydm7r3InitBitmapResources();
        bBitmapLoaded = true;
    }
    wxFont thisFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Ubuntu"));
    this->SetFont(thisFont);
    
    gridSizer25 = new wxGridSizer(0, 2, 0, 0);
    this->SetSizer(gridSizer25);
    
    m_staticText27 = new wxStaticText(this, wxID_ANY, _("PM1"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer25->Add(m_staticText27, 0, wxALL, WXC_FROM_DIP(5));
    
    m_pm1 = new wxStaticText(this, wxID_ANY, _("0.0"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer25->Add(m_pm1, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText31 = new wxStaticText(this, wxID_ANY, _("PM2.5"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer25->Add(m_staticText31, 0, wxALL, WXC_FROM_DIP(5));
    
    m_pm2_5 = new wxStaticText(this, wxID_ANY, _("0.0"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer25->Add(m_pm2_5, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText45 = new wxStaticText(this, wxID_ANY, _("PM10"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer25->Add(m_staticText45, 0, wxALL, WXC_FROM_DIP(5));
    
    m_pm10 = new wxStaticText(this, wxID_ANY, _("0.0"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer25->Add(m_pm10, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText35 = new wxStaticText(this, wxID_ANY, _("Status"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer25->Add(m_staticText35, 0, wxALL, WXC_FROM_DIP(5));
    
    m_status = new wxStaticText(this, wxID_ANY, _("OK"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer25->Add(m_status, 0, wxALL, WXC_FROM_DIP(5));
    
    m_timer60 = new wxTimer;
    m_timer60->Start(5000, false);
    
    SetName(wxT("OPCN3PanelBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    // Connect events
    m_timer60->Connect(wxEVT_TIMER, wxTimerEventHandler(OPCN3PanelBase::onTimer), NULL, this);
    
}

OPCN3PanelBase::~OPCN3PanelBase()
{
    m_timer60->Disconnect(wxEVT_TIMER, wxTimerEventHandler(OPCN3PanelBase::onTimer), NULL, this);
    
    m_timer60->Stop();
    wxDELETE( m_timer60 );

}
