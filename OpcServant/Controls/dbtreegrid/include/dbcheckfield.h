/////////////////////////////////////////////////////////////////////////////
// Name:        dbcheckfield.h
// Purpose:     Check field description
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////


#ifndef __dbcheckfield__
#define __dbcheckfield__
#include "dbfield.h"
#include <wx/datectrl.h>
#include <wx/dateevt.h>

/**
@class wxDbCheckField
Класс поле связанное с гридом поле для редактирования checkBox
*/
///Класс поле связанное с гридом поле для редактирования checkBox
class wxDbCheckField :public wxCheckBox, public wxDbField{
public:
	///Конструктор
	wxDbCheckField(wxWindow* parent,
				wxWindowID id, 
				const wxString& label, 
				const wxPoint& pos = wxDefaultPosition, 
				const wxSize& size = wxDefaultSize, 
				long style = 0|wxCHK_3STATE, 
				const wxValidator& val=wxDefaultValidator, 
				const wxString& name = "checkBox");

;
	virtual void SetValue(const wxString &v1, const wxString &v2 );
	virtual wxString GetValue1(){return value1;}
	virtual wxString GetValue2(){return value2;}
	///Проверить значение поля
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
	void OnChanged( wxCommandEvent &event);
	void OnMouse(wxMouseEvent& event);
	///Установить значение check box из строки

	///Если "0" то не выбрана если пустая строка то 3 состояние иначе выбрана
	void SetValueStr(const wxString &s);

	///Получить строковое значение checkBox
	wxString GetValueStr();
public:
	DECLARE_EVENT_TABLE()
};


#endif //__wxDbCheckField__