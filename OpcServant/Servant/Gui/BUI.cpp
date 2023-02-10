//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: BUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "BUI.h"


// Declare the bitmap loading function
extern void wxCrafterW2Fln4InitBitmapResources();

static bool bBitmapLoaded = false;


MainframeBase::MainframeBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterW2Fln4InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer15 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer15);
    
    m_notebook = new wxToolbook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTBK_HORZ_LAYOUT|wxTBK_BUTTONBAR|wxBK_BOTTOM|wxBK_DEFAULT);
    m_notebook->SetName(wxT("m_notebook"));
    wxImageList* m_notebook_il = new wxImageList(16, 16);
    m_notebook->AssignImageList(m_notebook_il);
    
    boxSizer15->Add(m_notebook, 1, wxALL|wxEXPAND, WXC_FROM_DIP(1));
    
    m_panelWeb = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    int m_panelWebImgIndex;
    m_panelWebImgIndex = m_notebook_il->Add(wxXmlResource::Get()->LoadBitmap(wxT("monitor")));
    m_notebook->AddPage(m_panelWeb, wxT(""), true, m_panelWebImgIndex);
    
    webSizer = new wxBoxSizer(wxVERTICAL);
    m_panelWeb->SetSizer(webSizer);
    
    m_urlToolBar = new wxToolBar(m_panelWeb, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelWeb, wxSize(-1,-1)), wxTB_FLAT);
    m_urlToolBar->SetToolBitmapSize(wxSize(16,16));
    
    webSizer->Add(m_urlToolBar, 0, wxALL|wxEXPAND, WXC_FROM_DIP(1));
    
    m_urlToolBar->AddTool(wxID_ANY, _("Home"), wxArtProvider::GetBitmap(wxART_GO_HOME, wxART_TOOLBAR, wxDefaultSize), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_urlToolBar->AddTool(wxID_ANY, _("Back"), wxArtProvider::GetBitmap(wxART_GO_BACK, wxART_TOOLBAR, wxDefaultSize), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    
    m_webURL = new wxTextCtrl(m_urlToolBar, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_urlToolBar, wxSize(300,-1)), wxTE_PROCESS_ENTER);
    #if wxVERSION_NUMBER >= 3000
    m_webURL->SetHint(wxT(""));
    #endif
    m_urlToolBar->AddControl(m_webURL);
    
    m_urlToolBar->AddTool(wxID_ANY, _("Go"), wxArtProvider::GetBitmap(wxART_FIND, wxART_TOOLBAR, wxDefaultSize), wxNullBitmap, wxITEM_NORMAL, wxT(""), wxT(""), NULL);
    m_urlToolBar->Realize();
    
    #if wxUSE_WEBVIEW
    m_webView = wxWebView::New(m_panelWeb, wxID_ANY, _("about:blank"), wxDefaultPosition, wxDLG_UNIT(m_panelWeb, wxSize(-1,-1)), wxWebViewBackendDefault, 0);
    
    webSizer->Add(m_webView, 1, wxALL|wxEXPAND, WXC_FROM_DIP(1));
    #endif // wxUSE_WEBVIEW
    
    m_navigationTab = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    int m_navigationTabImgIndex;
    m_navigationTabImgIndex = m_notebook_il->Add(wxXmlResource::Get()->LoadBitmap(wxT("cog")));
    m_notebook->AddPage(m_navigationTab, wxT(""), false, m_navigationTabImgIndex);
    
    boxSizer25 = new wxBoxSizer(wxVERTICAL);
    m_navigationTab->SetSizer(boxSizer25);
    
    m_splitter111 = new wxSplitterWindow(m_navigationTab, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_navigationTab, wxSize(-1,-1)), wxSP_3D);
    m_splitter111->SetSashGravity(0.5);
    m_splitter111->SetMinimumPaneSize(10);
    
    boxSizer25->Add(m_splitter111, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_splitterPage115 = new wxPanel(m_splitter111, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_splitter111, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    
    boxSizer121 = new wxBoxSizer(wxVERTICAL);
    m_splitterPage115->SetSizer(boxSizer121);
    
    m_Navigation = new wxDataViewTreeCtrl(m_splitterPage115, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_splitterPage115, wxSize(-1,-1)), wxDV_ROW_LINES|wxDV_SINGLE);
    wxFont m_NavigationFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Noto Sans"));
    m_Navigation->SetFont(m_NavigationFont);
    m_Navigation->SetToolTip(_("Navigate the configuration"));
    m_Navigation->Enable(false);
    
    boxSizer121->Add(m_Navigation, 5, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    gridSizer175 = new wxGridSizer(0, 4, 0, 0);
    
    boxSizer121->Add(gridSizer175, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_unlockNavigator = new wxButton(m_splitterPage115, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_splitterPage115, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 2904
    m_unlockNavigator->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("key")), wxLEFT);
    m_unlockNavigator->SetBitmapMargins(2,2);
    #endif
    m_unlockNavigator->SetToolTip(_("Unlock access to the navigator (expert only)"));
    
    gridSizer175->Add(m_unlockNavigator, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_buttonSysProps = new wxButton(m_splitterPage115, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_splitterPage115, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 2904
    m_buttonSysProps->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("emblem-system")), wxLEFT);
    m_buttonSysProps->SetBitmapMargins(2,2);
    #endif
    
    gridSizer175->Add(m_buttonSysProps, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_aliasConfigure = new wxButton(m_splitterPage115, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_splitterPage115, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 2904
    m_aliasConfigure->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("vcs-update-required")), wxLEFT);
    m_aliasConfigure->SetBitmapMargins(2,2);
    #endif
    m_aliasConfigure->SetToolTip(_("Alias Configure"));
    
    gridSizer175->Add(m_aliasConfigure, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_diagnosticPage = new wxPanel(m_splitter111, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_splitter111, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_splitter111->SplitVertically(m_splitterPage115, m_diagnosticPage, 0);
    
    boxSizer125 = new wxBoxSizer(wxVERTICAL);
    m_diagnosticPage->SetSizer(boxSizer125);
    
    m_notebook129 = new wxNotebook(m_diagnosticPage, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_diagnosticPage, wxSize(-1,-1)), wxBK_BOTTOM|wxBK_LEFT|wxBK_DEFAULT);
    wxFont m_notebook129Font(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Noto Sans"));
    m_notebook129->SetFont(m_notebook129Font);
    m_notebook129->SetName(wxT("m_notebook129"));
    wxImageList* m_notebook129_il = new wxImageList(22, 22);
    m_notebook129->AssignImageList(m_notebook129_il);
    
    boxSizer125->Add(m_notebook129, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelConfiguration = new wxPanel(m_notebook129, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook129, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    int m_panelConfigurationImgIndex;
    m_panelConfigurationImgIndex = m_notebook129_il->Add(wxXmlResource::Get()->LoadBitmap(wxT("configure")));
    m_notebook129->AddPage(m_panelConfiguration, _("Configuration"), false, m_panelConfigurationImgIndex);
    
    boxSizer137 = new wxBoxSizer(wxVERTICAL);
    m_panelConfiguration->SetSizer(boxSizer137);
    
    m_Configuration = new wxHtmlWindow(m_panelConfiguration, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelConfiguration, wxSize(-1,-1)), wxHW_SCROLLBAR_AUTO);
    m_Configuration->SetPage(wxT("<b>Configuration</b>"));
    
    boxSizer137->Add(m_Configuration, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelMeasure = new wxPanel(m_notebook129, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook129, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    int m_panelMeasureImgIndex;
    m_panelMeasureImgIndex = m_notebook129_il->Add(wxXmlResource::Get()->LoadBitmap(wxT("newdifferential")));
    m_notebook129->AddPage(m_panelMeasure, _("Measure"), false, m_panelMeasureImgIndex);
    
    boxSizer141 = new wxBoxSizer(wxVERTICAL);
    m_panelMeasure->SetSizer(boxSizer141);
    
    m_Measure = new wxHtmlWindow(m_panelMeasure, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelMeasure, wxSize(-1,-1)), wxHW_SCROLLBAR_AUTO);
    m_Measure->SetPage(wxT("<b>Measure</b>"));
    
    boxSizer141->Add(m_Measure, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelPublish = new wxPanel(m_notebook129, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook129, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    int m_panelPublishImgIndex;
    m_panelPublishImgIndex = m_notebook129_il->Add(wxXmlResource::Get()->LoadBitmap(wxT("newfunction")));
    m_notebook129->AddPage(m_panelPublish, _("Publish"), false, m_panelPublishImgIndex);
    
    boxSizer145 = new wxBoxSizer(wxVERTICAL);
    m_panelPublish->SetSizer(boxSizer145);
    
    m_Publish = new wxHtmlWindow(m_panelPublish, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_panelPublish, wxSize(-1,-1)), wxHW_SCROLLBAR_AUTO);
    m_Publish->SetPage(wxT("<b>Publish</b>"));
    
    boxSizer145->Add(m_Publish, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_messagePanel = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    int m_messagePanelImgIndex;
    m_messagePanelImgIndex = m_notebook_il->Add(wxXmlResource::Get()->LoadBitmap(wxT("application_side_list")));
    m_notebook->AddPage(m_messagePanel, wxT(""), false, m_messagePanelImgIndex);
    
    boxSizer31 = new wxBoxSizer(wxVERTICAL);
    m_messagePanel->SetSizer(boxSizer31);
    
    wxArrayString m_messagesArr;
    m_messages = new wxListBox(m_messagePanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_messagePanel, wxSize(-1,-1)), m_messagesArr, wxLB_SINGLE);
    wxFont m_messagesFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Noto Sans"));
    m_messages->SetFont(m_messagesFont);
    
    boxSizer31->Add(m_messages, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    gridSizer35 = new wxGridSizer(0, 2, 0, 0);
    
    boxSizer31->Add(gridSizer35, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_buttonClearMessages = new wxButton(m_messagePanel, wxID_ANY, _("Clear"), wxDefaultPosition, wxDLG_UNIT(m_messagePanel, wxSize(-1,-1)), 0);
    m_buttonClearMessages->SetToolTip(_("Clear Message Display"));
    
    gridSizer35->Add(m_buttonClearMessages, 0, wxALL, WXC_FROM_DIP(5));
    
    m_timer77 = new wxTimer;
    m_timer77->Start(1000, true);
    
    m_timerPeriodic = new wxTimer;
    m_timerPeriodic->Start(1000, false);
    
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_notebook)){
        wxPersistenceManager::Get().RegisterAndRestore(m_notebook);
    } else {
        wxPersistenceManager::Get().Restore(m_notebook);
    }
    #endif
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_notebook129)){
        wxPersistenceManager::Get().RegisterAndRestore(m_notebook129);
    } else {
        wxPersistenceManager::Get().Restore(m_notebook129);
    }
    #endif
    
    SetName(wxT("MainframeBase"));
    SetSize(wxDLG_UNIT(this, wxSize(700,480)));
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
    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainframeBase::onClose), NULL, this);
    this->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainframeBase::onHome), NULL, this);
    this->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainframeBase::onBack), NULL, this);
    m_webURL->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainframeBase::onUrlEnter), NULL, this);
    this->Connect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainframeBase::onGo), NULL, this);
    #if wxUSE_WEBVIEW
    
    #endif // wxUSE_WEBVIEW
    m_Navigation->Connect(wxEVT_COMMAND_DATAVIEW_ITEM_CONTEXT_MENU, wxDataViewEventHandler(MainframeBase::OnContextMenu), NULL, this);
    m_Navigation->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainframeBase::OnSelChanged), NULL, this);
    m_unlockNavigator->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainframeBase::onUnlockNavigator), NULL, this);
    m_buttonSysProps->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainframeBase::onProperties), NULL, this);
    m_aliasConfigure->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainframeBase::OnAliasConfigure), NULL, this);
    m_buttonClearMessages->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainframeBase::OnClearMessages), NULL, this);
    m_timer77->Connect(wxEVT_TIMER, wxTimerEventHandler(MainframeBase::OnStartupTimer), NULL, this);
    m_timerPeriodic->Connect(wxEVT_TIMER, wxTimerEventHandler(MainframeBase::onPeriodicTimer), NULL, this);
    
}

