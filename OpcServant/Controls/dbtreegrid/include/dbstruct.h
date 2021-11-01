/////////////////////////////////////////////////////////////////////////////
// Name:        dbstruct.h
// Purpose:     Classes to describe the data structures
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

//������� ��� ������������� ��������� ������ ������
#ifndef __dbstruct__
#define __dbstruct__
#include <wx/wx.h>
#include <wx/grid.h>
#include "db.h"
#include <wx/dynarray.h>
#include "dbfield.h"
#include <wx/validate.h>




///������ ������� ���������
WX_DECLARE_OBJARRAY( wxArrayString, ArrayDbRec );



/**
@class wxDbSerializer
����� ��� ������ � ������ �������� GUI � ������
*/
///����� ��� ������ � ������ �������� GUI � ������
class wxDbSerializer
{
	public:
		///�����������
		wxDbSerializer(){};
		~wxDbSerializer(){};
	///��������� ��������� GUI � ������
	virtual bool SaveGUI(ArrayDbRec &ar,wxString &pName){return true;};
	///��������� ��������� GUI �� �������
	virtual bool LoadGUI(ArrayDbRec &ar,wxString &pName){return true;};
	///�������� �������� � ������
	static bool SavePar(ArrayDbRec &ar,wxString &pname,wxString &name,wxString &parname, wxString &val);
	///������� �������� �� �������
	static wxString LoadPar(ArrayDbRec &ar,wxString &pname,wxString &name,wxString &parname);
	///��������� ���������� � ������� ���� � �������, ������������� �� �����
	static bool SaveWindowGUI(wxWindow *w,ArrayDbRec &ar,wxString &pName);
	///��������� ���������� � ������� ���� �� �������, ������������� �� �����
	static bool LoadWindowGUI(wxWindow *w,ArrayDbRec &ar,wxString &pName);

};


class wxDbDispatcher;

/**
@class wxDbPair
����� ������������ �������, ������������ � wxDbLOV
� ����� ������ ���� � LOV, ���� i2==-1 �� ��������� ������ ������
*/
///����� ������������ �������, ������������ � wxDbLOV
class wxDbPair
{
public:
	///�����������
	wxDbPair(){i2=-1;w=-1;};
	///����� ������� � LOV 
	int i1;
	///����� ������� � ���������� �����
	int i2;
	///������ ������ � LOV
	int w;
	///��� ���� � ��
	wxString fieldName;
};

WX_DECLARE_OBJARRAY( wxDbPair, ArraywxDbPair );

/**
@class wxDbLOV
����� ��������� ��� ������ ������ �������� �� ��
*/
///����� ��������� ��� ������ ������ �������� �� ��

class wxDbLOV
{
public:
	///�����������
	wxDbLOV(){qTime=true;};
	///������ ������� ������� � select �� ����� from
	wxString selectClause;
	///����� ������� ������� � from �� ����� where
	wxString fromClause;
	///����� ������� ������ � where �� ����� order 
	wxString whereClause;
	///����� ������� ������� � order � �� �����
	wxString orderClause;
	wxString query;
	///������ ������� LOV
	wxSize size;
	///��������� ����� LOV
	ArraywxDbPair pair;
	///��������� ������ ����� �� ����� - true ��� ����� ����� ������ ������
	bool qTime;

};

WX_DECLARE_OBJARRAY( wxDbLOV, ArraywxDbLOV );




const int 		DB_FIELD_EDITABLE=0x1;
const int 		DB_FIELD_VISIBLE=0x2;
const int		DB_FIELD_KEY=0x4;//���� ���� �������
const int 		DB_FIELD_TREE=0x8;//���� ������ �� �������� ��� ������
const int		DB_FIELD_DBASE=0x10;
///�������� ������ 
const int		DB_FIELD_QUERY=0x20;
///����� ��������
const int		DB_FIELD_SEARCH=0x40;
///������� ����
const int		DB_FIELD_FK=0x80;
///���������������� LOV
const int		DB_FIELD_USER_LOV=0x100;
///�� ������
const int		DB_FIELD_NOT_NULL=0x200;
///������� �������������� ����� LOV ��� � �����
const int		DB_FIELD_LOV_AND_EDIT=0x400;

///���� �������� ��� ����

