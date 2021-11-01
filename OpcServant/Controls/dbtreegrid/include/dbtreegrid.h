/////////////////////////////////////////////////////////////////////////////
// Name:        dbtreegrid.h
// Purpose:     Control for edit table data
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

//���� ��� �������������� � ��������� ������ � ���� ������
#ifndef __dbtreegrid__
#define __dbtreegrid__
#include "dbtreelistctrl.h"

#include "dbstruct.h"
#include "dbpopupmenu.h"

/**
@class wxDbTreeGrid
����� ��������� � �������������� ������ �� ����.
���� � ���� �������, ���� � ���� ������
*/
/// ����� ��������� � �������������� ������ �� ����


class wxDbTreeGrid : public wxDbTreeListCtrl, public wxDbQueryObject {
public:
	wxDbTreeGrid (wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTR_DEFAULT_STYLE, const wxValidator& validator = wxDefaultValidator, const wxString& name = "treelistctrl");
	~wxDbTreeGrid();
	///�������� ����� ����� �� ������ �������� �� wxDbQueryObject
	virtual void CreateField();
	///��� ������ ��� ����� � ���� ������ ��� � ���� �������
	bool isTree;
	///�������� ��� ���� ������ �� ���� � ��������� � ����
	virtual bool GetNextRow();
	///�������� ��� ������ �� ���� � ��������� � ����
	virtual bool GetAllRow();
	///��������� ������ �� ��, ���� ��� ���� ����� � ����������� �� ��������
	virtual bool GetRows(); 
	///������� ������� ������
	virtual bool DeleteCurrentRecord();
	///�������� ������
	virtual bool InsertRecord();
	///�������� �������� ������
	virtual bool InsertChildRecord();

	///�������� ������ � ������� �������
	virtual void GetCurrentRecord(wxArrayString& as);
	///�������� ������ ��  �������
	virtual void GetCurrentRecord(wxArrayString& as,wxTreeItemId & id);
	///��������� ���� ��� �������������� ������
	virtual void SetMenu(wxMenu *m){menu=m;};
	///�������� ��� ��������� � ��
	virtual bool Save();
	///���������� ������ �� ������� �����
	virtual void FillRecord(wxArrayString& as,wxTreeItemId & id);
	///��������� ���������� ��� ����������� ��������� �������
	virtual void ShowCol(int nf);
	///����� ��������� ��� ������
	virtual void EditCell(const wxTreeItemId & id, int col, wxKeyEvent &event);
	///����������� ��������� ��������� ������
	virtual void GetRect(const wxTreeItemId & id, int col, int &x, int &y, int &w, int &h);
	///��������� ����, ������������ ������ �� ��
	virtual void RefreshRecords();
	///�������� ����� �� ������� ���� , � ���������������� � ��������, ���� ����� ������������� ���� � � ���� ������� ������������ ������
	virtual bool PrepareClose();
	///������� ���� ����� ������� � ��������� ���
	virtual bool Query();
	///������� ���� ����� ������ � ��������� ���
	virtual bool Search();
	///������� ���� ����� ������ � ��������� ���
	virtual bool SearchRecord();
	///��������� �����
	virtual void Setup( wxDbQueryObject& obj, int property);
	///�������� ��� ������ , �������� ���� � ��������� �������� ��������� ����� ������ � �� (������ ������� �� �������������������)
	virtual void Reset();
	///������� �������� ������ ������ � �����
	virtual bool SelectFirstRecord();
	///�������� �������� PK
	virtual wxString GetPkValue();
	///��������, ���� �� ������ � �����
	virtual bool IsRecords();
	///������������� ����
	virtual bool IsEdit()
		{if(GetMainWindow()->m_editControl)return true; else return false;}
	///������ �������������� ����
	virtual void StopEdit();

	///���������� ����� �� �������
	virtual bool FillGrid(ArrayDbRec &ar);

	///�������� ������� �� ����� ����
	virtual bool MenuItemIsActive(int id);
	///�������� ��������� ����
	virtual void AddLinkField(wxDbField *f, int nf);
	///���������� �������� ��������� ����� �� ������� ������
	virtual void RefreshLinkField();

	///��������� ��������� GUI � ������
	virtual bool SaveGUI(ArrayDbRec &ar,wxString &pName);
	///��������� ��������� GUI �� �������
	virtual bool LoadGUI(ArrayDbRec &ar,wxString &pName);

	///������� ���������� �������� �������������� �������� �������
	virtual void CallHandler(wxEvtHandler* handler, int tp);
	
	///������� �������������� ������
	wxTreeItemId curId;
	///������ ������ ��� ����������� ��� ������ ������ ��������� ������ � ���� �������� ������
	wxTreeItemId EmptyId;
	///��������� ����� (���������� , ���������� ������� ����)
	void SetMenuItems();
	///�������� ���������� ������� � �����

