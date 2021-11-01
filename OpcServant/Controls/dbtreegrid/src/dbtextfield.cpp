/////////////////////////////////////////////////////////////////////////////
// Name:        dbtextfield.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dbtextfield.h"
#include "../include/dbstruct.h"
#include "../include/editctrl.h"

extern bool CheckVal(const wxString& fname,const wxString& s, int valueType, int len =0, int presition=0,const wxString& maxVal=wxEmptyString,const wxString& minVal=wxEmptyString, bool notNull=false, int minLen = 0);
extern wxString FormatField(const wxString &ss,wxDbGridFieldDesc *fd);

wxDbTextField::wxDbTextField (wxWindow *parent, wxWindowID id, const wxString &value,const wxPoint &pos,
				const wxSize &size, long style,	const wxValidator &validator,const wxString &name):
				wxTextCtrl(parent,  id, value,pos,size,style,validator,name)
{
		fdesc=0;
		owner=0;
};

BEGIN_EVENT_TABLE( wxDbTextField, wxTextCtrl)
EVT_REFRESH_FIELD(wxID_ANY,wxDbTextField::OnRefreshField)
EVT_TEXT_ENTER(wxID_ANY, wxDbTextField::OnEnter)
EVT_KILL_FOCUS(wxDbTextField::OnKillFocus)
EVT_SET_FOCUS(wxDbTextField::OnSetFocus)
EVT_CHAR(wxDbTextField::OnChar) 
EVT_RIGHT_DOWN(wxDbTextField::OnMouseRightClick)

END_EVENT_TABLE()

void wxDbTextField::OnRefreshField(wxDbFieldEvent& e){


  SetValue(e.GetValue1(), e.GetValue2() );
};
void wxDbTextField::OnEnter(wxCommandEvent& event){

	  event.Skip(true);

};
void wxDbTextField::OnKillFocus(wxFocusEvent& event){
	
	if(!CheckValue()){SetFocus(); return;}
	 if(GetValue()!=value1){
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CHANGED);
		ev.value2=value2;
		ev.value1=FormatField(GetValue(),fdesc);
		ev.field=this;
		value1=FormatField(GetValue(),fdesc);
		if(owner)owner->ProcessEvent(ev);
	 }
	 event.Skip(true);

};

void wxDbTextField::OnSetFocus(wxFocusEvent& event){
		if(!fdesc) return;
		if(fdesc->validator) SetValidator(*fdesc->validator);
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CMND);
		ev.value2=value2;
		ev.value1=FormatField(GetValue(),fdesc);
		ev.field=this;
		ev.eType=DB_FIELD_SET_FOCUS_EVT;
		if(owner)owner->ProcessEvent(ev);
	    event.Skip(true);

};



bool wxDbTextField::CheckValue(){
	if(!fdesc) return true;
	return CheckVal(fdesc->columnName,GetValue(),fdesc->valueType,fdesc->len,fdesc->presition,fdesc->maxVal,fdesc->minVal,fdesc->GetProperty()&DB_FIELD_NOT_NULL);
};


void wxDbTextField::OnChar(wxKeyEvent &event)
{

	
	if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==19)//Обрабатываем кнопку Сохранить отдельно 
	{
	
		if(!CheckValue()){ return;}
		if(GetValue()!=value1){
			wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CHANGED);
			ev.value2=value2;
			ev.value1=FormatField(GetValue(),fdesc);
			ev.field=this;
			value1=FormatField(GetValue(),fdesc);
			if(owner)owner->ProcessEvent(ev);
		}
			 
	}


	ProcessKey(event);
	if(event.GetSkipped()) event.Skip(false);
	else return;



	wxUint32 scode;
	scode=event.GetKeyCode();
	if((scode==32 ||  scode==341 || (scode >=0 && scode<=255)) && scode!=9 && fdesc && fdesc->lov)
	{
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CMND);
		ev.value2=value2;
		ev.value1=GetValue();
		ev.field=this;
		ev.eType=DB_FIELD_LOV_EVT;
		if(owner)owner->ProcessEvent(ev);
	    //event.Skip(false);			
		return;
	}

	if((scode==32 ||  scode==341 || (scode >=0 && scode<=255)) && scode!=9 && fdesc &&  fdesc->GetProperty()&DB_FIELD_USER_LOV)
	{
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CMND);
		ev.value2=value2;
		ev.value1=GetValue();
		ev.field=this;
		ev.eType=DB_FIELD_CUSTOM_LOV_EVT;
		if(owner)owner->ProcessEvent(ev);

	    //event.Skip(false);			
		return;
	}
	event.Skip();
};


void wxDbTextField::OnMouseRightClick(wxMouseEvent& event)
{
	if(!fdesc) return;
	if(GetValue()!=value1){
		wxDbFieldEvent ev=wxDbFieldEvent(dbEVT_COMMAND_FIELD_CHANGED);
		ev.value2=value2;
		ev.value1=FormatField(GetValue(),fdesc);
		ev.field=this;
		if(owner)owner->ProcessEvent(ev);
	 }

	wxMouseEvent ev=event;
	if(owner)owner->ProcessEvent(ev);
};
