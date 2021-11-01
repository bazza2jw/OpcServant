/////////////////////////////////////////////////////////////////////////////
// Name:        dbcheckfield.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dbcheckfield.h"
#include "../include/dbstruct.h"

wxDbCheckField::wxDbCheckField(wxWindow* parent,
				wxWindowID id, 
				const wxString& label, 
				const wxPoint& pos , 
				const wxSize& size , 
				long style , 
				const wxValidator& val, 
				const wxString& name ):
wxCheckBox(parent, id, label, pos, size, style, val,  name)
{
		fdesc=0;
		owner=0;
	
};

BEGIN_EVENT_TABLE( wxDbCheckField, wxCheckBox)
EVT_REFRESH_FIELD(wxID_ANY,wxDbCheckField::OnRefreshField)
EVT_TEXT_ENTER(wxID_ANY, wxDbCheckField::OnEnter)
EVT_KILL_FOCUS(wxDbCheckField::OnKillFocus)
EVT_SET_FOCUS(wxDbCheckField::OnSetFocus)
//EVT_CHAR(wxDbCheckField::OnChar) 
EVT_RIGHT_DOWN(wxDbCheckField::OnMouseRightClick)
EVT_MOUSE_EVENTS(wxDbCheckField::OnMouse)
EVT_CHECKBOX(wxID_ANY,wxDbCheckField::OnChanged)
//EVT_KEY_DOWN(wxDbCheckField::OnChar) 

END_EVENT_TABLE()



void wxDbCheckField::OnChanged( wxCommandEvent &event)
{

	wxFocusEvent ev;
	OnKillFocus(ev);

};


void wxDbCheckField::OnRefreshField(wxDbFieldEvent& e){
	

  SetValue(e.GetValue1(), e.GetValue2() );
};
void wxDbCheckField::OnEnter(wxCommandEvent& event){

	  event.Skip(true);

};
void wxDbCheckField::OnKillFocus(wxFocusEvent& event){

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

void wxDbCheckField::OnSetFocus(wxFocusEvent& event){

		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CMND);
		ev.value2=value2;
		ev.value1=GetValueStr();
		ev.field=this;
		ev.eType=DB_FIELD_SET_FOCUS_EVT;
		if(owner)owner->ProcessEvent(ev);
	    event.Skip(true);

};



bool wxDbCheckField::CheckValue(){
	
	return true;
};


void wxDbCheckField::OnChar(wxKeyEvent &event)
{
	wxUint32 scode;
	scode=event.GetKeyCode();

	wxKeyEvent ev;
	if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==83)//Обрабатываем кнопку Сохранить отдельно 
	{

	
		if(!CheckValue()){ return;}
		if(GetValue()!=value1){
			wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CHANGED);
			ev.value2=value2;
			ev.value1=GetValueStr();
			ev.field=this;
			value1=GetValueStr();
			if(owner)owner->ProcessEvent(ev);
		}

	}
	
	ProcessKey(event);


	if(event.GetSkipped()) event.Skip(false);
	else return;


	event.Skip();
};


void wxDbCheckField::OnMouseRightClick(wxMouseEvent& event)
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

void wxDbCheckField::SetValue(const wxString &v1, const wxString &v2 )
{
	value1=v1; value2=v2; 
	SetValueStr(v1);
};

void wxDbCheckField::OnMouse(wxMouseEvent& event)
{

	//Если не редактируем то фильтруем все события мыши
	if(fdesc && !(fdesc->GetProperty()&DB_FIELD_EDITABLE)) return ;
	event.Skip();		
};


///Установить значение check box из строки

///Если "0" то не выбрана если пустая строка то 3 состояние иначе выбрана
void wxDbCheckField::SetValueStr(const wxString &s)
{
	
	long l;
	s.ToLong(&l);
	if(l==0){ wxCheckBox::SetValue(false); return;}
	if(l==1){ wxCheckBox::SetValue(true); return;}
	Set3StateValue( wxCHK_UNCHECKED ); 
	return;
};

///Получить строковое значение checkBox
wxString wxDbCheckField::GetValueStr()
{
	if(Is3State()) return value1;
	if(IsChecked()) return wxT("1");
	return wxT("0");

};
