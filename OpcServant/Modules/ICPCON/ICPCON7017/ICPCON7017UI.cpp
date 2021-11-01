//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: ICPCON7017UI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "ICPCON7017UI.h"


// Declare the bitmap loading function
extern void wxCrafterzey5oKInitBitmapResources();

static bool bBitmapLoaded = false;


ICPCON7017SetupDialogBase::ICPCON7017SetupDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterzey5oKInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer3 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer3);
    
    m_notebook5 = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxBK_DEFAULT);
    m_notebook5->SetName(wxT("m_notebook5"));
    
    boxSizer3->Add(m_notebook5, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelSettings = new wxPanel(m_notebook5, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook5, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebook5->AddPage(m_panelSettings, _("Settings"), false);
    
    gridSizer17 = new wxGridSizer(0, 2, 0, 0);
    m_panelSettings->SetSizer(gridSizer17);
    
    m_staticText19 = new wxStaticText(m_panelSettings, wxID_ANY, _("Serial Port"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), 0);
    
    gridSizer17->Add(m_staticText19, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_serialPortsArr;
    m_serialPortsArr.Add(wxT("ttyUSB0"));
    m_serialPortsArr.Add(wxT("ttyUSB1"));
    m_serialPortsArr.Add(wxT("ttyUSB2"));
    m_serialPortsArr.Add(wxT("ttyUSB3"));
    m_serialPorts = new wxChoice(m_panelSettings, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), m_serialPortsArr, 0);
    m_serialPorts->SetSelection(0);
    
    gridSizer17->Add(m_serialPorts, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText23 = new wxStaticText(m_panelSettings, wxID_ANY, _("Baud Rate"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), 0);
    
    gridSizer17->Add(m_staticText23, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_baudRateArr;
    m_baudRateArr.Add(wxT("9600"));
    m_baudRateArr.Add(wxT("19200"));
    m_baudRateArr.Add(wxT("57600"));
    m_baudRate = new wxChoice(m_panelSettings, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), m_baudRateArr, 0);
    m_baudRate->SetSelection(2);
    
    gridSizer17->Add(m_baudRate, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText27 = new wxStaticText(m_panelSettings, wxID_ANY, _("Node Address"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), 0);
    
    gridSizer17->Add(m_staticText27, 0, wxALL, WXC_FROM_DIP(5));
    
    m_nodeAddress = new wxSpinCtrl(m_panelSettings, wxID_ANY, wxT("1"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_nodeAddress->SetRange(1, 100);
    m_nodeAddress->SetValue(1);
    
    gridSizer17->Add(m_nodeAddress, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText31 = new wxStaticText(m_panelSettings, wxID_ANY, _("Measure Interval"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), 0);
    
    gridSizer17->Add(m_staticText31, 0, wxALL, WXC_FROM_DIP(5));
    
    m_measureInterval = new wxSpinCtrl(m_panelSettings, wxID_ANY, wxT("1"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_measureInterval->SetRange(1, 100);
    m_measureInterval->SetValue(1);
    
    gridSizer17->Add(m_measureInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText35 = new wxStaticText(m_panelSettings, wxID_ANY, _("Publish Interval"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), 0);
    
    gridSizer17->Add(m_staticText35, 0, wxALL, WXC_FROM_DIP(5));
    
    m_publishInterval = new wxSpinCtrl(m_panelSettings, wxID_ANY, wxT("5"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_publishInterval->SetRange(10, 100);
    m_publishInterval->SetValue(10);
    
    gridSizer17->Add(m_publishInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_enable = new wxCheckBox(m_panelSettings, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), 0);
    m_enable->SetValue(false);
    
    gridSizer17->Add(m_enable, 0, wxALL, WXC_FROM_DIP(5));
    
    m_enableTabView = new wxCheckBox(m_panelSettings, wxID_ANY, _("Enable Tab View"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), 0);
    m_enableTabView->SetValue(false);
    
    gridSizer17->Add(m_enableTabView, 0, wxALL, WXC_FROM_DIP(5));
    
    m_publishRecord = new wxCheckBox(m_panelSettings, wxID_ANY, _("Publish Record"), wxDefaultPosition, wxDLG_UNIT(m_panelSettings, wxSize(-1,-1)), 0);
    m_publishRecord->SetValue(false);
    
    gridSizer17->Add(m_publishRecord, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelScale = new wxPanel(m_notebook5, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook5, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebook5->AddPage(m_panelScale, _("Scale"), false);
    
    boxSizer43 = new wxBoxSizer(wxVERTICAL);
    m_panelScale->SetSizer(boxSizer43);
    
    m_scaleTable = new wxGrid(m_panelScale, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelScale, wxSize(-1,-1)), wxWANTS_CHARS);
    m_scaleTable->CreateGrid(7, 3);
    m_scaleTable->SetRowLabelValue(0, _("IN0"));
    m_scaleTable->SetRowLabelValue(1, _("IN1"));
    m_scaleTable->SetRowLabelValue(2, _("IN2"));
    m_scaleTable->SetRowLabelValue(3, _("IN3"));
    m_scaleTable->SetRowLabelValue(4, _("IN4"));
    m_scaleTable->SetRowLabelValue(5, _("IN5"));
    m_scaleTable->SetRowLabelValue(6, _("IN6"));
    m_scaleTable->SetColLabelValue(0, _("Scale"));
    m_scaleTable->SetColSize(0, 200);
    m_scaleTable->SetColLabelValue(1, _("Offset"));
    m_scaleTable->SetColSize(1, 200);
    m_scaleTable->SetColLabelValue(2, _("Units"));
    m_scaleTable->SetColSize(2, 200);
    m_scaleTable->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_scaleTable->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_scaleTable->UseNativeColHeader(true);
    #endif
    m_scaleTable->EnableEditing(true);
    
    boxSizer43->Add(m_scaleTable, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_stdBtnSizer9 = new wxStdDialogButtonSizer();
    
    boxSizer3->Add(m_stdBtnSizer9, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button11 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer9->AddButton(m_button11);
    
    m_button13 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer9->AddButton(m_button13);
    m_stdBtnSizer9->Realize();
    
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_notebook5)){
        wxPersistenceManager::Get().RegisterAndRestore(m_notebook5);
    } else {
        wxPersistenceManager::Get().Restore(m_notebook5);
    }
    #endif
    
    SetName(wxT("ICPCON7017SetupDialogBase"));
    SetSize(600,400);
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
    m_button11->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ICPCON7017SetupDialogBase::OnOk), NULL, this);
    
}

ICPCON7017SetupDialogBase::~ICPCON7017SetupDialogBase()
{
    m_button11->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ICPCON7017SetupDialogBase::OnOk), NULL, this);
    
}

ICPCON7017PanelBase::ICPCON7017PanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterzey5oKInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer69 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer69);
    
    m_panel169 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_panel169->SetBackgroundColour(wxColour(wxT("rgb(32,32,32)")));
    m_panel169->SetForegroundColour(wxColour(wxT("rgb(39,229,71)")));
    wxFont m_panel169Font(16, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Monospace"));
    m_panel169->SetFont(m_panel169Font);
    
    boxSizer69->Add(m_panel169, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    gridSizer171 = new wxGridSizer(0, 3, 0, 0);
    m_panel169->SetSizer(gridSizer171);
    
    m_staticText173 = new wxStaticText(m_panel169, wxID_ANY, _("IN0"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_staticText173, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN0 = new wxStaticText(m_panel169, wxID_ANY, _("0.000"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN0, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN0units = new wxStaticText(m_panel169, wxID_ANY, _("V"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN0units, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText179 = new wxStaticText(m_panel169, wxID_ANY, _("IN1"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_staticText179, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN1 = new wxStaticText(m_panel169, wxID_ANY, _("0.000"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN1, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN1units = new wxStaticText(m_panel169, wxID_ANY, _("V"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN1units, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText185 = new wxStaticText(m_panel169, wxID_ANY, _("IN2"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_staticText185, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN2 = new wxStaticText(m_panel169, wxID_ANY, _("0.000"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN2, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_IN2units = new wxStaticText(m_panel169, wxID_ANY, _("V"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN2units, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText191 = new wxStaticText(m_panel169, wxID_ANY, _("IN3"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_staticText191, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN3 = new wxStaticText(m_panel169, wxID_ANY, _("0.000"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN3, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN3units = new wxStaticText(m_panel169, wxID_ANY, _("V"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN3units, 0, wxALL, WXC_FROM_DIP(5));
    
    m_statictest56 = new wxStaticText(m_panel169, wxID_ANY, _("IN4"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_statictest56, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN4 = new wxStaticText(m_panel169, wxID_ANY, _("0.000"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN4, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN4units = new wxStaticText(m_panel169, wxID_ANY, _("V"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN4units, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText203 = new wxStaticText(m_panel169, wxID_ANY, _("IN5"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_staticText203, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN5 = new wxStaticText(m_panel169, wxID_ANY, _("0.000"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN5, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN5units = new wxStaticText(m_panel169, wxID_ANY, _("V"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN5units, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText209 = new wxStaticText(m_panel169, wxID_ANY, _("IN6"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_staticText209, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN6 = new wxStaticText(m_panel169, wxID_ANY, _("0.000"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN6, 0, wxALL, WXC_FROM_DIP(5));
    
    m_IN6units = new wxStaticText(m_panel169, wxID_ANY, _("V"), wxDefaultPosition, wxDLG_UNIT(m_panel169, wxSize(-1,-1)), 0);
    
    gridSizer171->Add(m_IN6units, 0, wxALL, WXC_FROM_DIP(5));
    
    gridSizer97 = new wxGridSizer(0, 4, 0, 0);
    
    boxSizer69->Add(gridSizer97, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_pauseButton = new wxToggleButton(this, wxID_ANY, _("Pause"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    m_pauseButton->SetValue(false);
    
    gridSizer97->Add(m_pauseButton, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_timer165 = new wxTimer;
    m_timer165->Start(1000, false);
    
    SetBackgroundColour(wxColour(wxT("rgb(16,16,16)")));
    SetForegroundColour(wxColour(wxT("rgb(62,241,15)")));
    SetName(wxT("ICPCON7017PanelBase"));
    SetSize(700,500);
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    // Connect events
    m_timer165->Connect(wxEVT_TIMER, wxTimerEventHandler(ICPCON7017PanelBase::onProcessTimer), NULL, this);
    
}

ICPCON7017PanelBase::~ICPCON7017PanelBase()
{
    m_timer165->Disconnect(wxEVT_TIMER, wxTimerEventHandler(ICPCON7017PanelBase::onProcessTimer), NULL, this);
    
    m_timer165->Stop();
    wxDELETE( m_timer165 );

}
