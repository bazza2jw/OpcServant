#include "test.h"
#include "../include/dbdispatcher.h"
#include "../include/validator.h"

bool TestFieldDialog::Init()
{
	

	
	wxString s;
	dispatcher = new wxDbDispatcher();



	tree->db=dbase;
	tree->selectClause=wxT("select id, ftext1, fio, flist1_1, flist1, fdate1, ffloat1, fint, fbin1, fvalidator, \
		ftext2, flist2, fbin2, fdate2, ffloat2, fcheck, people_id");




	tree->fromClause=wxT("from v_sample");

	tree->CreateQuery();

	if(!tree->SetupField())
	{
		
		return false;
	}
 

	tree->dstName= wxT("sample");
	tree->seqName=wxT("GlobalSequence");
	tree->SetPkNumByName("id");
	

	tree->ResetFieldPropertyByName( wxT("id"), DB_FIELD_VISIBLE);


	tree->SetColumnNameByName( wxT("ftext1"), wxT("TEXT") );

	tree->SetColumnNameByName( wxT("fio"), wxT("QUERY LIST") );
	tree->ResetFieldPropertyByName( wxT("fio"), DB_FIELD_DBASE );
	tree->ResetFieldPropertyByName( wxT("people_id"), DB_FIELD_VISIBLE);
	wxDbLOV lov;
	lov.selectClause=wxT("select id, l_name, f_name, m_name, p.l_name||' '||coalesce(p.f_name,'')||' '||coalesce(p.m_name,'') fio ");
	lov.fromClause=wxT("from people p");
	lov.orderClause=wxT("order by l_name, f_name");
	wxDbPair p;
	p.i1=0;p.i2=tree->GetFieldIndex( wxT("people_id") ) ;p.w=0;p.fieldName=wxT("id");lov.pair.Add(p);
	p.i1=1;p.i2=0;p.w=150;p.fieldName=wxT("l_name");lov.pair.Add(p);
	p.i1=2;p.i2=0;p.w=150;p.fieldName=wxT("f_name");lov.pair.Add(p);
	p.i1=3;p.i2=0;p.w=150;p.fieldName=wxT("m_name");lov.pair.Add(p);
	p.i1=4;p.i2=tree->GetFieldIndex( wxT("fio") );p.w=0;p.fieldName=wxT("name");lov.pair.Add(p);
	lov.size=wxSize(400,300);
	wxDbLOV *l=tree->AddLOV(lov);
	tree->SetLOVByName(wxT("fio"), l );
	
	tree->SetValueTypeByName( wxT("flist1_1"), DB_FIELD_LIST );
	wxArrayString as;
	as.Add("one");
	as.Add("two");
	as.Add("three");

	tree->SetValuesByName( wxT("flist1_1"), as );

	as.Clear();
	as.Add("1");
	as.Add("2");
	as.Add("3");
	tree->SetValues2ByName( wxT("flist1_1"), as );
	tree->ResetFieldPropertyByName( wxT("flist1"), DB_FIELD_VISIBLE);
	tree->SetColumnNameByName( wxT("flist1_1"), wxT("LIST") );
	tree->ResetFieldPropertyByName( wxT("flist1_1"), DB_FIELD_DBASE );

	tree->SetColumnNameByName( wxT("fdate1"), wxT("DATE") );

	tree->SetColumnNameByName( wxT("ffloat1"), wxT("FLOAT") );
	tree->SetLenByName("ffloat1",10);
	tree->SetPrecisionByName("ffloat1",2);
	tree->SetMinValueByName("ffloat1","0");

	tree->SetColumnNameByName( wxT("fint"), wxT("INTEGER") );
	tree->SetValueTypeByName( wxT("fint"), DB_FIELD_INT );


	tree->SetColumnNameByName( wxT("fbin1"), wxT("BOOL") );
	tree->SetValueTypeByName( wxT("fbin1"), DB_FIELD_BOOL );

	tree->SetColumnNameByName( wxT("fvalidator"), wxT("VALIDATOR") );
	tree->SetValidatorByName( wxT("fvalidator"), &snvalidator );

	tree->SetColumnNameByName( wxT("ftext2"), wxT("Text field") );
	tree->ResetFieldPropertyByName( wxT("ftext2"), DB_FIELD_VISIBLE);
	tree->AddLinkFieldByName(wxT("ftext2"), t1 );

			
	tree->SetColumnNameByName( wxT("flist2_1"), wxT("List field") );
	tree->ResetFieldPropertyByName( wxT("flist2"), DB_FIELD_VISIBLE);
	tree->ResetFieldPropertyByName( wxT("flist2_1"), DB_FIELD_DBASE );
	tree->SetDefValueByName( wxT("flist2"), "One" );
	tree->AddLinkFieldByName(wxT("flist2"), l1 );
	l1->SetValues(as);

	tree->SetColumnNameByName( wxT("fbin2"), wxT("Bin field") );
	tree->ResetFieldPropertyByName( wxT("fbin2"), DB_FIELD_VISIBLE);
	tree->AddLinkFieldByName(wxT("fbin2"), c );

	tree->SetColumnNameByName( wxT("fdate2"), wxT("Date field") );
	tree->ResetFieldPropertyByName( wxT("fdate2"), DB_FIELD_VISIBLE);
	tree->AddLinkFieldByName(wxT("fdate2"), d1 );


	tree->SetColumnNameByName( wxT("ffloat2"), wxT("Fload field") );
	tree->ResetFieldPropertyByName( wxT("ffloat2"), DB_FIELD_VISIBLE);
	tree->SetLenByName("ffloat1",10);
	tree->SetPrecisionByName("ffloat1",2);
	tree->SetMinValueByName("ffloat1","0");


	tree->ResetFieldPropertyByName( wxT("fcheck"), DB_FIELD_VISIBLE);
	tree->AddLinkFieldByName(wxT("fcheck"), r1 );
	r1->SetValues(as);

	//tree->ResetFieldPropertyByName( wxT("fcheck"), DB_FIELD_VISIBLE);
	//tree->AddLinkFieldByName(wxT("fcheck"), c );
	//c->SetValues(as);

	/*ArraywxDbGridFieldDesc* fd =tree->GetFieldDescription();
	if(fd->Count()>=0){
		tree->SetPkNum(3);
	tree->SetTreeNum(4);
	(*fd)[0].SetProperty(DB_FIELD_NOT_NULL);
	(*fd)[5].ResetProperty(DB_FIELD_VISIBLE);
	(*fd)[6].ResetProperty(DB_FIELD_VISIBLE);
	(*fd)[3].ResetProperty(DB_FIELD_VISIBLE);
	(*fd)[4].ResetProperty(DB_FIELD_VISIBLE);
//	(*fd)[7].ResetProperty(DB_FIELD_EDITABLE);
	(*fd)[0].columnName=wxT("Фамилия*");
	(*fd)[1].columnName=wxT("Имя");
	(*fd)[2].columnName=wxT("Отчество");
	(*fd)[10].defValue=wxT("20");

	tree->SetProperty(DB_GRID_IS_TREE);
	//tree->ResetProperty(DB_GRID_QUERY_ENABLE);
	tree->seqName=wxT("GlobalSequence");
	(*fd)[7].valueType=DB_FIELD_DATETIME;
	(*fd)[11].valueType=DB_FIELD_FLOAT;
	(*fd)[12].valueType=DB_FIELD_INT;
	(*fd)[12].len = 16;

	(*fd)[11].len = 18;
	(*fd)[11].presition = 4;
	(*fd)[11].minVal = wxT("100");
	(*fd)[11].maxVal = wxT("400");

	tree->CreateField();
	tree->GetAllRow();
	tree->AddLinkField(t1, 0);
	tree->AddLinkField(d1, 7);
	
	tree->AddLinkField(c, 8);
	tree->AddLinkField(l1, 9);
	tree->AddLinkField(r1, 10);
//	(*fd)[7].validator=&dtvalidator;

	}

	wxDbLOV lov;
	lov.selectClause=wxT("select lastname as \"Фамилия\",\
					firstname as \"Имя\",\
					patronymic as \"Отчество\",\
					id,\
					lastname||' '||firstname||' '||patronymic as fio\
					from person");
	lov.query=wxT("select lastname as \"Фамилия\",\
					firstname as \"Имя\",\
					patronymic as \"Отчество\",\
					id,\
					lastname||' '||firstname||' '||patronymic as fio\
					from person");	
	wxDbPair p;
	p.i1=0;p.i2=0;p.w=200;p.fieldName=wxT("lastname");lov.pair.Add(p);
	p.i1=1;p.i2=1;p.w=200;p.fieldName=wxT("firstname");lov.pair.Add(p);
	p.i1=2;p.i2=2;p.w=200;p.fieldName=wxT("patronymic");lov.pair.Add(p);
	p.i1=3;p.i2=-1;p.w=0;p.fieldName=wxT("id");lov.pair.Add(p);
	p.i1=4;p.i2=-1;p.w=0;lov.pair.Add(p);
	
	lov.size=wxSize(700,300);
	wxDbLOV *l=tree->AddLOV(lov);

	(*fd)[0].lov=l;
	wxArrayString as;
	as.Add("10");
	as.Add("20");
	as.Add("30");
	l1->SetValues(as);
	r1->SetValues(as);
*/
	tree->CreateField();	
	tree->GetAllRow();
	tree->SelectFirstRecord();
	dispatcher->AddGrid((wxDbQueryObject*)tree);
	//tree->SetColumnWidth(0,200);
	//tree->SetColumnWidth(1,200);
	//tree->SetColumnWidth(2,200);

//	d1->Disable();
//	tree->Hide();
	//Устанавливаем главное окно для проверки на изменения при закрытии
	dispatcher->win = this;
	return true;

};

void TestFieldDialog::OnClose(wxCloseEvent &event){
	
	if(dispatcher->IsChanged())
	{
		int answer=wxMessageBox("Есть изменеия, выйдти без сохранения?",wxEmptyString,wxOK|wxCANCEL) ;
		if(! (wxOK == answer ))
		{  
			if (event.CanVeto())   {        event.Veto();  return ;}
		}
	}


	Destroy();

}


BEGIN_EVENT_TABLE( TestFieldDialog, wxDialog )
	EVT_CLOSE(TestFieldDialog::OnClose) 

END_EVENT_TABLE()
 