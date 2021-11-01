/////////////////////////////////////////////////////////////////////////////
// Name:        dbtextfield.h
// Purpose:     Control for edit text data
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __dbtextfield__
#define __dbtextfield__
#include "dbfield.h"
#include "wx/textctrl.h"

/**
@class wxDbTextField
����� ��������� � ������ ���� ��� �������������� ������
*/
///����� ��������� � ������ ���� ��� �������������� ������
class wxDbTextField :public wxTextCtrl, public wxDbField{
public:
	///�����������
	wxDbTextField (wxWindow *parent, wxWindowID id, 
				const wxString &value=wxEmptyString, 
				const wxPoint &pos=wxDefaultPosition,
				const wxSize &size=wxDefaultSize, long style=0,
				const wxValidator &validator=wxDefaultValidator,
				const wxString &name=wxTextCtrlNameStr);
	///���������� �������� ����
	virtual void SetValue(const wxString &v1, const wxString &v2 ){value1=v1; value2=v2; ChangeValue(value1);DiscardEdits ();};
	virtual wxString GetValue1(){return value1;}
	virtual wxString GetValue2(){return value2;}
	///��������� �������� ����
	virtual bool CheckValue();
	///�������� ����� �� ����������
	virtual wxEvtHandler* GetEvtHandler(){return  this;}

private:
	void OnRefreshField(wxDbFieldEvent& );
	void OnEnter(wxCommandEvent& );
	void OnKillFocus(wxFocusEvent& event);
	void OnSetFocus(wxFocusEvent& event);
	void OnChar(wxKeyEvent &event);
	//���������� ������� ������ ������ ����
	void OnMouseRightClick(wxMouseEvent& event);

public:
	DECLARE_EVENT_TABLE()
};


#endif //__dbtextfield__