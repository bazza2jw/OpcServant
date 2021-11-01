/////////////////////////////////////////////////////////////////////////////
// Name:        dbdatefield.h
// Purpose:     Date field description
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __dbdatefield__
#define __dbdatefield__
#include "dbfield.h"
#include <wx/datectrl.h>
#include <wx/dateevt.h>

/**
@class wxDbTextField
Класс поле связанное с гридом поле для редактирования даты
*/
///Класс поле связанное с гридом поле для редактирования даты
class wxDbDateField :public wxDatePickerCtrl, public wxDbField{
public:
	wxDbDateField (wxWindow *parent, 
				wxWindowID id, 
				const wxDateTime& dt = wxDefaultDateTime, 
				const wxPoint& pos = wxDefaultPosition, 
				const wxSize& size = wxDefaultSize, 
				long style = wxDP_DEFAULT | wxDP_SHOWCENTURY | wxDP_DROPDOWN , 
				const wxValidator& validator = wxDefaultValidator, 
				const wxString& name = "datectrl");
	///Проверить значение поля
	virtual void SetValue(const wxString &v1, const wxString &v2 );
	virtual wxString GetValue1(){return value1;}
	virtual wxString GetValue2(){return value2;}
	virtual bool CheckValue();
	///Получить сылку на обработчик
	virtual wxEvtHandler* GetEvtHandler(){return  this;}

private:
	void OnRefreshField(wxDbFieldEvent& );
	void OnEnter(wxCommandEvent& );
	void OnKillFocus(wxFocusEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnChar(wxKeyEvent &event);
	//Обработчик нажатия правой кнопки мыши
	void OnMouseRightClick(wxMouseEvent& event);
	void OnEnter( wxDateEvent &event);
	void OnMouse(wxMouseEvent& event);


public:
	DECLARE_EVENT_TABLE()
};


#endif //__wxDbDateField__