#include "appointdialog.h"
#include "../include/dbdispatcher.h"





bool AppointDialog::Init()
{
	

	
	dispatcher = new wxDbDispatcher();



	tree->db=dbase;
	tree->selectClause=wxT("select  id, name");
	tree->fromClause=wxT("from appoint");
	tree->orderClause=wxT("order by Name");
	tree->CreateQuery();

	if(!tree->SetupField())
	{
		
		return false;
	}

	tree->SetPkNum(0);
	tree->dstName= wxT("appoint");
	tree->seqName=wxT("GlobalSequence");


	ArraywxDbGridFieldDesc* fd =tree->GetFieldDescription();
	(*fd)[0].ResetProperty(DB_FIELD_VISIBLE);

	(*fd)[1].SetProperty(DB_FIELD_NOT_NULL);
	
	(*fd)[1].columnName=wxT("Name*");

	(*fd)[0].ResetProperty(DB_FIELD_SEARCH);
	(*fd)[0].ResetProperty(DB_FIELD_QUERY);
	(*fd)[0].ResetProperty(DB_FIELD_SEARCH);



	tree->CreateField();	
	tree->GetAllRow();
	tree->SelectFirstRecord();
	dispatcher->AddGrid((wxDbQueryObject*)tree);
	tree->SetColumnWidth(1,200);


	//Устанавливаем главное окно для проверки на изменения при закрытии
	dispatcher->win = this;	
	return true;

};

void AppointDialog::OnClose(wxCloseEvent &event){
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

BEGIN_EVENT_TABLE( AppointDialog, wxDialog )
//	EVT_CLOSE(AppointDialog::OnClose) 
END_EVENT_TABLE()
