/////////////////////////////////////////////////////////////////////////////
// Name:        dbdatefield.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dbdatefield.h"
#include "../include/dbstruct.h"
#include "../include/langdef.h"
extern bool CheckVal(const wxString& fname,const wxString& s, int valueType, int len =0, int presition=0,const wxString& maxVal=wxEmptyString,const wxString& minVal=wxEmptyString, bool notNull=false, int minLen = 0 );
extern wxString FormatField(const wxString &ss,wxDbGridFieldDesc *fd);


wxDbDateField::wxDbDateField (wxWindow *parent, 
				wxWindowID id, 
				const wxDateTime& dt, 
				const wxPoint& pos, 
				const wxSize& size, 
				long style, 
				const wxValidator& validator, 
				const wxString& name):
				wxDatePickerCtrl(parent, id, dt, pos, size, style, validator,  name)
{
		fdesc=0;
};

BEGIN_EVENT_TABLE( wxDbDateField, wxDatePickerCtrl)
EVT_REFRESH_FIELD(wxID_ANY,wxDbDateField::OnRefreshField)
EVT_TEXT_ENTER(wxID_ANY, wxDbDateField::OnEnter)
EVT_KILL_FOCUS(wxDbDateField::OnKillFocus)
EVT_SET_FOCUS(wxDbDateField::OnSetFocus)
EVT_CHAR(wxDbDateField::OnChar) 
EVT_RIGHT_DOWN(wxDbDateField::OnMouseRightClick)
EVT_DATE_CHANGED(wxID_ANY, wxDbDateField::OnEnter)
EVT_MOUSE_EVENTS(wxDbDateField::OnMouse)

END_EVENT_TABLE()

void wxDbDateField::OnEnter( wxDateEvent &event)
{
	wxFocusEvent ev;
	OnKillFocus(ev);

};


void wxDbDateField::OnRefreshField(wxDbFieldEvent& e){


  SetValue(e.GetValue1(), e.GetValue2() );
};
void wxDbDateField::OnEnter(wxCommandEvent& event){

	  event.Skip(true);

};
void wxDbDateField::OnKillFocus(wxFocusEvent& event){
	if(!CheckValue()){SetFocus(); return;}
	 if(GetValue().FormatDate()!=value1){
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CHANGED);
		ev.value2=value2;
		if(fdesc->valueType!=DB_FIELD_DATETIME) 
				ev.value1=GetValue().Format(DATE_FORMAT);
		else
				ev.value1=GetValue().Format(DATETIME_FORMAT);
		ev.field=this;
		if(owner)owner->ProcessEvent(ev);
	 }
	 event.Skip(true);

};

void wxDbDateField::OnSetFocus(wxFocusEvent& event){

		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CMND);
		ev.value2=value2;
		if(fdesc->valueType!=DB_FIELD_DATETIME) 
				ev.value1=GetValue().Format(DATE_FORMAT);
		else
				ev.value1=GetValue().Format(DATETIME_FORMAT);

		ev.field=this;
		ev.eType=DB_FIELD_SET_FOCUS_EVT;
		if(owner)owner->ProcessEvent(ev);
	    event.Skip(true);

};



bool wxDbDateField::CheckValue(){
	wxString s;
	if(fdesc->valueType!=DB_FIELD_DATETIME) 
			s=GetValue().Format(DATE_FORMAT);
		else
			s=GetValue().Format(DATETIME_FORMAT);

	return CheckVal(fdesc->columnName,s,fdesc->valueType,fdesc->len,fdesc->presition,fdesc->maxVal,fdesc->minVal, fdesc->GetProperty()&DB_FIELD_NOT_NULL);

};


void wxDbDateField::OnChar(wxKeyEvent &event)
{
	ProcessKey(event);
	if(event.GetSkipped()) event.Skip(false);
	else return;
	wxUint32 scode;
	scode=event.GetKeyCode();

	event.Skip();
};


void wxDbDateField::OnMouseRightClick(wxMouseEvent& event)
{
		if(GetValue().FormatDate()!=value1){
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CHANGED);
		ev.value2=value2;
		if(fdesc->valueType!=DB_FIELD_DATETIME) 
				ev.value1=GetValue().Format(DATE_FORMAT);
		else
				ev.value1=GetValue().Format(DATETIME_FORMAT);

		ev.field=this;
		if(owner)owner->ProcessEvent(ev);
	 }

	wxMouseEvent ev=event;
	if(owner)owner->ProcessEvent(ev);
};

void wxDbDateField::SetValue(const wxString &v1, const wxString &v2 )
{
	value1=v1; value2=v2; 
	wxDateTime  dt;
		if( v1== wxEmptyString ) dt.SetToCurrent();
		else dt.ParseFormat( v1,DATE_FORMAT);
		wxDatePickerCtrl::SetValue (dt);
};

void wxDbDateField::OnMouse(wxMouseEvent& event)
{

	//Если не редактируем то фильтруем все события мыши
	if(fdesc && !(fdesc->GetProperty()&DB_FIELD_EDITABLE)) return ;
	event.Skip();		
};
