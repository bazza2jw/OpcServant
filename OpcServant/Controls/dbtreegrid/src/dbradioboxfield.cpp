/////////////////////////////////////////////////////////////////////////////
// Name:        dbradioboxfield.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dbradioboxfield.h"
#include "../include/dbstruct.h"

wxDbRadioBoxField::wxDbRadioBoxField(wxWindow* parent, 
					wxWindowID id, 
					const wxString& label, 
					const wxPoint& point , 
					const wxSize& size, 
					int n , 
					const wxString choices[] , 
					int majorDimension, 
					long style , 
					const wxValidator& validator, 
					const wxString& name):
wxRadioBox( parent,	 id, 	  label,  point ,   size,  n ,  choices ,  majorDimension,	 style , 	  validator,  name)
{
		fdesc=0;
		owner=0;
};

BEGIN_EVENT_TABLE( wxDbRadioBoxField, wxRadioBox)
EVT_REFRESH_FIELD(wxID_ANY,wxDbRadioBoxField::OnRefreshField)
EVT_TEXT_ENTER(wxID_ANY, wxDbRadioBoxField::OnEnter)
EVT_KILL_FOCUS(wxDbRadioBoxField::OnKillFocus)
EVT_SET_FOCUS(wxDbRadioBoxField::OnSetFocus)
EVT_CHAR(wxDbRadioBoxField::OnChar) 
EVT_RIGHT_DOWN(wxDbRadioBoxField::OnMouseRightClick)
EVT_MOUSE_EVENTS(wxDbRadioBoxField::OnMouse)
EVT_RADIOBOX(wxID_ANY,wxDbRadioBoxField::OnChanged)

END_EVENT_TABLE()



void wxDbRadioBoxField::OnChanged( wxCommandEvent &event)
{
	
	wxFocusEvent ev;
	OnKillFocus(ev);

};


void wxDbRadioBoxField::OnRefreshField(wxDbFieldEvent& e){


  SetValue(e.GetValue1(), e.GetValue2() );
};
void wxDbRadioBoxField::OnEnter(wxCommandEvent& event){

	  event.Skip(true);

};
void wxDbRadioBoxField::OnKillFocus(wxFocusEvent& event){

	 if(GetValueStr()!=value1){
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CHANGED);
		ev.value2=value2;
		ev.value1=GetValueStr();
		ev.field=this;
		value1=ev.value1;
		if(owner)owner->ProcessEvent(ev);
	 }
	 event.Skip(true);

};

void wxDbRadioBoxField::OnSetFocus(wxFocusEvent& event){

		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CMND);
		ev.value2=value2;
		ev.value1=GetValueStr();
		ev.field=this;
		ev.eType=DB_FIELD_SET_FOCUS_EVT;
		if(owner)owner->ProcessEvent(ev);
	    event.Skip(true);

};



bool wxDbRadioBoxField::CheckValue(){
	
	return true;
};


void wxDbRadioBoxField::OnChar(wxKeyEvent &event)
{

	wxUint32 scode;
	scode=event.GetKeyCode();

	event.Skip();
};


void wxDbRadioBoxField::OnMouseRightClick(wxMouseEvent& event)
{
		if(GetValueStr()!=value1){
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CHANGED);
		ev.value2=value2;
		ev.value1=GetValueStr();
		ev.field=this;
		if(owner)owner->ProcessEvent(ev);
	 }

	wxMouseEvent ev=event;
	if(owner)owner->ProcessEvent(ev);
};

void wxDbRadioBoxField::SetValue(const wxString &v1, const wxString &v2 )
{
	value1=v1; value2=v2; 
	SetValueStr(v1);
};

void wxDbRadioBoxField::OnMouse(wxMouseEvent& event)
{

	//Если не редактируем то фильтруем все события мыши
	if(fdesc && !(fdesc->GetProperty()&DB_FIELD_EDITABLE)) return ;
	event.Skip();		
};



void wxDbRadioBoxField::SetValueStr(const wxString &s)
{
	if(s==wxEmptyString){	SetStringSelection(wxEmptyString); return;}
	for(int i=0; i<(int)values.Count();i++)
	{
		if(values[i]== s) break;
	}
	SetSelection(i);
	return;
};

///Получить строковое значение 
wxString wxDbRadioBoxField::GetValueStr()
{
	int i=GetSelection();
	if(i<(int)values.Count()) return values[i];
	return wxEmptyString;

};
