//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: MimicDisplayUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "MimicDisplayUI.h"


// Declare the bitmap loading function
extern void wxCrafteranM2fBInitBitmapResources();

static bool bBitmapLoaded = false;


MimicDisplayConfigurationDialogBase::MimicDisplayConfigurationDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafteranM2fBInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer3 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer3);
    
    gridSizer5 = new wxGridSizer(0, 2, 0, 0);
    
    boxSizer3->Add(gridSizer5, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText7 = new wxStaticText(this, wxID_ANY, _("Update Interval (s)"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer5->Add(m_staticText7, 0, wxALL, WXC_FROM_DIP(5));
    
    m_updateInterval = new wxSpinCtrl(this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_updateInterval->SetRange(2, 100);
    m_updateInterval->SetValue(2);
    
    gridSizer5->Add(m_updateInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText43 = new wxStaticText(this, wxID_ANY, _("Filename"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer5->Add(m_staticText43, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_fileNameArr;
    m_fileName = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_fileNameArr, 0);
    
    gridSizer5->Add(m_fileName, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_enabled = new wxCheckBox(this, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    m_enabled->SetValue(false);
    
    gridSizer5->Add(m_enabled, 0, wxALL, WXC_FROM_DIP(5));
    
    m_enableTabView = new wxCheckBox(this, wxID_ANY, _("Enable Tab View"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    m_enableTabView->SetValue(false);
    
    gridSizer5->Add(m_enableTabView, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button21 = new wxButton(this, wxID_ANY, _("Edit ..."), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer5->Add(m_button21, 0, wxALL, WXC_FROM_DIP(5));
    
    m_stdBtnSizer15 = new wxStdDialogButtonSizer();
    
    boxSizer3->Add(m_stdBtnSizer15, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button17 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer15->AddButton(m_button17);
    
    m_button19 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer15->AddButton(m_button19);
    m_stdBtnSizer15->Realize();
    
    SetName(wxT("MimicDisplayConfigurationDialogBase"));
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
    m_button21->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MimicDisplayConfigurationDialogBase::onEdit), NULL, this);
    m_button19->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MimicDisplayConfigurationDialogBase::onOk), NULL, this);
    
}

MimicDisplayConfigurationDialogBase::~MimicDisplayConfigurationDialogBase()
{
    m_button21->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MimicDisplayConfigurationDialogBase::onEdit), NULL, this);
    m_button19->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MimicDisplayConfigurationDialogBase::onOk), NULL, this);
    
}

MimicEditorDialogBase::MimicEditorDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafteranM2fBInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    canvasSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(canvasSizer);
    
    m_panelEditor = new MimicEditorPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    canvasSizer->Add(m_panelEditor, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_stdBtnSizer29 = new wxStdDialogButtonSizer();
    
    canvasSizer->Add(m_stdBtnSizer29, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button31 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer29->AddButton(m_button31);
    
    m_button33 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer29->AddButton(m_button33);
    m_stdBtnSizer29->Realize();
    
    SetName(wxT("MimicEditorDialogBase"));
    SetMinClientSize(wxSize(800,600));
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
    m_button33->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MimicEditorDialogBase::onOk), NULL, this);
    
}

MimicEditorDialogBase::~MimicEditorDialogBase()
{
    m_button33->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MimicEditorDialogBase::onOk), NULL, this);
    
}

MimicTabDisplayBase::MimicTabDisplayBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafteranM2fBInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    canvasSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(canvasSizer);
    
    m_toolbar47 = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTB_FLAT);
    m_toolbar47->SetToolBitmapSize(wxSize(16,16));
    
    canvasSizer->Add(m_toolbar47, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_toolbar47->AddTool(wxID_HOME, _("Home"), wxArtProvider::GetBitmap(wxART_GO_HOME, wxART_TOOLBAR, wxDefaultSize), wxNullBitmap, wxITEM_NORMAL, _("Reload Mimic"), wxT(""), NULL);
    
    m_toolbar47->AddTool(wxID_BACKWARD, _("Back"), wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_TOOLBAR, wxDefaultSize), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    m_toolbar47->Realize();
    
    m_timer = new wxTimer;
    m_timer->Start(3000, false);
    
    SetName(wxT("MimicTabDisplayBase"));
    SetSize(wxDLG_UNIT(this, wxSize(600,400)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    // Connect events
    this->Connect(wxID_HOME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MimicTabDisplayBase::onHome), NULL, this);
    this->Connect(wxID_BACKWARD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MimicTabDisplayBase::onBack), NULL, this);
    m_timer->Connect(wxEVT_TIMER, wxTimerEventHandler(MimicTabDisplayBase::onTimer), NULL, this);
    
}

MimicTabDisplayBase::~MimicTabDisplayBase()
{
    this->Disconnect(wxID_HOME, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MimicTabDisplayBase::onHome), NULL, this);
    this->Disconnect(wxID_BACKWARD, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MimicTabDisplayBase::onBack), NULL, this);
    m_timer->Disconnect(wxEVT_TIMER, wxTimerEventHandler(MimicTabDisplayBase::onTimer), NULL, this);
    
    m_timer->Stop();
    wxDELETE( m_timer );

}

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