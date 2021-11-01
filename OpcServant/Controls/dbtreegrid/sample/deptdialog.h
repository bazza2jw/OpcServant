///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __deptdialog__
#define __deptdialog__

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
class DeptDialog : public wxDialog 
{
	private:
	
	protected:
		
	
	
	public:
		wxDbTreeGrid * tree;
		
		DeptDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Department"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,299 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~DeptDialog();
		DatabaseLayer* db;

		///Дополнительная инициализация
		bool Init();
		///Обработчик закрытия
		void OnClose(wxCloseEvent &event);

		wxDbDispatcher * dispatcher;
		DECLARE_EVENT_TABLE()	
};

#endif //
