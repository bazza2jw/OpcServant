///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __testfielddialog__
#define __testfielddialog__

#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/datectrl.h>

#include "../include/dbstruct.h"
#include "../include/dbtextfield.h"
#include "../include/dbtreegrid.h"
#include "../include/dbdatefield.h"
#include "../include/dbcheckfield.h"
#include "../include/dblistfield.h"
#include "../include/dbradioboxfield.h"




///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class TestFieldDialog
///////////////////////////////////////////////////////////////////////////////
class TestFieldDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText2;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText5;
		
	
	public:
		wxDbTreeGrid * tree;
		wxDbTextField *t1;
		wxDbDateField *d1;
		wxDbCheckField *c;
		wxDbListField *l1;
		wxDbRadioBoxField *r1;
		
		TestFieldDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Test"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 590,576 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~TestFieldDialog();

		DatabaseLayer* db;
		///Дополнительная инициализация
		bool Init();
		///Обработчик закрытия
		void OnClose(wxCloseEvent &event);

		wxDbDispatcher * dispatcher;
		DECLARE_EVENT_TABLE()	
};

#endif //__testfielddialog__
