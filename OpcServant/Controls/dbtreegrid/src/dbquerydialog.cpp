///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../include/dbquerydialog.h"

///////////////////////////////////////////////////////////////////////////

DbQueryDialog::DbQueryDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,250 ), wxSize( -1,250 ) );
	
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	tree = new wxDbTreeGrid( this,wxID_ANY,
	wxDefaultPosition,
	wxSize(-1,50),
	wxTR_NO_LINES |
	wxTR_SINGLE |
	wxTR_HAS_BUTTONS |
	wxTR_FULL_ROW_HIGHLIGHT |
	wxTR_HIDE_ROOT |
	wxTR_ROW_LINES |
	wxTR_COLUMN_LINES |
	wxTR_EDIT_LABELS
	);
	
	tree->SetMinSize( wxSize( -1,80 ) );
	
	bSizer1->Add( tree, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("Where"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer1->Add( m_staticText1, 0, wxALL, 5 );
	
	where = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	where->SetMinSize( wxSize( -1,40 ) );
	
	bSizer1->Add( where, 1, wxALL|wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	bSizer1->Add( m_sdbSizer1, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( DbQueryDialog::OnSize ) );
}

DbQueryDialog::~DbQueryDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( DbQueryDialog::OnSize ) );
	
}
