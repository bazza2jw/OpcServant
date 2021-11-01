/////////////////////////////////////////////////////////////////////////////
// Name:        dblistfield.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dblistfield.h"
#include "../include/dbstruct.h"

wxDbListField::wxDbListField(wxWindow *parent, 
				wxWindowID id, 
				const wxPoint& pos, 
				const wxSize& size, 
				int n, 
				const wxString choices[], 
				long style, 
				const wxValidator& validator , 
				const wxString& name ):
wxChoice(parent, id,   pos,   size, n,   choices, style,  validator ,   name)
{
		fdesc=0;
		owner=0;
};

BEGIN_EVENT_TABLE( wxDbListField, wxChoice)
EVT_REFRESH_FIELD(wxID_ANY,wxDbListField::OnRefreshField)
EVT_TEXT_ENTER(wxID_ANY, wxDbListField::OnEnter)
EVT_KILL_FOCUS(wxDbListField::OnKillFocus)
EVT_SET_FOCUS(wxDbListField::OnSetFocus)
EVT_CHAR(wxDbListField::OnChar) 
EVT_RIGHT_DOWN(wxDbListField::OnMouseRightClick)
EVT_MOUSE_EVENTS(wxDbListField::OnMouse)
EVT_CHOICE(wxID_ANY,wxDbListField::OnChanged)

END_EVENT_TABLE()



void wxDbListField::OnChanged( wxCommandEvent &event)
{
	
	wxFocusEvent ev;
	OnKillFocus(ev);

};


void wxDbListField::OnRefreshField(wxDbFieldEvent& e){
	

  SetValue(e.GetValue1(), e.GetValue2() );
};
void wxDbListField::OnEnter(wxCommandEvent& event){

	  event.Skip(true);

};
void wxDbListField::OnKillFocus(wxFocusEvent& event){

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

void wxDbListField::OnSetFocus(wxFocusEvent& event){

		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CMND);
		ev.value2=value2;
		ev.value1=GetValueStr();
		ev.field=this;
		ev.eType=DB_FIELD_SET_FOCUS_EVT;
		if(owner)owner->ProcessEvent(ev);
	    event.Skip(true);

};



bool wxDbListField::CheckValue(){
	
	return true;
};


void wxDbListField::OnChar(wxKeyEvent &event)
{

	ProcessKey(event);
	if(event.GetSkipped()) event.Skip(false);
	else return;
	wxUint32 scode;
	scode=event.GetKeyCode();

	event.Skip();
};


void wxDbListField::OnMouseRightClick(wxMouseEvent& event)
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

void wxDbListField::SetValue(const wxString &v1, const wxString &v2 )
{
	value1=v1; value2=v2; 
	SetValueStr(v1);
};

void wxDbListField::OnMouse(wxMouseEvent& event)
{
	
	//Если не редактируем то фильтруем все события мыши
	if(fdesc && !(fdesc->GetProperty()&DB_FIELD_EDITABLE)) return ;
	event.Skip();		
};



void wxDbListField::SetValueStr(const wxString &s)
{
	if(s==wxEmptyString){	SetSelection(-1); return;}
	for(int i=0; i<(int)values.Count();i++)
	{
		if(values[i]== s) break;
	}
	SetSelection(i);
	return;
};

///Получить строковое значение checkBox
wxString wxDbListField::GetValueStr()
{
	int i=GetSelection();
	if(i<(int)values.Count()) return values[i];
	return wxEmptyString;

};
