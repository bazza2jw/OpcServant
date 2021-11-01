///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __person__
#define __person__

#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

#include "../include/dbtreegrid.h"
#include "../include/dbdispatcher.h"
#include "../include/validator.h"
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PersonDialog
///////////////////////////////////////////////////////////////////////////////
class PersonDialog : public wxDialog 
{
	private:
	
	protected:
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event );
		
	
	public:
		wxDbTreeGrid * tree;
		
		PersonDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("People"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 630,440 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~PersonDialog();
	
	
		bool Init();
		wxDbDispatcher * dispatcher;
		
};

#endif //__person__
