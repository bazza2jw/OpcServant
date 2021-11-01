/////////////////////////////////////////////////////////////////////////////
// Name:        dbfield.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dbfield.h"
#include <wx/arrimpl.cpp>

WX_DEFINE_OBJARRAY( ArraywxDbFieldLink );




DEFINE_EVENT_TYPE(dbEVT_COMMAND_REFRESH_FIELD)
DEFINE_EVENT_TYPE(dbEVT_COMMAND_FIELD_CHANGED)
DEFINE_EVENT_TYPE(dbEVT_COMMAND_FIELD_CMND)

wxDbFieldEvent::wxDbFieldEvent(wxEventType commandEventType, int id):
wxNotifyEvent(commandEventType,id)
{

};

void wxDbField::ProcessKey(wxKeyEvent &event)
{


		wxString s;
		s.sprintf( "%d",event.GetKeyCode() );
		
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==342)//�����
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL| wxMOD_ALT     && event.GetKeyCode()==342)//����� ��������� ������
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==17)//������
		{
			if(owner) owner->ProcessEvent(event);				
			return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==322)//�������� ������
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==127)//������� ������
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL| wxMOD_ALT     && event.GetKeyCode()==322)//�������� ������ �������
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==19)//���������
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==18)//��������
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}

	event.Skip();
};
