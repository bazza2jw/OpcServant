///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __myframe__
#define __myframe__

#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_PEOPLE 1000
#define ID_APPOINT 1001
#define ID_DEPT 1002
#define ID_WORKER 1003
#define ID_TEST 1004

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame
///////////////////////////////////////////////////////////////////////////////
class MyFrame : public wxFrame 
{
	private:
	
	protected:
		wxStatusBar* m_statusBar1;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPeople( wxCommandEvent& event );
		virtual void OnAppointment( wxCommandEvent& event ) ;
		virtual void OnDepartment( wxCommandEvent& event ) ;
		virtual void OnWorker( wxCommandEvent& event ) ;
		virtual void OnTest( wxCommandEvent& event );
		
	
	public:
		
		MyFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Sample"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 514,305 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MyFrame();
	
		void Init();
};

#endif //__noname__
