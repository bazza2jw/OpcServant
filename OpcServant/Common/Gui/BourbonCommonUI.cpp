//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: BourbonCommonUI.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "BourbonCommonUI.h"


// Declare the bitmap loading function
extern void wxCrafterML0iT9InitBitmapResources();

static bool bBitmapLoaded = false;


AddObjectDialogBase::AddObjectDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    this->SetToolTip(_("Add Object to selected parent"));
    
    boxSizer3 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer3);
    
    flexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer5->SetFlexibleDirection( wxBOTH );
    flexGridSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer5->AddGrowableCol(1);
    
    boxSizer3->Add(flexGridSizer5, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText13 = new wxStaticText(this, wxID_ANY, _("Name"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText13, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_objectNameArr;
    m_objectName = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_objectNameArr, wxCB_SORT|wxCB_DROPDOWN);
    m_objectName->SetToolTip(_("New object name (must be unique to parent)"));
    #if wxVERSION_NUMBER >= 3000
    m_objectName->SetHint(wxT(""));
    #endif
    
    flexGridSizer5->Add(m_objectName, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText17 = new wxStaticText(this, wxID_ANY, _("Type"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer5->Add(m_staticText17, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_typeArr;
    m_type = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_typeArr, 0);
    m_type->SetToolTip(_("Object type"));
    
    flexGridSizer5->Add(m_type, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_stdBtnSizer7 = new wxStdDialogButtonSizer();
    
    boxSizer3->Add(m_stdBtnSizer7, 0, wxALL, WXC_FROM_DIP(5));
    
    m_buttonOk = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_buttonOk->SetDefault();
    m_stdBtnSizer7->AddButton(m_buttonOk);
    
    m_buttonCancel = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer7->AddButton(m_buttonCancel);
    m_stdBtnSizer7->Realize();
    
    SetName(wxT("AddObjectDialogBase"));
    SetMinClientSize(wxSize(500,300));
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
    m_objectName->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AddObjectDialogBase::OnEditChanged), NULL, this);
    m_buttonOk->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AddObjectDialogBase::OnOk), NULL, this);
    
}

AddObjectDialogBase::~AddObjectDialogBase()
{
    m_objectName->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(AddObjectDialogBase::OnEditChanged), NULL, this);
    m_buttonOk->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AddObjectDialogBase::OnOk), NULL, this);
    
}

