/////////////////////////////////////////////////////////////////////////////
// Name:        dbpopupmenu.h
// Purpose:     Menu creation
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////


//Меню для редактирования данных
#ifndef __dbpopupmenu__
#define __dbpopupmenu__



#include <wx/wx.h>

#define ID_EDIT 8000
#define ID_DELETE 8001
#define ID_INSERT 8002
#define ID_SEARCH 8003
#define ID_SAVE 8004
#define ID_REFRESH 8005
#define ID_QUERY 8006
#define ID_INSERT_CHILD 8007
#define ID_SEARCH_NEXT 8008
#define ID_DUBLICATE_RECORD 8009
#define ID_SHOW_PROPERTY 8010
#define ID_EXTERN_EDITOR 8011
#define ID_LAST_TOOL 8100






extern wxMenu dbmenu;
void createDbMenu();



#endif