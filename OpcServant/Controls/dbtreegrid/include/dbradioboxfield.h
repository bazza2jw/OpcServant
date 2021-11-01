/////////////////////////////////////////////////////////////////////////////
// Name:        dbradioboxfield.h
// Purpose:     Control is used to select one of number choices
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __dbradioboxfield__
#define __dbradioboxfield__
#include "dbfield.h"
#include <wx/datectrl.h>
#include <wx/dateevt.h>

/**
@class wxDbRadioBoxField
����� ���� ��������� � ������ ���� ��� ��������������  � ������ ������
������ ���� �� ������� ��������, ������� ������������ � ����� � �������� ���� default value, �� null ����
*/
///����� ���� ��������� � ������ ���� ��� ��������������  � ������ ������
class wxDbRadioBoxField :public wxRadioBox, public wxDbField{
public:
	///�����������
	wxDbRadioBoxField(wxWindow* parent, 
					wxWindowID id, 
					const wxString& label, 
					const wxPoint& point = wxDefaultPosition, 
					const wxSize& size = wxDefaultSize, 
					int n = 0, 
					const wxString choices[] = NULL, 
					int majorDimension = 0, 
					long style = wxRA_SPECIFY_COLS, 
					const wxValidator& validator = wxDefaultValidator, 
					const wxString& name = "radioBox");

	virtual void SetValue(const wxString &v1, const wxString &v2 );
	virtual wxString GetValue1(){return value1;}
	virtual wxString GetValue2(){return value2;}
	///��������� �������� ����
	virtual bool CheckValue();
	///�������� ����� �� ����������
	virtual wxEvtHandler* GetEvtHandler(){return  this;}
	virtual SetValues(wxArrayString &v){values=v;};
private:
	void OnRefreshField(wxDbFieldEvent& );
	void OnEnter(wxCommandEvent& );
	void OnKillFocus(wxFocusEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnChar(wxKeyEvent &event);
	//���������� ������� ������ ������ ����
	void OnMouseRightClick(wxMouseEvent& event);
	void OnChanged( wxCommandEvent &event);
	void OnMouse(wxMouseEvent& event);
	///���������� �������� �����
	void SetValueStr(const wxString &s);

	///�������� ��������� �������� 
	wxString GetValueStr();
	///�������� ����������� ��� ��
	wxArrayString values;

public:
	DECLARE_EVENT_TABLE()
};


#endif //__wxDbRadioBoxField__