EditObjectDialogBase::EditObjectDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer23 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer23);
    
    m_notebook31 = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxBK_DEFAULT);
    m_notebook31->SetName(wxT("m_notebook31"));
    
    boxSizer23->Add(m_notebook31, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelCommon = new wxPanel(m_notebook31, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook31, wxSize(800,600)), wxTAB_TRAVERSAL);
    m_notebook31->AddPage(m_panelCommon, _("Common"), false);
    
    CommonSizer = new wxBoxSizer(wxVERTICAL);
    m_panelCommon->SetSizer(CommonSizer);
    
    flexGridSizer59 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer59->SetFlexibleDirection( wxBOTH );
    flexGridSizer59->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer59->AddGrowableCol(1);
    
    CommonSizer->Add(flexGridSizer59, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText61 = new wxStaticText(m_panelCommon, wxID_ANY, _("Name"), wxDefaultPosition, wxDLG_UNIT(m_panelCommon, wxSize(-1,-1)), 0);
    
    flexGridSizer59->Add(m_staticText61, 0, wxALL, WXC_FROM_DIP(5));
    
    m_objectName = new wxStaticText(m_panelCommon, wxID_ANY, _("Object Name"), wxDefaultPosition, wxDLG_UNIT(m_panelCommon, wxSize(-1,-1)), 0);
    
    flexGridSizer59->Add(m_objectName, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText65 = new wxStaticText(m_panelCommon, wxID_ANY, _("Type"), wxDefaultPosition, wxDLG_UNIT(m_panelCommon, wxSize(-1,-1)), 0);
    
    flexGridSizer59->Add(m_staticText65, 0, wxALL, WXC_FROM_DIP(5));
    
    m_objectType = new wxStaticText(m_panelCommon, wxID_ANY, _("Object Type"), wxDefaultPosition, wxDLG_UNIT(m_panelCommon, wxSize(-1,-1)), 0);
    
    flexGridSizer59->Add(m_objectType, 0, wxALL, WXC_FROM_DIP(5));
    
    m_panelRecipe = new wxPanel(m_notebook31, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook31, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebook31->AddPage(m_panelRecipe, _("Recipe"), false);
    
    RecipeSizer = new wxBoxSizer(wxVERTICAL);
    m_panelRecipe->SetSizer(RecipeSizer);
    
    flexGridSizer41 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer41->SetFlexibleDirection( wxBOTH );
    flexGridSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer41->AddGrowableCol(1);
    
    RecipeSizer->Add(flexGridSizer41, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText43 = new wxStaticText(m_panelRecipe, wxID_ANY, _("Recipe"), wxDefaultPosition, wxDLG_UNIT(m_panelRecipe, wxSize(-1,-1)), 0);
    
    flexGridSizer41->Add(m_staticText43, 0, wxALL|wxALIGN_CENTER_VERTICAL, WXC_FROM_DIP(5));
    
    wxArrayString m_recipeArr;
    m_recipe = new wxComboBox(m_panelRecipe, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(m_panelRecipe, wxSize(-1,-1)), m_recipeArr, wxCB_SORT|wxCB_READONLY|wxCB_DROPDOWN);
    m_recipe->SetToolTip(_("Select the recipe to configure"));
    
    flexGridSizer41->Add(m_recipe, 0, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panelNotes = new wxPanel(m_notebook31, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook31, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebook31->AddPage(m_panelNotes, _("Notes"), false);
    
    boxSizer47 = new wxBoxSizer(wxVERTICAL);
    m_panelNotes->SetSizer(boxSizer47);
    
    m_notes = new wxTextCtrl(m_panelNotes, wxID_ANY, wxT("Notes"), wxDefaultPosition, wxDLG_UNIT(m_panelNotes, wxSize(-1,-1)), wxTE_MULTILINE);
    
    boxSizer47->Add(m_notes, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_stdBtnSizer25 = new wxStdDialogButtonSizer();
    
    boxSizer23->Add(m_stdBtnSizer25, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button29 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer25->AddButton(m_button29);
    
    m_button27 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer25->AddButton(m_button27);
    m_stdBtnSizer25->Realize();
    
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_notebook31)){
        wxPersistenceManager::Get().RegisterAndRestore(m_notebook31);
    } else {
        wxPersistenceManager::Get().Restore(m_notebook31);
    }
    #endif
    
    SetName(wxT("EditObjectDialogBase"));
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
    m_recipe->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(EditObjectDialogBase::OnRecipeSelectionChanged), NULL, this);
    m_button27->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditObjectDialogBase::OnOk), NULL, this);
    
}

EditObjectDialogBase::~EditObjectDialogBase()
{
    m_recipe->Disconnect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(EditObjectDialogBase::OnRecipeSelectionChanged), NULL, this);
    m_button27->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditObjectDialogBase::OnOk), NULL, this);
    
}