MainframeBase::~MainframeBase()
{
    this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainframeBase::onClose), NULL, this);
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainframeBase::onHome), NULL, this);
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainframeBase::onBack), NULL, this);
    m_webURL->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainframeBase::onUrlEnter), NULL, this);
    this->Disconnect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainframeBase::onGo), NULL, this);
    #if wxUSE_WEBVIEW
    
    #endif // wxUSE_WEBVIEW
    m_Navigation->Disconnect(wxEVT_COMMAND_DATAVIEW_ITEM_CONTEXT_MENU, wxDataViewEventHandler(MainframeBase::OnContextMenu), NULL, this);
    m_Navigation->Disconnect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainframeBase::OnSelChanged), NULL, this);
    m_unlockNavigator->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainframeBase::onUnlockNavigator), NULL, this);
    m_buttonSysProps->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainframeBase::onProperties), NULL, this);
    m_aliasConfigure->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainframeBase::OnAliasConfigure), NULL, this);
    m_buttonClearMessages->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainframeBase::OnClearMessages), NULL, this);
    m_timer77->Disconnect(wxEVT_TIMER, wxTimerEventHandler(MainframeBase::OnStartupTimer), NULL, this);
    m_timerPeriodic->Disconnect(wxEVT_TIMER, wxTimerEventHandler(MainframeBase::onPeriodicTimer), NULL, this);
    
    m_timer77->Stop();
    wxDELETE( m_timer77 );

    m_timerPeriodic->Stop();
    wxDELETE( m_timerPeriodic );

}

