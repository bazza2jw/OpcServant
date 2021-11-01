/////////////////////////////////////////////////////////////////////////////
// Name:        dbquerydialog_init.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dbquerydialog.h"

//ƒополнительна€ инициализаци€
void DbQueryDialog::Init(){
	//	tree->AddRoot( wxEmptyString);
		tree->SetSize(100,60);
		wxWindow *ww=GetParent();
		int w,h,h1,w1;
		ww->GetSize(&w,&h);
		GetSize(&w1,&h1);
		SetSize(wxDefaultCoord,wxDefaultCoord,w,h1,wxSIZE_FORCE);
		SetMinSize(wxSize(w,h1));
		SetMaxSize(wxSize(2000,h1));
	
		Layout();
		bSizer1->Layout( );
};


void DbQueryDialog::OnSize( wxSizeEvent& event )
{


	event.Skip();
}


///ѕолучение строки с данными дл€ запроса или поиска
wxArrayString DbQueryDialog::GetQueryData(){
	wxArrayString as;
	wxTreeItemId id=tree->GetNext(tree->GetRootItem());
	for(int i=0; i<tree->GetColumnCount(); i++)
	{
		as.Add(tree->GetItemText(id,i));
	}
	return as;
};
	
