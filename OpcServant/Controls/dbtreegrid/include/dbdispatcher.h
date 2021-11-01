/////////////////////////////////////////////////////////////////////////////
// Name:        dbdispatcher.h
// Purpose:     Description of the class synchronization
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

//����� ��� ����� � ���������� ����������� ������� 
//����� ������ � ������ ����� � �������� � ������������ ������ ����������
#ifndef __dbdispatcher__
#define __dbdispatcher__
#include <wx/wx.h>
#include <wx/grid.h>
#include "dbstruct.h"
//

class wxDbDispatcherEvent;
class wxDbGridEvent;

/**
@class wxDbGridLink
���������� ������� � ����������� ����
*/
///���������� ������� � ����������� ����
class wxDbGridLink 
{
public:
	wxDbGridLink(){parent=0; child=0;};
	~wxDbGridLink(){};
	///�������(������������) ����
	wxDbQueryObject* parent;
	///����������� ����
	wxDbQueryObject* child;
};
#include <wx/dynarray.h>

WX_DECLARE_OBJARRAY( wxDbGridLink, ArraywxDbGridLink );




/**
@class wxDbDispatcher
����� ��� ������������� ������ ���������� ��������� ������ � �����
*/
///����� ��� ������������� ������ ���������� ��������� ������ � �����

class  wxDbDispatcher : public wxEvtHandler
{

public:
	wxDbDispatcher(){activeGrid=0;win=0;};
	~wxDbDispatcher();
	///���������� ������� �� ������, ��������� �� ���� ��������� ������ �� ��������� ���� �������
	void Handler_(wxDbRecEvent &event);
	ArraywxDbGridLink links;
	///�������� ����� ����� �������
	void AddLink(wxDbQueryObject* p,wxDbQueryObject* c);
	///���������� ����� � ������� ������

	///����������� ���� �� ����������� �����, ���� ���� � � ��� ���� ���������, �� �������� ��������������
	bool OnPostRecord(wxDbRecEvent& event);

	///���������� �������� �� ����� ������

	///���� � ����� ���� ���������, ����������� ����� �� ������������� ���������� ����������� ������
	bool OnPreRecord(wxDbRecEvent& event);


	///���������� ������

	///���������� ����������� ������ ��������� ������ ������� �� �������� � ������������
	bool OnSave(wxDbRecEvent& event);

	///�������� ������������� �� ������ ������ �� �������
	void GetLinkTree(ArraywxDbGridLink &al,wxDbQueryObject * root);

	///���������� �������� ������ �������������

	///��������� ���� �� � ����������� ������ ������
	bool OnDelete(wxDbRecEvent& event);

	///�������� ����

	///��������� ����� ����� ������� � ������������� ����������� ����������� � �����
	///���� ����� ���� parent ���� ����������� !!!
	void AddGrid(wxDbQueryObject *grid);

	///���� �� ���������
	bool IsChanged();
	///������������� ����
	bool IsEdit();
	///�������� �������������� � ������
	void DelEditField()	;
	///������� ����������� � ������
	void Clear();
	///�������� ������� ������ � �����
	wxString GetCurPos();

	///�������� ����
	wxDbQueryObject *activeGrid;

	///���������� ��������� �����

	///���������� �������� ���� � �������� ������� � ������� ���� ��� ��������� ����
	void OnGridActive(wxDbGridEvent &event);


	///���������� ��������� ������� ���������� ������� ��������� ����� 
	void OnToolBar(wxCommandEvent &event);

	///�������� ���� �����
	wxEvtHandler * GetMainWindow();
	
	///������� ���� �����
	wxWindow *win;
protected:
	DECLARE_EVENT_TABLE()




};



/**
@class wxDbGridEvent
����� ������� ��� ����������
*/
///����� ������� ��� ����������

class wxDbDispatcherEvent : public wxNotifyEvent{
public:
   wxDbDispatcherEvent():wxNotifyEvent(){dispatcher=0;};
   wxDbDispatcherEvent(const wxDbDispatcherEvent& event)
	   : wxNotifyEvent(event){
	   dispatcher=event.dispatcher;
   };
   wxDbDispatcherEvent (wxEventType commandEventType, int id=0);
   virtual wxEvent *Clone() const { return new wxDbDispatcherEvent(*this); }
public:
	///���������
	wxDbDispatcher *dispatcher;
};


BEGIN_DECLARE_EVENT_TYPES()

	DECLARE_EVENT_TYPE(dbEVT_DISPATCHER_ACTIVE_HANDLE, 7102)

END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*dbDispatcharEventFunction)(wxDbDispatcherEvent&);


#define dbDispatcherEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(dbDispatcharEventFunction, &func)

///���������� ������� ��������� � ���������� ����(�������) ���������� ���������
#define EVT_DISPATCHER_ACTIVE_HANDLE( winid, func) \
    wx__DECLARE_EVT1(dbEVT_DISPATCHER_ACTIVE_HANDLE, winid, dbDispatcherEventHandler(func))


#endif //__dbdispatcher__