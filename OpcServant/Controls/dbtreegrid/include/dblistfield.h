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
����� ���� ��������� � ������ ���� ��� ��������������  � ���� ������
*/
///����� ���� ��������� � ������ ���� ��� ��������������  � ���� ������
class wxDbListField :public wxChoice, public wxDbField{
public:
	///�����������
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
	///���������� ��������
	virtual void SetValue(const wxString &v1, const wxString &v2 );
	virtual wxString GetValue1(){return value1;}
	virtual wxString GetValue2(){return value2;}
	///��������� �������� ����
	virtual bool CheckValue();
	///�������� ����� �� ����������
	virtual wxEvtHandler* GetEvtHandler(){return  this;}
	///���������� �������� ��� ��
	virtual SetValues(wxArrayString &v){values=v;};
private:
	//���������� ���������� �����
	void OnRefreshField(wxDbFieldEvent& );
	void OnEnter(wxCommandEvent& );
	void OnKillFocus(wxFocusEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnChar(wxKeyEvent &event);
	//���������� ������� ������ ������ ����
	void OnMouseRightClick(wxMouseEvent& event);
	void OnChanged( wxCommandEvent &event);
	void OnMouse(wxMouseEvent& event);
	//���������� �������� ������
	void SetValueStr(const wxString &s);

	///�������� ��������� �������� 
	wxString GetValueStr();
	//������ �������� ��������������� ��������� ��� ������, ��������� � ��
	wxArrayString values;

public:
	DECLARE_EVENT_TABLE()
};


#endif //__wxDbListField__