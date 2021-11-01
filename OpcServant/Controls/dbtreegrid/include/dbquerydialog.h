///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 17 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dbquerydialog__
#define __dbquerydialog__

#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include "dbtreegrid.h"
///////////////////////////////////////////////////////////////////////////


/**
@class DbQueryDialog
Диалог ввода данных для поиска и для запроса
*/


class DbQueryDialog : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* bSizer1;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		wxStaticText* m_staticText1;
	
	
	public:
		///Грид для ввода данных для поиска
		wxDbTreeGrid * tree;
		
		DbQueryDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("?????/??????"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~DbQueryDialog();

		void Init();
		void OnSize(wxSizeEvent& event );
		///Получение строки с данными для запроса или поиска
		wxArrayString GetQueryData();
		wxTextCtrl* where;
	
};

#endif //__dbquerydialog__