ImageList::ImageList()
    : wxImageList(16, 16, true)
    , m_imagesWidth(16)
    , m_imagesHeight(16)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterW2Fln4InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("m_bmp553"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())){
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("m_bmp553"), bmp));
        }
    }
    
    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("m_bmp555"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())){
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("m_bmp555"), bmp));
        }
    }
    
    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("m_bmp557"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())){
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("m_bmp557"), bmp));
        }
    }
    
}

ImageList::~ImageList()
{
}

MainFrameToolBoxBase::MainFrameToolBoxBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterW2Fln4InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer541 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer541);
    
    m_toolbook543 = new wxToolbook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxTBK_HORZ_LAYOUT|wxBK_BOTTOM|wxBK_DEFAULT);
    m_toolbook543->SetName(wxT("m_toolbook543"));
    wxImageList* m_toolbook543_il = new wxImageList(16, 16);
    m_toolbook543->AssignImageList(m_toolbook543_il);
    
    boxSizer541->Add(m_toolbook543, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panel545 = new wxPanel(m_toolbook543, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_toolbook543, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    int m_panel545ImgIndex;
    m_panel545ImgIndex = m_toolbook543_il->Add(wxXmlResource::Get()->LoadBitmap(wxT("cog")));
    m_toolbook543->AddPage(m_panel545, wxT(""), false, m_panel545ImgIndex);
    
    boxSizer547 = new wxBoxSizer(wxVERTICAL);
    m_panel545->SetSizer(boxSizer547);
    
    m_panel549 = new wxPanel(m_toolbook543, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_toolbook543, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    int m_panel549ImgIndex;
    m_panel549ImgIndex = m_toolbook543_il->Add(wxXmlResource::Get()->LoadBitmap(wxT("monitor")));
    m_toolbook543->AddPage(m_panel549, wxT(""), false, m_panel549ImgIndex);
    
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_toolbook543)){
        wxPersistenceManager::Get().RegisterAndRestore(m_toolbook543);
    } else {
        wxPersistenceManager::Get().Restore(m_toolbook543);
    }
    #endif
    
    SetName(wxT("MainFrameToolBoxBase"));
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
}

MainFrameToolBoxBase::~MainFrameToolBoxBase()
{
}
