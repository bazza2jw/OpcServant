/////////////////////////////////////////////////////////////////////////////
// Name:        dbdispatcher.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/dbdispatcher.h"
#include <wx/arrimpl.cpp>
#include "../include/t02.h"
#include "../include/dbpopupmenu.h"
#include "../include/dbtreegrid.h"
#include "../include/langdef.h"

DEFINE_EVENT_TYPE(dbEVT_DISPATCHER_ACTIVE_HANDLE)


wxDbDispatcherEvent::wxDbDispatcherEvent(wxEventType commandEventType, int id):wxNotifyEvent(commandEventType,id)
 {dispatcher=0;};



BEGIN_EVENT_TABLE( wxDbDispatcher, wxEvtHandler)
		EVT_REC_HANDLE(wxID_ANY,wxDbDispatcher::Handler_)
		EVT_GRID_ACTIVE_HANDLE(wxID_ANY,wxDbDispatcher::OnGridActive)
		EVT_MENU_RANGE(ID_EDIT,ID_LAST_TOOL,wxDbDispatcher::OnToolBar)
END_EVENT_TABLE()




WX_DEFINE_OBJARRAY( ArraywxDbGridLink );

void wxDbDispatcher::Handler_(wxDbRecEvent &event){


	switch(event.tp)
	{
	case DB_POST_RECORD_HANDLE :   
		OnPostRecord(event);
		break;
	case DB_PRE_RECORD_HANDLE :   
		OnPreRecord(event);
		break;
	case DB_SAVE_HANDLE :  
		OnSave(event);
		break;
	case DB_DELETE_HANDLE :  
		OnDelete(event);
		break;
	default :
		event.Skip();
	};



};




///�������� ����� ����� �������
void  wxDbDispatcher::AddLink(wxDbQueryObject* p,wxDbQueryObject* c)
{
	for(int i=0; i<(int)links.Count(); i++)
	{
		if(p==links[i].parent && c==links[i].child)
		{
			return ;
		};
	};
	
	wxDbGridLink l;
	l.parent=p;
	l.child=c;
	links.Add(l);
	return ;
}


	///����������� ���� �� ����������� �����, ���� ���� � � ��� ���� ���������, �� �������� ��������������
bool wxDbDispatcher::OnPostRecord(wxDbRecEvent& event)
{


	ArraywxDbGridLink al;
	wxDbQueryObject* o=event.obj;
	GetLinkTree(al,o);
	for(int i=0; i<(int) al.Count(); i++)
	{

		if(al[i].child->GetProperty(DB_GRID_IS_CHANGED))
		{
			int answer=wxMessageBox(TEXT_IS_CHANGE_IN_BLOCKS,wxEmptyString,wxOK|wxCANCEL) ;
			if(! (wxOK == answer ))
			{  
				event.veto=true;
				return false;
			}

		}
	};
	return true;
};

	///���������� �������� �� ����� ������
	///���� � ����� ���� ���������, ����������� ����� �� ������������� ���������� ����������� ������
bool wxDbDispatcher::OnPreRecord(wxDbRecEvent& event)
{
	ArraywxDbGridLink al;
	wxDbQueryObject* o=event.obj;
	GetLinkTree(al,o);
	for(int i=0; i<(int)al.Count(); i++)
	{
		al[i].child->Reset();
		al[i].child->CreateQuery();
		al[i].child->ExecuteQuery();
		al[i].child->GetRows();
		al[i].child->SelectFirstRecord();



	};
	//�������� ���������� �������� ���� ����� �� ��� ���������� ����� ������� ������
	wxWindow *w=wxGetApp().GetTopWindow();
	if(w)
	{
			w->ProcessEvent(event);
	};
	return true;
};

	///���������� ������
	///���������� ����������� ������ ��������� ������ ������� �� �������� � ������������
bool wxDbDispatcher::OnSave(wxDbRecEvent& event)
{
	ArraywxDbGridLink al;
	wxDbQueryObject* o=0;
	GetLinkTree(al,o);
	if(al.Count())
	{
		al[0].child->db->BeginTransaction();
		for(int i=0; i<(int)al.Count(); i++)
		{
			if(!al[i].child->Save())
			{
				al[0].child->db->RollBack();
				return false;
			};
		};
	};
	al[0].child->db->Commit();

	///�������� ����� � ���� �������
	for(int i=0; i<(int)al.Count(); i++)
	{
			al[i].child->ResetFlag();
	};



	return true;
};