const int DB_FIELD_STRING=0;
const int DB_FIELD_INT=1;
const int DB_FIELD_FLOAT=2;
const int DB_FIELD_DATE=3;
const int DB_FIELD_LIST=4;
const int DB_FIELD_BOOL=5;
const int DB_FIELD_FLAGS=6;//���� ������� ������  
const int DB_FIELD_DATETIME=7;




///���� ��������� ������
const int DB_QUERY_RECORD=0x1;//������ �������� �� �������
const int DB_INSERTED_RECORD=0x2;//������ ��� �������
const int DB_DELETED_RECORD=0x4;//��������� ������
const int DB_UPDATED_RECORD=0x8;//���������� ������
const int DB_TREE_QUERY=0x10;//�� ���� ��������� ����������� ������




/**
@class wxDbGridFieldDesc
��������� ���� ��� �����  @n

��������� �������� ���� @n
DB_FIELD_EDITABLE=0x1; @n
DB_FIELD_VISIBLE=0x2; @n
���� ���� �������  DB_FIELD_KEY=0x4; @n
���� ������ �� �������� ��� ������ DB_FIELD_TREE=0x8; @n
DB_FIELD_DBASE=0x10;@n
�������� ������ DB_FIELD_QUERY=0x20; @n
����� �������� DB_FIELD_SEARCH=0x40; @n
������� ����	DB_FIELD_FK=0x80; @n
���������������� LOV		DB_FIELD_USER_LOV=0x100; @n
�� ������	DB_FIELD_NOT_NULL=0x200; @n

���� �������� ��� ���� @n

DB_FIELD_STRING=0;@n
DB_FIELD_INT=1;@n
DB_FIELD_FLOAT=2;@n
DB_FIELD_DATE=3;@n
DB_FIELD_LIST=4;@n
DB_FIELD_BOOL=5;@n
���� ������� ���� DB_FIELD_FLAGS=6; @n
DB_FIELD_DATETIME=7;@n

*/
///��������� ���� ��� ����� 
class wxDbGridFieldDesc {
public:
	
	///�����������
	wxDbGridFieldDesc(){boolProperty=(int)DB_FIELD_EDITABLE|DB_FIELD_VISIBLE|DB_FIELD_QUERY|DB_FIELD_SEARCH; valueType=DB_FIELD_STRING;lov=0;validator=0;minLen = 0; presition = 0;};
	///�����������
	wxDbGridFieldDesc(const wxString& label,const wxString& fname,unsigned long flag=DB_FIELD_EDITABLE|DB_FIELD_VISIBLE,
		int tp=DB_FIELD_STRING);
	~wxDbGridFieldDesc(){};
	///�������� ���� �������
	void SetVisible(bool value){  (value ) ? boolProperty|=DB_FIELD_VISIBLE :  boolProperty&=~DB_FIELD_VISIBLE;}
	bool GetVisible(){ if (boolProperty&DB_FIELD_VISIBLE) return true; else return false;}
	///���������� �������� ���� - �������������
	void SetEditable(bool value){  (value ) ? boolProperty|=DB_FIELD_EDITABLE :  boolProperty&=~DB_FIELD_EDITABLE;}
	bool GetEditable(){return boolProperty&DB_FIELD_EDITABLE;}
	///���������� ��� ������ ��� ����
	void SetFieldValueType(int value){valueType=value;}
	int GetFieldValueType(){return valueType;}
	///���������� �������� ���� � ��
	void SetFieldName(wxString value){fieldName=value;}
	wxString& GetFieldName(){return fieldName;};
	///���������� �������� ������� � �����
	void SetColumnName(wxString value){columnName=value;}
	wxString& GetColumnName(){return columnName;};
	//�� ����� ???
	//void SetColumnOperator(wxString value){columnOperator=value;}
	//wxString& SetColumnOperator(){return columnOperator;};
	///���������� ����� ����, ��� ��������� - ���������� ��������, ��� ����� ����� , ��� ������������ ���������� �����
	void SetLength(int value){len=value;}
	int GetLength(){return len;}
	///���������� �������� ��� ������������ �����
	void SetPresition(int value){presition=value;}
	int GetPresition(){return presition;}
	
