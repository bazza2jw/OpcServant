///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../include/dblovdialog.h"

///////////////////////////////////////////////////////////////////////////

DbLovDialog::DbLovDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	ftext = new wxTextCtrl( this, wxID_ANY, wxT("%"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer2->Add( ftext, 0, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	tree = new wxDbTreeGrid( this,wxID_ANY,
	wxDefaultPosition,
	wxSize(-1,-1),
	wxTR_NO_LINES |
	wxTR_SINGLE |
	wxTR_HAS_BUTTONS |
	wxTR_FULL_ROW_HIGHLIGHT |
	wxTR_HIDE_ROOT |
	wxTR_ROW_LINES |
	wxTR_COLUMN_LINES |
	wxTR_EDIT_LABELS
	);
	
	bSizer2->Add( tree, 1, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	bSizer2->Add( m_sdbSizer2, 0, wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	ftext->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DbLovDialog::OnQueryEnter ), NULL, this );
//	tree->Connect( wxEVT_MIDDLE_DCLICK, wxMouseEventHandler( DbLovDialog::OnChoice ), NULL, this );
}

DbLovDialog::~DbLovDialog()
{
	// Disconnect Events
	ftext->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( DbLovDialog::OnQueryEnter ), NULL, this );
//	tree->Disconnect( wxEVT_MIDDLE_DCLICK, wxMouseEventHandler( DbLovDialog::OnChoice ), NULL, this );
	
}


