///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dblovdialog__
#define __dblovdialog__

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include "dbtreegrid.h"

/**
@class DbLovDialog
Класс диалог списка значений для поля
*/
///Класс диалог списка значений для поля
class DbLovDialog : public wxDialog 
{
	private:
	
	protected:

		wxPanel* m_panel1;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;
	
	public:
		///Поле вода строки для поиска
		wxTextCtrl* ftext;
		///Грид с данными 
		wxDbTreeGrid * tree;
		
		DbLovDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style =  wxCAPTION|wxRESIZE_BORDER  );
		~DbLovDialog();
		
		//----------------------
    protected:
	void OnQueryEnter(wxCommandEvent &event);
	void OnChoice(wxMouseEvent &event);

	//Обработчик нажатия двойной кнопки мыши
	void OnMouseDoubleClick(wxMouseEvent& event);
	void OnKey(wxKeyEvent& event);
	public:
	///Инициализация списка
	void Setup(wxDbLOV &lov);
	void OnClose(wxCloseEvent &event);
	public:
	DECLARE_EVENT_TABLE()

};

#endif //__dblovdialog__