	///������ ������ �������� ��� ���� ���� ������, ������������ � �����
	void SetListValues(wxArrayString& value){values=value;}
	wxArrayString& GetListValues(){return values;}
	///������ ������ �������� �������� ��� ������ , �� ��
	void SetListValues2(wxArrayString& value){values2=value;}
	wxArrayString& SetListValues2(){return values2;}
	///�������� ��������
	unsigned long GetProperty(){return boolProperty;};
	///���������� ��������
	void SetProperty(unsigned long property){  boolProperty|=property ;}
	///�������� ��������
	void ResetProperty(unsigned long property){   boolProperty&=~property;}




	/// ��� ���� � ��
	wxString fieldName;
	///��� ���� � �����
	wxString columnName;
	///����� �������� ���� �������� ����� ������� ������   SUM(price) total, ���� �� ������������
	wxString columnOperator;
	///��� �������� ����
	int valueType;
	///�������� ��� ������ (��� ���� DB_FIELD_LIST)
	wxArrayString values; 
	///�������� ��� ������  (��� ���� DB_FIELD_LIST)
	wxArrayString values2; 
	/// field length
	int len;
	///����������� ����� ���� 
	int minLen;
	///���������� ������ ����� �������
	int presition;
	///������ �������� ��� ����(��� ���� DB_FIELD_CHOICE)
	wxDbLOV *lov;
	///�������� �� ���������
	wxString defValue;
	///������������ ��������
	wxString maxVal;
	///����������� ��������
	wxString minVal;
	///���������
	wxValidator *validator;
private:
	//�������� ����
	unsigned long boolProperty;

	




};


WX_DECLARE_OBJARRAY( wxDbGridFieldDesc, ArraywxDbGridFieldDesc );


class wxDbTreeGrid;


const int DB_SAVE_HANDLE=1;
const int DB_PRE_INSERT_HANDLE=2;
const int DB_ON_INSERT_HANDLE=3;
const int DB_POST_INSERT_HANDLE=4;
const int DB_PRE_UPDATE_HANDLE=5;
const int DB_ON_UPDATE_HANDLE=6;
const int DB_POST_UPDATE_HANDLE=7;
const int DB_PRE_DELETE_HANDLE=8;
const int DB_ON_DELETE_HANDLE=9;
const int DB_POST_DELETE_HANDLE=10;
const int DB_POST_QUERY_HANDLE=11;
const int DB_POST_RECORD_HANDLE=12;
const int DB_PRE_RECORD_HANDLE=13;
const int DB_DELETE_HANDLE=14;
const int DB_CUSTOM_LOV_HANDLE=15;
const int DB_POST_FIELD_CHANGED_HANDLE=16;


const int DB_OBJ_TYPE_TREE_GRID=1;
class wxDbQueryObject;


/**
@class wxDbRecEvent
����� ������� ��� ������������ ������� � ��
*/
///����� ������� ��� ������������ ������� � ��
class wxDbRecEvent : public wxNotifyEvent{
public:
   wxDbRecEvent():wxNotifyEvent(){as=0;objType=0;tp=0;veto=false;obj=0;};
   wxDbRecEvent(const wxDbRecEvent& event)
	   : wxNotifyEvent(event){
	   as=event.as;
	   tp=event.tp;
   };
   wxDbRecEvent (wxEventType commandEventType, int id=0);
   virtual wxEvent *Clone() const { return new wxDbRecEvent(*this); }
public:
	///������ � ������ �������������� ������
	wxArrayString *as;
	///��� ������� �������� wxDbTreeGrid
	int objType;
	///��� �������
	int tp;
	///���� ���� ���������� ������������ �� ������ ������ �������� ���������� ���������� (��� ������������ ��������� � ������� � ��)
	bool veto;
	wxDbQueryObject *obj;
};


/**
@class wxDbGridEvent
����� ������� ��� �����
*/
///����� ������� ��� �����

class wxDbGridEvent : public wxNotifyEvent{
public:
   wxDbGridEvent():wxNotifyEvent(){grid=0;};
   wxDbGridEvent(const wxDbGridEvent& event)
	   : wxNotifyEvent(event){
	   grid=event.grid;
   };
   wxDbGridEvent (wxEventType commandEventType, int id=0);
   virtual wxEvent *Clone() const { return new wxDbGridEvent(*this); }
public:
	///����
	wxDbQueryObject *grid;
};




