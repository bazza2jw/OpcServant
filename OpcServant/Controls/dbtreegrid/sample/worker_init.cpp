#include "worker.h"
#include "../include/dbdispatcher.h"





bool WorkerDialog::Init()
{
	

	wxString s;
	dispatcher = new wxDbDispatcher();



	dept->db=dbase;
	dept->selectClause=wxT("select  d.name name , dept.id id, d.dept_id");




	dept->fromClause=wxT("from connect_by_prior('department','ID','dept_ID','dept_id is null','name',null) as dept\
		  left join department  d on dept.id=d.id");
//	dept->orderClause=wxT("order by LastName");
	dept->CreateQuery();

	if(!dept->SetupField())
	{
		
		return false;
	}

	dept->SetPkNum(1);
	dept->SetTreeNum(2);
	dept->dstName= wxT("department");
	dept->seqName=wxT("GlobalSequence");


	ArraywxDbGridFieldDesc* fd =dept->GetFieldDescription();



	//Установка поля PK
	dept->SetPkNumByName(wxT("id"));
	//Установка поля связи в дереве
	dept->SetTreeNumByName(wxT("dept_id"));
	

	//Дополнительное описание св-в полей
	
	dept->ResetFieldPropertyByName( wxT("id"), DB_FIELD_VISIBLE);
	dept->ResetFieldPropertyByName( wxT("id"), DB_FIELD_SEARCH);
	dept->SetFieldPropertyByName( wxT("Name"), DB_FIELD_NOT_NULL); 
	dept->ResetFieldPropertyByName( wxT("dept_id"), DB_FIELD_VISIBLE);
	dept->ResetFieldPropertyByName( wxT("dept_id"), DB_FIELD_SEARCH);
	dept->SetColumnNameByName( wxT("name"), wxT("Name*") );
	dept->SetProperty(DB_GRID_IS_TREE);
	dept->ResetProperty(DB_GRID_QUERY_ENABLE);




	dept->CreateField();	
	dept->GetAllRow();
	dept->SelectFirstRecord();
	dispatcher->AddGrid((wxDbQueryObject*)dept);

	dept->SetColumnWidth(0,200);


	s=dept->GetPkValue();
	worker->db=dbase;
	worker->ResetProperty(DB_GRID_READ_ONE);
	worker->selectClause=wxT("select id,dept_id,appoint_id,people_id,tab_n, fio,salary,d_from, d_to,aname,tmp ");
	worker->fromClause=wxT("from v_worker");
	worker->orderClause=wxT("order by tab_n");
	worker->CreateQuery();
	if( s == wxT("null") ) s = wxT("-1");
	worker->AddWhere(wxT("dept_id='")+s+wxT("'"));

	if(!worker->SetupField())
	{
		
		return false;
	}

	worker->SetPkNum(0);
	worker->SetFkNum(1);
	worker->dstName= wxT("worker");
	worker->seqName=wxT("GlobalSequence");
	worker->parent=dept;

	worker->SetFieldPropertyByName( wxT("fio"), DB_FIELD_NOT_NULL); 
	worker->SetFieldPropertyByName( wxT("tab_n"), DB_FIELD_NOT_NULL); 
	worker->SetFieldPropertyByName( wxT("d_from"), DB_FIELD_NOT_NULL); 

	worker->ResetFieldPropertyByName( wxT("id"), DB_FIELD_VISIBLE);
	worker->ResetFieldPropertyByName( wxT("dept_id"), DB_FIELD_VISIBLE);
	worker->ResetFieldPropertyByName( wxT("appoint_id"), DB_FIELD_VISIBLE);
	worker->ResetFieldPropertyByName( wxT("people_id"), DB_FIELD_VISIBLE);
	worker->ResetFieldPropertyByName( wxT("aname"), DB_FIELD_VISIBLE);
	worker->ResetFieldPropertyByName( wxT("tmp"), DB_FIELD_VISIBLE);


	worker->AddLinkFieldByName(wxT("aname"), appoint );
	worker->AddLinkFieldByName(wxT("tmp"), always );


	worker->SetColumnNameByName( wxT("tab_n"), wxT("Code*") );
	worker->SetColumnNameByName( wxT("fio"), wxT("People*") );
	worker->SetColumnNameByName( wxT("d_from"), wxT("Begin*") );
	worker->SetColumnNameByName( wxT("d_to"), wxT("End") );
	worker->SetColumnNameByName( wxT("salary"), wxT("Salary") );

	worker->ResetFieldPropertyByName( wxT("fio"), DB_FIELD_DBASE );
	worker->ResetFieldPropertyByName( wxT("aname"), DB_FIELD_DBASE );


	worker->SetDefValueByName( wxT("tmp"), wxT("0")) ;


	wxDbLOV lov;
	lov.selectClause=wxT("select id, l_name, f_name, m_name, p.l_name||' '||coalesce(p.f_name,'')||' '||coalesce(p.m_name,'') fio ");
	lov.fromClause=wxT("from people p");
	lov.orderClause=wxT("order by l_name, f_name");


	wxDbPair p;
	p.i1=0;p.i2=worker->GetFieldIndex( wxT("people_id") ) ;p.w=0;p.fieldName=wxT("id");lov.pair.Add(p);
	p.i1=1;p.i2=0;p.w=150;p.fieldName=wxT("l_name");lov.pair.Add(p);
	p.i1=2;p.i2=0;p.w=150;p.fieldName=wxT("f_name");lov.pair.Add(p);
	p.i1=3;p.i2=0;p.w=150;p.fieldName=wxT("m_name");lov.pair.Add(p);
	p.i1=4;p.i2=worker->GetFieldIndex( wxT("fio") );p.w=0;p.fieldName=wxT("name");lov.pair.Add(p);
	lov.size=wxSize(400,300);
	wxDbLOV *l=worker->AddLOV(lov);
	worker->SetLOVByName(wxT("fio"), l );

	lov.pair.Clear();
	lov.selectClause=wxT("select  name, id ");
	lov.fromClause=wxT("from appoint");
	lov.orderClause=wxT("order by name");
	p.i1=0;p.i2=worker->GetFieldIndex( wxT("aname") );p.w=200;p.fieldName=wxT("name");lov.pair.Add(p);
	p.i1=1;p.i2=worker->GetFieldIndex( wxT("appoint_id") );p.w=0;p.fieldName=wxT("id");lov.pair.Add(p);
	lov.size=wxSize(250,300);
	l=worker->AddLOV(lov);
	worker->SetLOVByName(wxT("aname"), l );


	worker->CreateField();	
	worker->GetAllRow();
	worker->SelectFirstRecord();
	

	worker->SetColumnWidthByName("tab_n",100);
	worker->SetColumnWidthByName("fio",200);
	worker->SetColumnWidthByName("d_from",100);
	worker->SetColumnWidthByName("d_to",100);
	worker->SetColumnWidthByName("salary",100);
	worker->SetLenByName("salary",10);
	worker->SetPrecisionByName("salary",2);
	worker->SetMinValueByName("salary","0");


	dispatcher->AddGrid((wxDbQueryObject*)worker);
	//Устанавливаем главное окно для проверки на изменения при закрытии
	dispatcher->win = this;	
	return true;

};

void WorkerDialog::OnClose(wxCloseEvent &event){
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

BEGIN_EVENT_TABLE( WorkerDialog, wxDialog )
//	EVT_CLOSE(AppointDialog::OnClose) 
END_EVENT_TABLE()
