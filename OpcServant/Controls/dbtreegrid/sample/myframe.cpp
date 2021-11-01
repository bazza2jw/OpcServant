///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "myframe.h"

///////////////////////////////////////////////////////////////////////////

MyFrame::MyFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, ID_PEOPLE, wxString( wxT("People") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );
	
	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu1, ID_APPOINT, wxString( wxT("Appointment") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem2 );
	
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu1, ID_DEPT, wxString( wxT("Department") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem3 );
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu1, ID_WORKER, wxString( wxT("Worker") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem4 );
	
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu1, ID_TEST, wxString( wxT("Test") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem5 );
	
	m_menubar1->Append( m_menu1, wxT("Main") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( m_menuItem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnPeople ) );
	this->Connect( m_menuItem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnAppointment ) );
	this->Connect( m_menuItem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnDepartment ) );
	this->Connect( m_menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnWorker ) );
	this->Connect( m_menuItem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnTest ) );
}

MyFrame::~MyFrame()
{
	// Disconnect Events
	this->Disconnect( ID_PEOPLE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnPeople ) );
	this->Disconnect( ID_APPOINT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnAppointment ) );
	this->Disconnect( ID_DEPT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnDepartment ) );
	this->Disconnect( ID_WORKER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnWorker ) );
	this->Disconnect( ID_TEST, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MyFrame::OnTest ) );
	
}