BEGIN_DECLARE_EVENT_TYPES()

    DECLARE_EVENT_TYPE(dbEVT_REC_HANDLE, 7100)
	DECLARE_EVENT_TYPE(dbEVT_GRID_ACTIVE_HANDLE, 7103)


END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*dbRecEventFunction)(wxDbRecEvent&);
typedef void (wxEvtHandler::*dbGridEventFunction)(wxDbGridEvent&);

 #define dbRecEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(dbRecEventFunction, &func)
///���������� ������� ��������� � �������
 #define EVT_REC_HANDLE( winid, func) \
    wx__DECLARE_EVT1(dbEVT_REC_HANDLE, winid, dbRecEventHandler(func))

#define dbGridEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(dbGridEventFunction, &func)
///���������� ������� ��������� � ���������� �����
#define EVT_GRID_ACTIVE_HANDLE( winid, func) \
    wx__DECLARE_EVT1(dbEVT_GRID_ACTIVE_HANDLE, winid, dbGridEventHandler(func))


/// �������� ����� ������
	const int DB_GRID_INSERT_ENABLE=0x1;
	const int DB_GRID_UPDATE_ENABLE=0x2;
	const int DB_GRID_DELET_ENABLE=0x4;
	const int DB_GRID_DELETE_ENABLE=0x4;
	///���� ���� ������
	const int DB_GRID_IS_TREE=0x8;
	///���� ��������� ���� ��������
	const int DB_GRID_IS_CHANGED=0x10;
	///��������� ��� ������ �� �� �� ���� ��������
	const int DB_GRID_READ_ONE=0x20;
	///��� ������ �������� �� ��
	const int DB_GRID_ALL_RECORD_FETCHED=0x40;
	///������ � �� ��������
	const int DB_GRID_QUERY_ENABLE=0x80;
	///��������� ��������� ������ ��������
	const int DB_GRID_REFRESH_ENABLE=0x100;
	///������������ ������� ���������
	const int DB_GRID_DUBLICATE_ENABLE=0x200;
/// ���������� ������� ����������� �� �� �� ���� ��������
const int NEXT_NUMBER_ROWS=10;

