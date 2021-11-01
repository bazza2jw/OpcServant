/////////////////////////////////////////////////////////////////////////////
// Name:        dblovdialog_init.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dblovdialog.h"

void DbLovDialog::Setup(wxDbLOV &dl)
{

	
	ArraywxDbGridFieldDesc *fd;
	SetSize(dl.size);
	tree->query=dl.query;
	tree->selectClause=dl.selectClause;
	tree->fromClause=dl.fromClause;
	tree->whereClause=dl.whereClause;
	tree->orderClause=dl.orderClause;
	tree->CreateQuery();
	tree->SetupField();
	fd=tree->GetFieldDescription();
	for ( int i=0; i<(int)dl.pair.Count() ; i++)
	{
		if(dl.pair[i].fieldName!=wxEmptyString ) (*fd) [dl.pair[i].i1].fieldName =dl.pair[i].fieldName;
	};
	tree->CreateField();
	for ( i=0; i<(int)dl.pair.Count() ; i++)
	{
		if(dl.pair[i].w>=0) tree->SetColumnWidth(dl.pair[i].i1,dl.pair[i].w);
	};
	

	tree->ResetProperty(DB_GRID_READ_ONE|DB_GRID_ALL_RECORD_FETCHED|DB_GRID_INSERT_ENABLE|DB_GRID_UPDATE_ENABLE|DB_GRID_DELET_ENABLE);
	tree->ResetProperty(DB_GRID_READ_ONE);

	if(dl.qTime) tree->GetAllRow();
	//Добавляем пустую запись в список выборки вначале списка
	wxTreeItemId root;
	root = tree->GetRootItem();
	if( root.IsOk() )
	{
		tree->InsertItem ( root, 0, wxEmptyString );
	}
	tree->SelectFirstRecord();

};


void DbLovDialog::OnQueryEnter(wxCommandEvent &event)
{


	tree->Reset();

	tree->queryData.Clear();
	for(int i=0; i<tree->GetColumnCount(); i++)
	{
		
		if(tree->GetColumnWidth(i)>0) tree->queryData.Add(ftext->GetValue());
		else tree->queryData.Add(wxEmptyString);
	};


	
	tree->CreateLOVQuery();

	tree->ExecuteQuery();
	tree->GetRows();
	tree->SelectFirstRecord();
	

};
void DbLovDialog::OnChoice(wxMouseEvent &event)
{

};


void DbLovDialog::OnMouseDoubleClick(wxMouseEvent& event)
{

		if(((wxWindow*)event.GetEventObject())->GetParent()==tree && tree->GetSelection().IsOk()) EndModal(wxID_OK); 
};
void DbLovDialog::OnKey(wxKeyEvent& event)
{

		if(((wxWindow*)event.GetEventObject())->GetParent()==tree && tree->GetSelection().IsOk()) EndModal(wxID_OK); 
};

void DbLovDialog::OnClose(wxCloseEvent &event)
{
	
	event.Veto();
	//event.Skip();
	
};

BEGIN_EVENT_TABLE( DbLovDialog, wxDialog)
	EVT_LEFT_DCLICK(DbLovDialog::OnMouseDoubleClick) 
	EVT_CHAR(DbLovDialog::OnKey) 
	EVT_KEY_DOWN(DbLovDialog::OnKey) 
	EVT_CLOSE(DbLovDialog::OnClose) 
END_EVENT_TABLE()
 