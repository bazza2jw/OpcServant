///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __customlovdialog__
#define __customlovdialog__

#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include "dbtreegrid.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DbCustomLOVDialog
///////////////////////////////////////////////////////////////////////////////
class CustomLOVDialog : public wxDialog 
{
	private:
	
	protected:
	
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) ;
		
	
	public:
		
		CustomLOVDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 795,630 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~CustomLOVDialog();
	
		DatabaseLayer* db;
		wxDbTreeGrid * tree;
		///Дополнительная инициализация
		bool Init( DatabaseLayer* d );
		///Обработчик событий от грида
		void OnRecHandle(wxDbRecEvent &event);
		///Обработчик нажатия двойной кнопки мыши
		void OnMouseDoubleClick(wxMouseEvent& event);
		void OnKey(wxKeyEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif //__customlovdialog__