/**
@class wxDbQueryObject
����� ����� ����� � ���� @n

���� ��������� ������ @n

������ �������� �� ������� DB_QUERY_RECORD=0x1;@n
������ ��� ������� DB_INSERTED_RECORD=0x2;@n
��������� ������ DB_DELETED_RECORD=0x4;@n
���������� ������ DB_UPDATED_RECORD=0x8;@n
�� ���� ��������� ����������� ������ const int DB_TREE_QUERY=0x10;@n

�������� ����� ������ @n

DB_GRID_INSERT_ENABLE=0x1; @n
DB_GRID_UPDATE_ENABLE=0x2; @n
DB_GRID_DELET_ENABLE=0x4; @n
���� ���� ������ DB_GRID_IS_TREE=0x8; @n
���� ��������� ���� �������� DB_GRID_IS_CHANGED=0x10; @n
��������� ��� ������ �� �� �� ���� ��������  DB_GRID_READ_ONE=0x20; @n
��� ������ �������� �� �� DB_GRID_ALL_RECORD_FETCHED=0x40; @n
������ � �� �������� DB_GRID_QUERY_ENABLE=0x80; @n
��������� ��������� ������ ��������  DB_GRID_REFRESH_ENABLE=0x100; @n

*/
///����� ����� ����� � ����
class wxDbQueryObject : public wxDbSerializer {

public:
	///�����������
	wxDbQueryObject()
	{
		db=0;dbq=0;boolProperty=DB_GRID_DUBLICATE_ENABLE|DB_GRID_INSERT_ENABLE|DB_GRID_UPDATE_ENABLE|DB_GRID_DELET_ENABLE|DB_GRID_READ_ONE|DB_GRID_ALL_RECORD_FETCHED|DB_GRID_QUERY_ENABLE|DB_GRID_REFRESH_ENABLE;pkNum=-1;treeNum=-1;fkNum=-1;dstName=wxT("");
		saveHandler=0;
		preInsertHandler=0;
		onInsertHandler=0;
		postInsertHandler=0;
		preUpdateHandler=0;
		onUpdateHandler=0;
		postUpdateHandler=0;
		preDeleteHandler=0;
		onDeleteHandler=0;
		postDeleteHandler=0;
		postQueryHandler=0;
		postRecordHandler=0;
		preRecordHandler=0;
		deleteHandler=0;
		customLOVHandler=0;
		postFieldChangedHandle=0;
		parent=0;
		dispatcher=0;
		rcount = 0;
		tree = 0;
	};
	~wxDbQueryObject();
	///�������� ���������� �������, ��� ����������� �����
	virtual wxEvtHandler* GetEvtHandler(){return 0;} 
protected:
	///�������� ����� ������
	unsigned long boolProperty;

	
	///������ �� ���������� ������ 
	ArraywxDbFieldLink linkFields;
	///���������� ������� � �����
	int rcount;
	///������� ������
	wxDbTreeGrid *tree;
	///������ ������� ��������
	ArraywxDbLOV lov;
	/// ������ ��������� �������
	ArrayDbRec recArray;
public:
	///������ ������� ������� � select �� ����� from
	wxString selectClause;
	///����� ������� ������� � from �� ����� where
	wxString fromClause;
	///����� ������� ������ � where �� ����� order 
	wxString whereClause;
	///����� ������� ������� � order � �� �����
	wxString orderClause;
	///������� ���� ���������� ������
	wxString dstName;
	///�������� ��� ������� ��������� �������������
	wxArrayString queryData;
	///�������� ��� ������ ��������� �������������
	wxArrayString searchData;
	///�������� ����� where  ��� ������
	wxString queryDataWhere;
	///��������������� ������ ����� createQuery(). ����� ����� ���� ��������� ������ �� ����� �� ����� �������� ������ � ��������������
	///������ �� ����� ���������� ������������� ���� fromClause �����
	///���� ���������� ��� ����� ���������� ���� ������� �� ����� ������������!!! �������� ����� ������������ �� ___
	wxString query;
	///�� ����������
	DatabaseLayer* db;
	///��������� �������	
	DatabaseResultSet* dbq;
	///����������� ������� ������  wxEvtHandler ������������ ����� ProcessEvent ��� ������ ������������ ��������� � ��������� ��������� � ��
	///���������� ����������, ���������� ������ ������������, ������������ � �������� ����� 
	///��������� ��������� ������ � ���� ��������� ���������� � ������������ �������
	wxEvtHandler *saveHandler;
	///���������� ����� �������� ������ � ��
	wxEvtHandler *preInsertHandler;
	///���������� ������ ����������� ��������� ������� ������
	wxEvtHandler *onInsertHandler;
	///���������� ����� ������� ������
	wxEvtHandler *postInsertHandler;
	///���������� ����� ����������� ������
	wxEvtHandler *preUpdateHandler;
	///���������� ������ ����������� ��������� ������� ������
	wxEvtHandler *onUpdateHandler;
	///���������� ����� ���������� ������
	wxEvtHandler *postUpdateHandler;
	///���������� ����� ��������� ������
	wxEvtHandler *preDeleteHandler;
	///���������� ������ ����������� ��������� �������� ������
	wxEvtHandler *onDeleteHandler;
	///���������� ����� �������� ������
	wxEvtHandler *postDeleteHandler;
	///���������� ����� ��������� ��������� ������ �� ��
	wxEvtHandler *postQueryHandler;
	///���������� ����� ����� � ������(�������� ������ � ������� ������)
	wxEvtHandler *postRecordHandler;
	///���������� ��� ������ ����� ������ (�������� ������ ������� �������)
	wxEvtHandler *preRecordHandler;
	///���������� ��� �������� ������ �������������
	wxEvtHandler *deleteHandler;
	///���������� ����������������� LOV
	wxEvtHandler *customLOVHandler;
	///���������� ��������� �������� ����, ���������� ����� ��������� �������� ����� ���������
	wxEvtHandler *postFieldChangedHandle;

	///��� sequence ��� PK
	wxString seqName;
	///������ �� ������������ ����(������������ ��� ������������ FK)
	wxDbQueryObject *parent;


	///���������
	wxDbDispatcher *dispatcher;
	///���������� ���� ��� ��������������
	wxMenu * menu;
	///����� ���� ���������� �����
	int pkNum;
	///����� ���� ����� �� �������� � ������
	int treeNum;
	///����� ���� ������ �� ������������ ����
	int fkNum;

	///���������� ������� ���������� �� ���� �������� ����������, ����� �� ����������� ��-�� �������� ��� ������ �� ���
	int nRows;

