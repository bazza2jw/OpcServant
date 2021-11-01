///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "appointdialog.h"

///////////////////////////////////////////////////////////////////////////

AppointDialog::AppointDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	tree = new wxDbTreeGrid( this,wxID_ANY,
	wxDefaultPosition,
	wxSize(-1,-1),
	wxTR_SINGLE |
	wxTR_HAS_BUTTONS |
	wxTR_FULL_ROW_HIGHLIGHT |
	wxTR_HIDE_ROOT |
	wxTR_ROW_LINES |
	wxTR_COLUMN_LINES |
	wxTR_EDIT_LABELS |
	wxTR_NO_LINES
	);
	
	bSizer1->Add( tree, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AppointDialog::OnClose ) );
}

AppointDialog::~AppointDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AppointDialog::OnClose ) );
	
}
 