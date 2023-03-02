//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: HarnessUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "HarnessUI.h"


// Declare the bitmap loading function
extern void wxCrafterwLO2oXInitBitmapResources();

static bool bBitmapLoaded = false;


HarnessTestDialogBase::HarnessTestDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterwLO2oXInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer3 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer3);
    
    m_panel5 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    boxSizer3->Add(m_panel5, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    boxSizer15 = new wxBoxSizer(wxVERTICAL);
    m_panel5->SetSizer(boxSizer15);
    
    dateF = new DateEntry(m_panel5,wxID_ANY);
    boxSizer15->Add(dateF, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    doubleF = new DoubleEntry(m_panel5,wxID_ANY);
    boxSizer15->Add(doubleF, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    intF = new IntEntry(m_panel5,wxID_ANY);
    boxSizer15->Add(intF, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    textF=new TextEntry(m_panel5,wxID_ANY);
    boxSizer15->Add(textF, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    timeF = new TimeEntry(m_panel5,wxID_ANY);
    boxSizer15->Add(timeF, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_stdBtnSizer9 = new wxStdDialogButtonSizer();
    
    boxSizer3->Add(m_stdBtnSizer9, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button11 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer9->AddButton(m_button11);
    
    m_button13 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer9->AddButton(m_button13);
    m_stdBtnSizer9->Realize();
    
    SetName(wxT("HarnessTestDialogBase"));
    SetMinClientSize(wxSize(500,300));
    SetSize(wxDLG_UNIT(this, wxSize(800,600)));
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
    m_button11->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HarnessTestDialogBase::onOk), NULL, this);
    m_button13->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HarnessTestDialogBase::onCancel), NULL, this);
    
}

HarnessTestDialogBase::~HarnessTestDialogBase()
{
    m_button11->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HarnessTestDialogBase::onOk), NULL, this);
    m_button13->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HarnessTestDialogBase::onCancel), NULL, this);
    
}