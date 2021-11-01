/////////////////////////////////////////////////////////////////////////////
// Name:        dbdispatcher.h
// Purpose:     Base field class
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////



#ifndef __dbfield__
#define __dbfield__
#include <wx/wx.h>
#include "db.h"
#include <wx/dynarray.h>





class wxDbGridFieldDesc;
class wxDbQueryObject;

/**
@class wxDbField 
������� ����� ��� ����� �������������� ������
*/
///������� ����� ��� ����� �������������� ������


class wxDbField /*:public wxEventHandler*/{
public:
	///�����������
	wxDbField(){owner=0;fdesc=0;};
	//dbField(wxString &v1, wxString &v2 ){value1=v1; value2=v2;};
	///���������� ��������
	virtual void SetValue(const wxString &v1,const wxString &v2 ){value1=v1; value2=v2;};
	///�������� �������� 1
	virtual wxString GetValue1(){return value1;}
	///�������� �������� 2
	virtual wxString GetValue2(){return value2;}
	///�������� 1
	wxString value1;
	///�������� 2
	wxString value2;
	///������ �� ������������ ����
	wxEvtHandler *owner;
	///������ �� �������� ����
	wxDbGridFieldDesc* fdesc;
	///�������� ����� �� ����������
	virtual wxEvtHandler* GetEvtHandler(){return 0;}
	///������������ ����������� ������� �� EVT_CHAR
	virtual void ProcessKey(wxKeyEvent &event);

};

/**
@class wxDbFieldLink
����� ����� ����� � ����� 
*/
///����� ����� ����� � ����� 
class wxDbFieldLink{
public:
	wxDbFieldLink(){obj=0; fnum=0;};
	wxDbFieldLink(wxEvtHandler *f,int fn){obj=f;fnum=fn;}
	wxEvtHandler *obj;
	///field number
	int fnum;

};

///������ ��� �������� ������ ���� - ����

WX_DECLARE_OBJARRAY( wxDbFieldLink, ArraywxDbFieldLink );


///���� ������� ��� eType wxDbFieldEvent
///���� �������� �����
const int DB_FIELD_SET_FOCUS_EVT=1;
///������ LOV ��� ����
const int DB_FIELD_LOV_EVT=2;
///������ custom LOV ��� ����
const int DB_FIELD_CUSTOM_LOV_EVT=3;

/**
@class 
������� ��� �������� ������ ����� ������ � ������, � ������������  ��������� ���������
*/
///������� ��� �������� ������ ����� ������ � ������, � ������������  ��������� ���������
class wxDbFieldEvent : public wxNotifyEvent{
public:
   wxDbFieldEvent():wxNotifyEvent(){};
   wxDbFieldEvent(const wxDbFieldEvent& event)
	   : wxNotifyEvent(event){
	   field=event.field;
   };
   wxDbFieldEvent(wxEventType commandEventType, int id=0);
   virtual wxString GetValue1(){return value1;}
   virtual wxString GetValue2(){return value2;}
   virtual void SetValue(const wxString& v1,const wxString& v2){value1=v1; value2=v2;}
   virtual wxEvent *Clone() const { return new wxDbFieldEvent(*this); }
	wxEvtHandler *field;
	wxString value1,value2;
	///��� �������
	int eType;
};


///������� ��� �������� ������ ����� ������ � ������, � ������������  ��������� ���������
BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE(dbEVT_COMMAND_REFRESH_FIELD,7001)
	DECLARE_EVENT_TYPE(dbEVT_COMMAND_FIELD_CHANGED,7002)
	DECLARE_EVENT_TYPE(dbEVT_COMMAND_FIELD_CMND,7003)
END_DECLARE_EVENT_TYPES()


typedef void (wxEvtHandler::*wxDbFieldEventFunction)(wxDbFieldEvent&);
 
 #define wxDbFieldEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxDbFieldEventFunction, &func)



 #define EVT_REFRESH_FIELD( winid, func) \
    wx__DECLARE_EVT1(dbEVT_COMMAND_REFRESH_FIELD, winid, wxDbFieldEventHandler(func))


#define EVT_FIELD_CHANGED( winid, func) \
    wx__DECLARE_EVT1(dbEVT_COMMAND_FIELD_CHANGED, winid, wxDbFieldEventHandler(func))

#define EVT_FIELD_CMND( winid, func) \
    wx__DECLARE_EVT1(dbEVT_COMMAND_FIELD_CMND, winid, wxDbFieldEventHandler(func))



#endif __dbfield__