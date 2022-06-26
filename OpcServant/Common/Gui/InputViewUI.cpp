//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: InputViewUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "InputViewUI.h"


// Declare the bitmap loading function
extern void wxCrafteranM2fBInitBitmapResources();

static bool bBitmapLoaded = false;


InputViewerBase::InputViewerBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafteranM2fBInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer55 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer55);
    
    m_tabs = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxBK_DEFAULT);
    m_tabs->SetName(wxT("m_tabs"));
    
    boxSizer55->Add(m_tabs, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelCurrent = new wxPanel(m_tabs, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_tabs, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_tabs->AddPage(m_panelCurrent, _("Current"), false);
    
    boxSizer63 = new wxBoxSizer(wxVERTICAL);
    m_panelCurrent->SetSizer(boxSizer63);
    
    m_currentStatus = new wxRichTextCtrl(m_panelCurrent, wxID_ANY, wxT("Waiting"), wxDefaultPosition, wxDLG_UNIT(m_panelCurrent, wxSize(-1,-1)), wxTE_MULTILINE|wxTE_PROCESS_TAB|wxTE_PROCESS_ENTER|wxWANTS_CHARS);
    
    boxSizer63->Add(m_currentStatus, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_timer83 = new wxTimer;
    m_timer83->Start(1000, false);
    
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_tabs)){
        wxPersistenceManager::Get().RegisterAndRestore(m_tabs);
    } else {
        wxPersistenceManager::Get().Restore(m_tabs);
    }
    #endif
    
    SetName(wxT("InputViewerBase"));
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
    m_timer83->Connect(wxEVT_TIMER, wxTimerEventHandler(InputViewerBase::DriveProcess), NULL, this);
    
}

InputViewerBase::~InputViewerBase()
{
    m_timer83->Disconnect(wxEVT_TIMER, wxTimerEventHandler(InputViewerBase::DriveProcess), NULL, this);
    
    m_timer83->Stop();
    wxDELETE( m_timer83 );

}

InputViewerHistoryBase::InputViewerHistoryBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafteranM2fBInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer75 = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(boxSizer75);
    
    m_graph = new MRL::GraphWindow(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    boxSizer75->Add(m_graph, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    wxArrayString m_historyArr;
    m_history = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_historyArr, wxLB_SINGLE);
    
    boxSizer75->Add(m_history, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    SetName(wxT("InputViewerHistoryBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
}

InputViewerHistoryBase::~InputViewerHistoryBase()
{
}