///�������� ������������� �� ������ ������ �� �������
void wxDbDispatcher::GetLinkTree(ArraywxDbGridLink &al,wxDbQueryObject * root)
{
	for(int i=0; i<(int)links.Count(); i++)
	{

		if(links[i].parent==root)
		{

			al.Add(links[i]);
			GetLinkTree(al,links[i].child);
		}
	};
};

///�������� ����
void wxDbDispatcher::AddGrid(wxDbQueryObject *grid)
{
	AddLink(grid->parent,grid);
	grid->saveHandler=this;
	grid->postRecordHandler=this;
	grid->preRecordHandler=this;
	grid->deleteHandler=this;
	grid->dispatcher=this;
}

///���������� �������� ������ �������������

///��������� ���� �� � ����������� ������ ������
bool wxDbDispatcher::OnDelete(wxDbRecEvent& event)
{


	ArraywxDbGridLink al;
	wxDbQueryObject* o=event.obj;
	GetLinkTree(al,o);
	for(int i=0; i<(int) al.Count(); i++)
	{

		if(al[i].child->IsRecords())
		{
			int answer=wxMessageBox(TEXT_IS_CHANGE_IN_BLOCKS_DELETE,wxEmptyString,wxOK) ;
			event.veto=true;
			return false;
		

		}
	};
	return true;
};

bool wxDbDispatcher::IsChanged()
{

	for(int i=0; i<(int)links.Count(); i++)
	{

		if(links[i].child->GetProperty()&DB_GRID_IS_CHANGED) return true;

	};
	return false;
};


bool wxDbDispatcher::IsEdit()
{

	for(int i=0; i<(int)links.Count(); i++)
	{

		if(links[i].child->IsEdit()) return true;
	
	};
	return false;
};
void wxDbDispatcher::DelEditField()
{

	for(int i=0; i<(int)links.Count(); i++)
	{

		links[i].child->StopEdit();
	};

};

void wxDbDispatcher::Clear()
{
	for(int i=0; i<(int)links.Count(); i++)
	{
		if(links[i].child->saveHandler==this)links[i].child->saveHandler=0;
		if(links[i].child->postRecordHandler==this)links[i].child->postRecordHandler=0;
		if(links[i].child->preRecordHandler==this)links[i].child->preRecordHandler=0;
		if(links[i].child->deleteHandler==this)links[i].child->deleteHandler=0;
		if(links[i].child->dispatcher==this)links[i].child->dispatcher=0;
	
	
	};
};


	///���������� ��������� �����

	///���������� �������� ���� � �������� ������� � ������� ���� ��� ��������� ����
void wxDbDispatcher::OnGridActive(wxDbGridEvent &event)
{

	activeGrid=event.grid;
	wxWindow *w=wxGetApp().GetTopWindow();
	wxDbDispatcherEvent ev(dbEVT_DISPATCHER_ACTIVE_HANDLE);
		ev.dispatcher=this;
		if(w)
		{
			w->ProcessEvent(ev);
		};

};


wxDbDispatcher::~wxDbDispatcher()
{
	wxWindow *w=wxGetApp().GetTopWindow();
	///������� �������� ��������� � ����������
	wxDbDispatcherEvent ev(dbEVT_DISPATCHER_ACTIVE_HANDLE);
		ev.dispatcher=0;
		if(w)
		{
			w->ProcessEvent(ev);
		};

};

///���������� ��������� ������� ���������� ������� ��������� ����� 
void wxDbDispatcher::OnToolBar(wxCommandEvent &event)
{

	if(activeGrid){
		activeGrid->SetFocus_();
		activeGrid->GetEvtHandler()->ProcessEvent(event);
	}
	
};

///�������� ���� �����
wxEvtHandler * wxDbDispatcher::GetMainWindow()
{
	wxWindow *w;
	if(this->activeGrid)
	{
		w = win;
		while( w )
		{
			if( w->IsTopLevel() ) return w;
			w = w->GetParent();

		};


	};
	return 0;
};

///�������� ������� ������ � �����
wxString wxDbDispatcher::GetCurPos()
{
	if( activeGrid )
	{
		return ((wxDbTreeGrid*)activeGrid)->GetCurPos();
	};
	return wxEmptyString;
};

