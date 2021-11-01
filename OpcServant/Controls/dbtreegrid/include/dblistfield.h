/////////////////////////////////////////////////////////////////////////////
// Name:        dblistfield.h
// Purpose:     Field for choice several values
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __dblistfield__
#define __dblistfield__
#include "dbfield.h"
#include <wx/datectrl.h>
#include <wx/dateevt.h>

/**
@class wxDbListField
Класс поле связанное с гридом поле для редактирования  в виде списка
*/
///Класс поле связанное с гридом поле для редактирования  в виде списка
class wxDbListField :public wxChoice, public wxDbField{
public:
	///Конструктор
	wxDbListField(wxWindow *parent, 
				wxWindowID id, 
				const wxPoint& pos, 
				const wxSize& size, 
				int n, 
				const wxString choices[], 
				long style = 0, 
				const wxValidator& validator = wxDefaultValidator, 
				const wxString& name = "choice");


;
	///Установить значение
	virtual void SetValue(const wxString &v1, const wxString &v2 );
	virtual wxString GetValue1(){return value1;}
	virtual wxString GetValue2(){return value2;}
	///Проверить значение поля
	virtual bool CheckValue();
	///Получить сылку на обработчик
	virtual wxEvtHandler* GetEvtHandler(){return  this;}
	///Установить значения для БД
	virtual SetValues(wxArrayString &v){values=v;};
private:
	//Обработчик обновления полей
	void OnRefreshField(wxDbFieldEvent& );
	void OnEnter(wxCommandEvent& );
	void OnKillFocus(wxFocusEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnChar(wxKeyEvent &event);
	//Обработчик нажатия правой кнопки мыши
	void OnMouseRightClick(wxMouseEvent& event);
	void OnChanged( wxCommandEvent &event);
	void OnMouse(wxMouseEvent& event);
	//Установить значение списка
	void SetValueStr(const wxString &s);

	///Получить строковое значение 
	wxString GetValueStr();
	//Второе значение соответствующее основному для списка, заносится в БД
	wxArrayString values;

public:
	DECLARE_EVENT_TABLE()
};


#endif //__wxDbListField__