ConfigureI2cDialogBase::ConfigureI2cDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer81 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer81);
    
    gridSizer83 = new wxGridSizer(0, 2, 0, 0);
    
    boxSizer81->Add(gridSizer83, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText85 = new wxStaticText(this, wxID_ANY, _("I2C Type"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer83->Add(m_staticText85, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_i2cTypeArr;
    m_i2cType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_i2cTypeArr, 0);
    
    gridSizer83->Add(m_i2cType, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText89 = new wxStaticText(this, wxID_ANY, _("I2C Device / Address"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer83->Add(m_staticText89, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_i2cDeviceArr;
    m_i2cDevice = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_i2cDeviceArr, 0);
    #if wxVERSION_NUMBER >= 3000
    m_i2cDevice->SetHint(wxT(""));
    #endif
    
    gridSizer83->Add(m_i2cDevice, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText93 = new wxStaticText(this, wxID_ANY, _("I2C Network Port"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    gridSizer83->Add(m_staticText93, 0, wxALL, WXC_FROM_DIP(5));
    
    m_i2cPort = new wxSpinCtrl(this, wxID_ANY, wxT("5000"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_i2cPort->Enable(false);
    m_i2cPort->SetRange(100, 32000);
    m_i2cPort->SetValue(5000);
    
    gridSizer83->Add(m_i2cPort, 0, wxALL, WXC_FROM_DIP(5));
    
    m_stdBtnSizer97 = new wxStdDialogButtonSizer();
    
    boxSizer81->Add(m_stdBtnSizer97, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button99 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer97->AddButton(m_button99);
    
    m_button101 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer97->AddButton(m_button101);
    m_stdBtnSizer97->Realize();
    
    SetName(wxT("ConfigureI2cDialogBase"));
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
    m_i2cType->Connect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ConfigureI2cDialogBase::onTypeChanged), NULL, this);
    m_button101->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ConfigureI2cDialogBase::onOK), NULL, this);
    
}

ConfigureI2cDialogBase::~ConfigureI2cDialogBase()
{
    m_i2cType->Disconnect(wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler(ConfigureI2cDialogBase::onTypeChanged), NULL, this);
    m_button101->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ConfigureI2cDialogBase::onOK), NULL, this);
    
}

GridPanelBase::GridPanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    Sizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(Sizer);
    
    m_grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxWANTS_CHARS);
    wxFont m_gridFont(16, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Serif"));
    m_grid->SetFont(m_gridFont);
    m_grid->CreateGrid(0, 2);
    m_grid->SetColLabelValue(0, _("Values"));
    m_grid->SetColLabelValue(1, _("Status"));
    m_grid->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_grid->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_grid->UseNativeColHeader(false);
    #endif
    m_grid->EnableEditing(false);
    
    Sizer->Add(m_grid, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_updateTimer = new wxTimer;
    m_updateTimer->Start(2000, false);
    
    SetName(wxT("GridPanelBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    // Connect events
    m_updateTimer->Connect(wxEVT_TIMER, wxTimerEventHandler(GridPanelBase::onTimer), NULL, this);
    
}

GridPanelBase::~GridPanelBase()
{
    m_updateTimer->Disconnect(wxEVT_TIMER, wxTimerEventHandler(GridPanelBase::onTimer), NULL, this);
    
    m_updateTimer->Stop();
    wxDELETE( m_updateTimer );

}

SetupDialogBase::SetupDialogBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer117 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer117);
    
    m_notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxBK_DEFAULT);
    m_notebook->SetName(wxT("m_notebook"));
    
    boxSizer117->Add(m_notebook, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_panel121 = new wxPanel(m_notebook, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_notebook, wxSize(-1,-1)), wxTAB_TRAVERSAL);
    m_notebook->AddPage(m_panel121, _("Setup"), false);
    
    setupSizer = new wxFlexGridSizer(0, 2, 0, 0);
    setupSizer->SetFlexibleDirection( wxBOTH );
    setupSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    m_panel121->SetSizer(setupSizer);
    
    m_staticText131 = new wxStaticText(m_panel121, wxID_ANY, _("Measure Interval (s)"), wxDefaultPosition, wxDLG_UNIT(m_panel121, wxSize(-1,-1)), 0);
    
    setupSizer->Add(m_staticText131, 0, wxALL, WXC_FROM_DIP(5));
    
    m_measureInterval = new wxSpinCtrl(m_panel121, wxID_ANY, wxT("60"), wxDefaultPosition, wxDLG_UNIT(m_panel121, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_measureInterval->SetRange(10, 3600);
    m_measureInterval->SetValue(60);
    
    setupSizer->Add(m_measureInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText135 = new wxStaticText(m_panel121, wxID_ANY, _("Publish Interval (s)"), wxDefaultPosition, wxDLG_UNIT(m_panel121, wxSize(-1,-1)), 0);
    
    setupSizer->Add(m_staticText135, 0, wxALL, WXC_FROM_DIP(5));
    
    m_publishInterval = new wxSpinCtrl(m_panel121, wxID_ANY, wxT("60"), wxDefaultPosition, wxDLG_UNIT(m_panel121, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_publishInterval->SetRange(10, 3600);
    m_publishInterval->SetValue(60);
    
    setupSizer->Add(m_publishInterval, 0, wxALL, WXC_FROM_DIP(5));
    
    m_Enabled = new wxCheckBox(m_panel121, wxID_ANY, _("Enabled"), wxDefaultPosition, wxDLG_UNIT(m_panel121, wxSize(-1,-1)), 0);
    m_Enabled->SetValue(false);
    
    setupSizer->Add(m_Enabled, 0, wxALL, WXC_FROM_DIP(5));
    
    m_enableTabView = new wxCheckBox(m_panel121, wxID_ANY, _("Enable Tab View"), wxDefaultPosition, wxDLG_UNIT(m_panel121, wxSize(-1,-1)), 0);
    m_enableTabView->SetValue(false);
    
    setupSizer->Add(m_enableTabView, 0, wxALL, WXC_FROM_DIP(5));
    
    m_stdBtnSizer123 = new wxStdDialogButtonSizer();
    
    boxSizer117->Add(m_stdBtnSizer123, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button125 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer123->AddButton(m_button125);
    
    m_button127 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer123->AddButton(m_button127);
    m_stdBtnSizer123->Realize();
    
    
    #if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(m_notebook)){
        wxPersistenceManager::Get().RegisterAndRestore(m_notebook);
    } else {
        wxPersistenceManager::Get().Restore(m_notebook);
    }
    #endif
    
    SetName(wxT("SetupDialogBase"));
    SetSize(wxDLG_UNIT(this, wxSize(600,300)));
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
    m_button127->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SetupDialogBase::onOk), NULL, this);
    
}

SetupDialogBase::~SetupDialogBase()
{
    m_button127->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SetupDialogBase::onOk), NULL, this);
    
}

I2cConfigurePanelBase::I2cConfigurePanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer149 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer149);
    
    flexGridSizer151 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer151->SetFlexibleDirection( wxBOTH );
    flexGridSizer151->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    boxSizer149->Add(flexGridSizer151, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText153 = new wxStaticText(this, wxID_ANY, _("I2C Type"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer151->Add(m_staticText153, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_i2cTypeArr;
    m_i2cTypeArr.Add(wxT("Local"));
    m_i2cTypeArr.Add(wxT("Remote"));
    m_i2cType = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_i2cTypeArr, 0);
    m_i2cType->SetSelection(0);
    
    flexGridSizer151->Add(m_i2cType, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText157 = new wxStaticText(this, wxID_ANY, _("I2C Device / Address"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer151->Add(m_staticText157, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_i2cDeviceArr;
    m_i2cDevice = new wxComboBox(this, wxID_ANY, wxT("i2c-1"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_i2cDeviceArr, wxCB_SORT|wxCB_SIMPLE|wxCB_DROPDOWN);
    #if wxVERSION_NUMBER >= 3000
    m_i2cDevice->SetHint(wxT(""));
    #endif
    
    flexGridSizer151->Add(m_i2cDevice, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText161 = new wxStaticText(this, wxID_ANY, _("I2C Port"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer151->Add(m_staticText161, 0, wxALL, WXC_FROM_DIP(5));
    
    m_i2cPort = new wxSpinCtrl(this, wxID_ANY, wxT("5001"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_i2cPort->SetRange(1030, 30000);
    m_i2cPort->SetValue(5001);
    
    flexGridSizer151->Add(m_i2cPort, 0, wxALL, WXC_FROM_DIP(5));
    
    SetName(wxT("I2cConfigurePanelBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
}

I2cConfigurePanelBase::~I2cConfigurePanelBase()
{
}

SerialConfigurePanelBase::SerialConfigurePanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer173 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer173);
    
    flexGridSizer175 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer175->SetFlexibleDirection( wxBOTH );
    flexGridSizer175->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    boxSizer173->Add(flexGridSizer175, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText177 = new wxStaticText(this, wxID_ANY, _("Device"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer175->Add(m_staticText177, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_deviceArr;
    m_deviceArr.Add(wxT("ttyS0"));
    m_deviceArr.Add(wxT("ttyUSB0"));
    m_deviceArr.Add(wxT("ttyACM0"));
    m_device = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_deviceArr, 0);
    m_device->SetSelection(0);
    
    flexGridSizer175->Add(m_device, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText181 = new wxStaticText(this, wxID_ANY, _("Baudrate"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer175->Add(m_staticText181, 0, wxALL, WXC_FROM_DIP(5));
    
    wxArrayString m_baudrateArr;
    m_baudrateArr.Add(wxT("9600"));
    m_baudrateArr.Add(wxT("19200"));
    m_baudrateArr.Add(wxT("34800"));
    m_baudrateArr.Add(wxT("115200"));
    m_baudrate = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), m_baudrateArr, 0);
    m_baudrate->SetSelection(0);
    
    flexGridSizer175->Add(m_baudrate, 0, wxALL, WXC_FROM_DIP(5));
    
    m_nodeLabel = new wxStaticText(this, wxID_ANY, _("Node Address"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    m_nodeLabel->Hide();
    
    flexGridSizer175->Add(m_nodeLabel, 0, wxALL, WXC_FROM_DIP(5));
    
    m_nodeAddress = new wxSpinCtrl(this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_nodeAddress->Hide();
    m_nodeAddress->SetRange(0, 255);
    m_nodeAddress->SetValue(1);
    
    flexGridSizer175->Add(m_nodeAddress, 0, wxALL, WXC_FROM_DIP(5));
    
    SetName(wxT("SerialConfigurePanelBase"));
    SetSize(wxDLG_UNIT(this, wxSize(300,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
}

SerialConfigurePanelBase::~SerialConfigurePanelBase()
{
}

NetworkConfigurePanelBase::NetworkConfigurePanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    flexGridSizer193 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer193->SetFlexibleDirection( wxBOTH );
    flexGridSizer193->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer193->AddGrowableCol(1);
    this->SetSizer(flexGridSizer193);
    
    m_staticText195 = new wxStaticText(this, wxID_ANY, _("Address"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer193->Add(m_staticText195, 0, wxALL, WXC_FROM_DIP(5));
    
    m_address = new wxTextCtrl(this, wxID_ANY, wxT("localhost"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_address->SetHint(wxT(""));
    #endif
    
    flexGridSizer193->Add(m_address, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText199 = new wxStaticText(this, wxID_ANY, _("Port"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer193->Add(m_staticText199, 0, wxALL, WXC_FROM_DIP(5));
    
    m_port = new wxSpinCtrl(this, wxID_ANY, wxT("1024"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_port->SetRange(0, 30000);
    m_port->SetValue(1024);
    
    flexGridSizer193->Add(m_port, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button203 = new wxButton(this, wxID_ANY, _("Test ..."), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer193->Add(m_button203, 0, wxALL, WXC_FROM_DIP(5));
    
    SetName(wxT("NetworkConfigurePanelBase"));
    SetSize(wxDLG_UNIT(this, wxSize(300,200)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    // Connect events
    m_button203->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NetworkConfigurePanelBase::onTest), NULL, this);
    
}

NetworkConfigurePanelBase::~NetworkConfigurePanelBase()
{
    m_button203->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NetworkConfigurePanelBase::onTest), NULL, this);
    
}

DatabaseConfigurePanelBase::DatabaseConfigurePanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    flexGridSizer207 = new wxFlexGridSizer(0, 2, 0, 0);
    flexGridSizer207->SetFlexibleDirection( wxBOTH );
    flexGridSizer207->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer207->AddGrowableCol(1);
    this->SetSizer(flexGridSizer207);
    
    m_staticText209 = new wxStaticText(this, wxID_ANY, _("Host"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer207->Add(m_staticText209, 0, wxALL, WXC_FROM_DIP(5));
    
    m_host = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_host->SetHint(wxT(""));
    #endif
    
    flexGridSizer207->Add(m_host, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_staticText213 = new wxStaticText(this, wxID_ANY, _("Database"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer207->Add(m_staticText213, 0, wxALL, WXC_FROM_DIP(5));
    
    m_database = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_database->SetHint(wxT(""));
    #endif
    
    flexGridSizer207->Add(m_database, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText217 = new wxStaticText(this, wxID_ANY, _("Port"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer207->Add(m_staticText217, 0, wxALL, WXC_FROM_DIP(5));
    
    m_port = new wxSpinCtrl(this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxSP_ARROW_KEYS);
    m_port->SetRange(0, 30000);
    m_port->SetValue(0);
    
    flexGridSizer207->Add(m_port, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText221 = new wxStaticText(this, wxID_ANY, _("Username"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer207->Add(m_staticText221, 0, wxALL, WXC_FROM_DIP(5));
    
    m_username = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_username->SetHint(wxT(""));
    #endif
    
    flexGridSizer207->Add(m_username, 0, wxALL, WXC_FROM_DIP(5));
    
    m_staticText225 = new wxStaticText(this, wxID_ANY, _("Password"), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer207->Add(m_staticText225, 0, wxALL, WXC_FROM_DIP(5));
    
    m_password = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    #if wxVERSION_NUMBER >= 3000
    m_password->SetHint(wxT(""));
    #endif
    
    flexGridSizer207->Add(m_password, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button229 = new wxButton(this, wxID_ANY, _("Test ..."), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), 0);
    
    flexGridSizer207->Add(m_button229, 0, wxALL, WXC_FROM_DIP(5));
    
    SetName(wxT("DatabaseConfigurePanelBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
    // Connect events
    m_button229->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DatabaseConfigurePanelBase::onTest), NULL, this);
    
}

DatabaseConfigurePanelBase::~DatabaseConfigurePanelBase()
{
    m_button229->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DatabaseConfigurePanelBase::onTest), NULL, this);
    
}

ScaleOffsetConfigurePanelBase::ScaleOffsetConfigurePanelBase(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : wxPanel(parent, id, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer237 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer237);
    
    m_grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxWANTS_CHARS);
    m_grid->CreateGrid(0, 2);
    m_grid->SetColLabelValue(0, _("Scale"));
    m_grid->SetColSize(0, 150);
    m_grid->SetColLabelValue(1, _("Offset"));
    m_grid->SetColSize(1, 150);
    m_grid->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
    m_grid->SetColLabelAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    #if wxVERSION_NUMBER >= 2904
    m_grid->UseNativeColHeader(true);
    #endif
    m_grid->EnableEditing(true);
    
    boxSizer237->Add(m_grid, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    SetName(wxT("ScaleOffsetConfigurePanelBase"));
    SetSize(wxDLG_UNIT(this, wxSize(500,300)));
    if (GetSizer()) {
         GetSizer()->Fit(this);
    }
}

ScaleOffsetConfigurePanelBase::~ScaleOffsetConfigurePanelBase()
{
}

ConfigurationPropertySheetBase::ConfigurationPropertySheetBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    boxSizer247 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer247);
    
    wxArrayString m_PropertySheetArr;
    wxUnusedVar(m_PropertySheetArr);
    wxArrayInt m_PropertySheetIntArr;
    wxUnusedVar(m_PropertySheetIntArr);
    m_PropertySheet = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1,-1)), wxPG_DESCRIPTION|wxPG_SPLITTER_AUTO_CENTER|wxPG_BOLD_MODIFIED);
    
    boxSizer247->Add(m_PropertySheet, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    
    m_pgProp257 = m_PropertySheet->Append(  new wxStringProperty( _("My Label4"), wxPG_LABEL, wxT("")) );
    m_pgProp257->SetHelpString(wxT(""));
    
    m_stdBtnSizer251 = new wxStdDialogButtonSizer();
    
    boxSizer247->Add(m_stdBtnSizer251, 0, wxALL, WXC_FROM_DIP(5));
    
    m_button253 = new wxButton(this, wxID_CANCEL, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_stdBtnSizer251->AddButton(m_button253);
    
    m_button255 = new wxButton(this, wxID_OK, wxT(""), wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), 0);
    m_button255->SetDefault();
    m_stdBtnSizer251->AddButton(m_button255);
    m_stdBtnSizer251->Realize();
    
    SetName(wxT("ConfigurationPropertySheetBase"));
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
    m_button255->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ConfigurationPropertySheetBase::onOk), NULL, this);
    
}

ConfigurationPropertySheetBase::~ConfigurationPropertySheetBase()
{
    m_button255->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ConfigurationPropertySheetBase::onOk), NULL, this);
    
}

CommonImageList::CommonImageList()
    : wxImageList(32, 32, true)
    , m_imagesWidth(32)
    , m_imagesHeight(32)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCrafterML0iT9InitBitmapResources();
        bBitmapLoaded = true;
    }
    
    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("m_Default"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())){
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("m_Default"), bmp));
        }
    }
    
    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("m_Folder"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())){
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("m_Folder"), bmp));
        }
    }
    
}

CommonImageList::~CommonImageList()
{
}
