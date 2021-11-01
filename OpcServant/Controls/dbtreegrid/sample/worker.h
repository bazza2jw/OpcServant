///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __worker__
#define __worker__

#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/dialog.h>

#include "../include/dbtreegrid.h"
#include "../include/dbtextfield.h"
#include "../include/dbcheckfield.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class WorkerDialog
///////////////////////////////////////////////////////////////////////////////
class WorkerDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxDbTextField *appoint;
		wxDbCheckField *always;
		
		
	
	public:
		wxDbTreeGrid * dept;
		wxDbTreeGrid * worker;
		
		
		WorkerDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("People"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 667,549 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~WorkerDialog();
		DatabaseLayer* db;

		///Дополнительная инициализация
		bool Init();
		///Обработчик закрытия
		void OnClose(wxCloseEvent &event);

		wxDbDispatcher * dispatcher;
		DECLARE_EVENT_TABLE()	
	
};

#endif //__worker__
