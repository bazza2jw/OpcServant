/////////////////////////////////////////////////////////////////////////////
// Name:        dbpopupmenu.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dbpopupmenu.h"
#include "../include/langdef.h"
wxMenu dbmenu;

void createDbMenu(){
	
//	wxMenuItem* m_edit;
//	m_edit = new wxMenuItem( &dbmenu, ID_EDIT, wxT("Редактировать") , wxEmptyString, wxITEM_NORMAL );
//	dbmenu.Append( m_edit );
	

	wxMenuItem* m_del;
	m_del = new wxMenuItem( &dbmenu, ID_DELETE, DEL_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_del );
	
	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( &dbmenu, ID_INSERT, ADD_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_menuItem3 );
	
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( &dbmenu, ID_INSERT_CHILD, ADD_CHILD_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_menuItem4 );

	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( &dbmenu, ID_DUBLICATE_RECORD, DUPLICATE_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_menuItem6 );

	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( &dbmenu, ID_EXTERN_EDITOR, EXTERNAL_EDITOR_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_menuItem7 );

	wxMenuItem* m_separator1;
	m_separator1 = dbmenu.AppendSeparator();
	
	wxMenuItem* m_search;
	m_search = new wxMenuItem( &dbmenu, ID_SEARCH, SEARCH_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_search );
	
	wxMenuItem* m_search_next;
	m_search_next = new wxMenuItem( &dbmenu, ID_SEARCH_NEXT, SEARCH_NEXT_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_search_next );

	wxMenuItem* m_query;
	m_query = new wxMenuItem( &dbmenu, ID_QUERY, QUERY_MENU_TEXT, wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_query );
	
	wxMenuItem* m_save;
	m_save = new wxMenuItem( &dbmenu, ID_SAVE, SAVE_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_save );
	
	wxMenuItem* m_refresh;
	m_refresh = new wxMenuItem( &dbmenu, ID_REFRESH, REFRESH_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_refresh );

	wxMenuItem* m_property;
	m_property = new wxMenuItem( &dbmenu, ID_SHOW_PROPERTY, PROPERTY_MENU_TEXT , wxEmptyString, wxITEM_NORMAL );
	dbmenu.Append( m_property );

	
	
};
