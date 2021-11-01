///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "worker.h"

///////////////////////////////////////////////////////////////////////////

WorkerDialog::WorkerDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Department") ), wxVERTICAL );
	
	dept= new wxDbTreeGrid( this,wxID_ANY,
	wxDefaultPosition,
	wxSize(-1,-1),
	wxTR_SINGLE |
	wxTR_HAS_BUTTONS |
	wxTR_FULL_ROW_HIGHLIGHT |
	wxTR_HIDE_ROOT |
	wxTR_ROW_LINES |
	wxTR_COLUMN_LINES |
	wxTR_EDIT_LABELS
	);
	
	sbSizer1->Add( dept, 0, wxALL|wxEXPAND, 5 );
	
	bSizer1->Add( sbSizer1, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Worker") ), wxVERTICAL );
	
	worker= new wxDbTreeGrid( this,wxID_ANY,
	wxDefaultPosition,
	wxSize(-1,-1),
	wxTR_SINGLE |
	wxTR_HAS_BUTTONS |
	wxTR_FULL_ROW_HIGHLIGHT |
	wxTR_HIDE_ROOT |
	wxTR_ROW_LINES |
	wxTR_COLUMN_LINES |
	wxTR_EDIT_LABELS
	);
	
	sbSizer2->Add( worker, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Appointment*"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer2->Add( m_staticText1, 0, wxALL, 5 );
	
	appoint= new wxDbTextField( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1,-1),0);
	bSizer2->Add( appoint, 1, wxALL, 5 );
	
	always= new wxDbCheckField( this, wxID_ANY, wxT("Constantly"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer2->Add( always, 0, wxALL, 5 );
	
	sbSizer2->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	sbSizer2->Add( fgSizer1, 0, wxEXPAND, 5 );
	
	bSizer1->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( WorkerDialog::OnClose ) );
}

WorkerDialog::~WorkerDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( WorkerDialog::OnClose ) );
	
}
