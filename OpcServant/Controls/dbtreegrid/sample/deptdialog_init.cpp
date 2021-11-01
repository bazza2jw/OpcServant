#include "deptdialog.h"
#include "../include/dbdispatcher.h"





bool DeptDialog::Init()
{
	

	
	dispatcher = new wxDbDispatcher();



	tree->db=dbase;
	tree->selectClause=wxT("select  d.name name , tree.id id, d.dept_id");




	tree->fromClause=wxT("from connect_by_prior('department','ID','dept_ID','dept_id is null','name',null) as tree\
		  left join department  d on tree.id=d.id");
//	tree->orderClause=wxT("order by LastName");
	tree->CreateQuery();

	if(!tree->SetupField())
	{
		
		return false;
	}

	tree->SetPkNum(1);
	tree->SetTreeNum(2);
	tree->dstName= wxT("department");
	tree->seqName=wxT("GlobalSequence");


	ArraywxDbGridFieldDesc* fd =tree->GetFieldDescription();



	//Установка поля PK
	tree->SetPkNumByName(wxT("id"));
	//Установка поля связи в дереве
	tree->SetTreeNumByName(wxT("dept_id"));
	

	//Дополнительное описание св-в полей
	
	tree->ResetFieldPropertyByName( wxT("id"), DB_FIELD_VISIBLE);
	tree->ResetFieldPropertyByName( wxT("id"), DB_FIELD_SEARCH);
	tree->SetFieldPropertyByName( wxT("Name"), DB_FIELD_NOT_NULL); 
	tree->ResetFieldPropertyByName( wxT("dept_id"), DB_FIELD_VISIBLE);
	tree->ResetFieldPropertyByName( wxT("dept_id"), DB_FIELD_SEARCH);
	tree->SetColumnNameByName( wxT("name"), wxT("Name*") );
	tree->SetProperty(DB_GRID_IS_TREE);
	tree->ResetProperty(DB_GRID_QUERY_ENABLE);




	tree->CreateField();	
	tree->GetAllRow();
	tree->SelectFirstRecord();
	dispatcher->AddGrid((wxDbQueryObject*)tree);
	tree->SetColumnWidth(0,200);


	//Устанавливаем главное окно для проверки на изменения при закрытии
	dispatcher->win = this;	
	return true;

};

void DeptDialog::OnClose(wxCloseEvent &event){
	if(dispatcher->IsEdit())
	{
		
			if (event.CanVeto())   {        event.Veto();  return ;}
	};
	if(dispatcher->IsChanged())
	{
		int answer=wxMessageBox("Есть изменеия, выйдти без сохранения?",wxEmptyString,wxOK|wxCANCEL) ;
		if(! (wxOK == answer ))
		{  
			if (event.CanVeto())   {       event.Veto();  return ;}
		}
	}

//	if(db){
//		db->RollBack(); if (db->IsOpen())db->Close();
//	}

	Destroy();

}

BEGIN_EVENT_TABLE( DeptDialog, wxDialog )
//	EVT_CLOSE(AppointDialog::OnClose) 
END_EVENT_TABLE()
