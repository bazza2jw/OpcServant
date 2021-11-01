///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __appointdialog__
#define __appointdialog__

#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include "../include/dbtreegrid.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AppointDialog
///////////////////////////////////////////////////////////////////////////////
class AppointDialog : public wxDialog 
{
	private:
	
	protected:
		
	
	
	public:
		wxDbTreeGrid * tree;
		
		AppointDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Appointment"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,299 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~AppointDialog();
		DatabaseLayer* db;

		///Дополнительная инициализация
		bool Init();
		///Обработчик закрытия
		void OnClose(wxCloseEvent &event);

		wxDbDispatcher * dispatcher;
		DECLARE_EVENT_TABLE()	
};

#endif //__appointdialog__