	///�������� �����

	///�������� �����, ���� ������ �������, ����� �� ��������� ������ ����� ��������� select �����.
	///���� �������� ������ ����� �� ������� c ������� setupField()
	
	ArraywxDbGridFieldDesc fieldsDescription;	
public:
	///���������� ��������� �����
	virtual void SetFieldDescription(ArraywxDbGridFieldDesc& value){fieldsDescription=value;};
	///���������� ��������� �����
	virtual void SetFieldDescription(ArraywxDbGridFieldDesc* value){fieldsDescription=*value;};
	///���������� ��������� �����
	virtual void AddFieldDescription(wxDbGridFieldDesc* value){fieldsDescription.Add(value);};
	///��������� ������� �������� �����
	virtual ArraywxDbGridFieldDesc* GetFieldDescription(){return &fieldsDescription;};
	///������������ ���������� ����� �� �������
	virtual bool SetupField();
	///��������� ������ ������� �� ������� �����
	virtual void FillRecord(wxArrayString&,  long ){};
	///���������� ��������
	virtual void SetProperty(unsigned long property){  boolProperty|=property ;}
	///�������� ��������
	virtual void ResetProperty(unsigned long property){   boolProperty&=~property;}
	///�������� ������������ �������� 
	virtual bool GetProperty(unsigned long property){return (boolProperty&property)>0;}
	///�������� ��� ��������
	virtual unsigned long GetProperty(){return boolProperty;}
	///���������� ������� ��� ������
	virtual void SetDstName(const wxString& s){dstName=s;}
	///������� ������� ��� ������
	virtual wxString GetDscName(){return dstName;}
	///�������� ������� �� ������ �������� � selectClause fromClause...
	virtual void CreateQuery();
	///�������� ������� ��� LOV
	virtual void CreateLOVQuery();
	///�������� selectClause �� ��������� �������� �����
	virtual void CreateSelectClause();
	///��������� SQL ������
	virtual void ExecuteQuery();
	///��������� ��������� ������ �� �� ���� ������ ������� �� ���������� true
	virtual bool GetNextRecord(wxArrayString& lst);
	///��������� ������ ������� �������� ��������� 
	virtual bool ProcessRecord(wxArrayString& as,wxDbTreeGrid *gr);
	///�������� ������������ �� ������ ��������� ������ 
	virtual bool CheckSearch(wxArrayString as);
	///�������� ������
	bool DoUpdate(const wxArrayString& as);
	///�������� ������
	bool DoInsert(const wxArrayString& as);
	///������� ������
	bool DoDelete(const wxArrayString& as);
	///����������� ���������� ����� �������� � �� (����������� PK � FK)
	bool DoPreInsert(const wxArrayString& as);

	///�������� ���������� �������������� �������
	int GetRowCount(){ return rcount; };
	///�������� ����� ���� ���������� �����
	int GetPkNum();
	///���������� ����� ���� ���������� �����
	void SetPkNum(int nf);
    ///�������� ����� ���� �������� �����
	int GetFkNum();
	///���������� ����� ���� �������� �����
	void SetFkNum(int nf);
    ///�������� ����� ���� ������ �� ������
	int GetTreeNum();
	///���������� ����� ���� ������ �� ������
	void SetTreeNum(int nf);

	///�������� �������� PK
	virtual wxString GetPkValue(){return wxEmptyString;};
	///������� �������� ���� ��� �������
	wxString GetColumnValueForSQL(const wxArrayString& as, int nf);

	///�������� �� ��������� ������ ������� � �������� �� ������
	bool IsTree(){return (boolProperty&DB_GRID_IS_TREE)>0;};

	///���� �� ��������� ��� ������ � ��
	bool HasUpdate(){return (boolProperty&DB_GRID_IS_CHANGED)>0;};