	///���������� ����� ��������  �������  � ����������� ������
	void RowRecalc();
	///�������� ������ � ������� ��������
	wxString GetCurPos();
	///�������� ������� ������
	wxTreeItemId GetCurrentItem(){ return eId;}
	///��������� �������� ���� ������� ������
	void SetFlagsValue(wxTreeItemId & id,int v);
	//��������� ������ ���� �� �����
	virtual bool SetColumnWidthByName( wxString fn, int val);


protected:
	///���������� �������� �������� ������
	void OnShowProperty(wxCommandEvent & event);
	//������ ���������� �������� �� ����������� ��� HTML
	void ProcessStr( wxString &s );

	///���������� �������� ������
	void OnDeleteRecord(wxCommandEvent & event);
	///���������� ������� ������
	void OnInsertRecord(wxCommandEvent & event);
	///���������� ����������
	void OnSaveRecord(wxCommandEvent & event);
	/// ���������� �������������� ������
	void OnEditRecord(wxCommandEvent & event);
	/// ���������� ������ ������
	void OnSelectItem(wxTreeEvent &event);
	/// ���������� �������
	void OnQueryItem(wxTreeEvent &event);
	///���������� ������� �������� ������(��� ������)
	void OnInsertChildRecord(wxCommandEvent & event);
	///���������� ������������ ������
	void OnDuplicateRecord(wxCommandEvent & event);
	///���������� �������� �������� ���������� ����
	void OnwxDbFieldChanged(wxDbFieldEvent &event);
	///���������� ������� �� ��������� ����� (SetFocus ,LOV..)
	void OnwxDbFieldCmnd(wxDbFieldEvent &event);
	///���������� ������ ��������������
	void OnBeginDrag(wxTreeEvent &event);	
	///���������� ����� ��������������
	void OnEndDrag(wxTreeEvent &event);	
	///���������� ������
	void OnKey(wxKeyEvent &event);
	/// ���������� ������ �������� ���������
	void OnExternEditor(wxCommandEvent & event);

	void OnKeyDown(wxKeyEvent &event);
	///���������� ��������� ��������������
	void OnEndEdit(wxTreeEvent &event);
	///������� ������ ��� �������������� ����
	void EditDate(const wxTreeItemId & id, int col);
	///������� ������ ��� �������������� ���� ������
	void EditList(const wxTreeItemId & id, int col);
	///�������������� ��������� ����
	void EditBool(const wxTreeItemId & id, int col);
	///������� ������ ��� �������������� ���������� ��� ��������� ����
	void EditText(const wxTreeItemId & id, int col,wxKeyEvent &event);
	///������� ������ ��� ������ �������� �� ������ (������ �������� �� ��)
	void EditChoice(const wxTreeItemId & id, int col);
	///������� ������ ��� ������ �������� �� ������ ������������ ������������
	void EditCustomChoice(const wxTreeItemId & id, int col);

	///��������� ���������� ����� ������������ ������ �� ��
	void OnRefreshRecord(wxCommandEvent & event);
	///���������� ����� �������
	void OnQueryRecord(wxCommandEvent & event);
	/// ���������� ������
	void OnSearchRecord(wxCommandEvent &event);
	/// ���������� ������ ��������� ������
	void OnSearchRecordNext(wxCommandEvent &event);

	///�������� �� ����������� �� ��������� ������� � ������������ �� ��
	void FetchNext();

	void OnTextEnter(wxCommandEvent &event);

	///���������� ������� �� ��������� ����
	void OnExpanded(  wxTreeEvent &event );


	///���������� ������� ������ ������ ����
	void OnMouseRightClick(wxMouseEvent& event);

	///���������� ������� ������� ������ ����
	void OnMouseDoubleClick(wxMouseEvent& event);


	///�������� �� ���� ��� ������ ��� ������
	bool GetAllRowTree(wxTreeItemId& parent);
	///�������� �������� Tree ��� �������� ������
	wxString GetItemTreeValue(wxTreeItemId& id);


	virtual  SetFocus_(){GetMainWindow()->SetFocus(); };
	virtual wxEvtHandler* GetEvtHandler(){return this;} 
	///����������� ���� ������
	bool MoveTreeNode(wxTreeItemId &src,wxTreeItemId &dst);

	///����������� �������� ���� ������ (��������)
	bool Reparent(wxTreeItemId &src,wxTreeItemId &dst);

	///����������� ������ �� ����� ������ � ������
	void CopyRow(wxTreeItemId &src,wxTreeItemId &dst);
	///�������� ����� � ���� �������
	virtual void ResetFlag();
	///������� ������� ������� ������ �� ��
	bool GetTreeLevel(wxTreeItemId &id);
	///���� �� ��������� ������ �� ������
	bool noCheck;
	///���� �� ������������� ��� ������ ������
	bool noEdit;
	///������������ ����
	wxTreeItemId dragItem;
	void OnSetFocus(wxFocusEvent& event);
	///����� ��� ������� ������
	wxArrayString buf;
	
public:
	DECLARE_EVENT_TABLE()

	
};


#endif