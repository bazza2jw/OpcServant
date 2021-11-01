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
		
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==342)//Поиск
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL| wxMOD_ALT     && event.GetKeyCode()==342)//Поиск следующей записи
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==17)//Запрос
		{
			if(owner) owner->ProcessEvent(event);				
			return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==322)//Добавить запись
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==127)//Удалить запись
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL| wxMOD_ALT     && event.GetKeyCode()==322)//Добавить запись потомка
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==19)//Сохранить
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==18)//Обновить
		{
			   if(owner) owner->ProcessEvent(event);
			   return;
		}

	event.Skip();
};