	///�������� �������� ����
	virtual bool CheckValue(const wxString& s, int nf);
	///���������� LOV
	virtual void SetLOV(ArraywxDbLOV& value){lov=value;};
	virtual wxDbLOV* AddLOV(wxDbLOV& value){lov.Add(value); return &(lov[lov.Count()-1]);}
	///�������� ������ � ������� �������
	virtual void GetCurrentRecord(wxArrayString& as){};
	///�������� ��� ������ , ��������  � ��������� �������� ��������� ����� ������ � �� (������ ������� �� �������������������)
	virtual void Reset(){};
	///��������� ������ �� ��, ���� ��� ���� ����� � ����������� �� ��������
	virtual bool GetRows(){return false;}; 
	///������� �������� ������ ������ ����������� ����� ������ �� ����������, ������������ ��� �������������
	virtual bool SelectFirstRecord(){return false;}
	///��������� ������
	virtual bool Save(){return false;};
	///��������, ���� �� ������ � �����
	virtual bool IsRecords(){return false;}
	///������������� ����
	virtual bool IsEdit(){return false;}
	///������ �������������� ����
	virtual void StopEdit(){};
	///���������� ����� �� �������
	virtual bool FillGrid(ArrayDbRec &ar){return false;}
	///�������� ��������� ����

	///���������� ��� �� �������������  ���� ��� ����� � ������
	virtual void AddLinkField(wxDbField *f, int nf){};
	///���������� �������� ��������� ����� �� ������� ������
	virtual void RefreshLinkField(){};
	///�������� ������ �� ������

	///���������� ������ � ����� ���� ������
	virtual void CheckRecord(wxArrayString& as, int *nf, bool *status);

	///
	virtual  SetFocus_(){};

	///�������� ������� � ������� ������ , ���� �������� ����� �� �������� ���� � ������ ��� ����, ����� ��������� ����� � ���� ���������� ������
	virtual void AddWhere(wxString &s);
	///�������� ����� � ���� �������
	virtual void ResetFlag(){};
	///��������� ��������� GUI � ������
	virtual bool SaveGUI(ArrayDbRec &ar,wxString &pName){return true;};
	///��������� ��������� GUI �� �������
	virtual bool LoadGUI(ArrayDbRec &ar,wxString &pName){return true;};
	///�������� ������ k������ �� �������� , ���������� -1 ���� �� �������
	int GetFieldIndex( wxString s );
	///���������� �������� ���� 
	bool SetFieldPropertyByName(wxString fn, unsigned long property);
	///�������� �������� ����
	bool ResetFieldPropertyByName(wxString fn, unsigned long property);
	
	///���������� �������� ������� �� �����
	bool SetColumnNameByName(wxString fn, wxString name);

	///���������� Pk �� �����
	bool SetPkNumByName(wxString fn);
	///���������� Fk �� �����
	bool SetFkNumByName(wxString fn);
	///���������� TreeKey �� �����
	bool SetTreeNumByName(wxString fn);

	///���������� LOV �� �����
	bool SetLOVByName( wxString fn, wxDbLOV *lov );

	///���������� ���������� ����
	bool AddLinkFieldByName( wxString fn, wxDbField *f);

	///��������� ������ ����
	virtual bool SetColumnWidthByName( wxString fn, int val){return false;}
	///���������� ��� �������� ���� �� ����� - int valueType;
	bool SetValueTypeByName(   wxString fn, int val);
	///���������� �������� ��� ������ �� ����� (��� ���� DB_FIELD_LIST) - wxArrayString values; 
	bool SetValuesByName(    wxString fn, wxArrayString val );
	///���������� �������� ��� ������ �� �����  (��� ���� DB_FIELD_LIST) - 	wxArrayString values2; 
	bool SetValues2ByName(    wxString fn, wxArrayString val );
	///����������  ����� ���� �� ����� -	int len;
	bool SetLenByName(   wxString fn, int val);
	///���������� ����������� ����� ���� �� �����  -	int minLen;
	bool SetMinLenByName(   wxString fn, int val);
	///���������� ���������� ������ ����� ������� �� ����� -	int presition;
	bool SetPrecisionByName(   wxString fn, int val);
	///���������� �������� �� ��������� �� �����  -	wxString defValue;
	bool SetDefValueByName( wxString fn, wxString val );
	///���������� ������������ �������� �� ����� -	wxString maxVal;
	bool SetMaxValueByName( wxString fn, wxString val );
	///���������� ����������� �������� �� ����� -	wxString minVal;
	bool SetMinValueByName( wxString fn, wxString val );
	///���������� ��������� �� ����� - 	wxValidator *validator;
	bool SetValidatorByName( wxString fn, wxValidator *validator );


};
///////////////////////////


/////////////////////////////


///������ ������


#endif