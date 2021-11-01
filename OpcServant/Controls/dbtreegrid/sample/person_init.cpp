#include "person.h"

bool PersonDialog::Init()
{

	wxString where;
	wxArrayString as;
	dispatcher = new wxDbDispatcher();
	if(!dispatcher) return false;;
	SetLabel(wxT("People"));
	
	


	
	
	//Инициализация грида
	tree->db=dbase;
	tree->selectClause=wxT("select  id, l_name, f_name, m_name,     d_birth ,  sex sex1, sex ");
	tree->fromClause=wxT("from people");
	tree->orderClause=wxT("order by 2,3,4");

	//Создание запроса
	tree->CreateQuery();

	//Определение полей из запроса
	if(!tree->SetupField()) {  return false;}
	//Установка поля PK
	tree->SetPkNumByName(wxT("id"));
	//Установка таблицы куда записываются изменения
	tree->dstName= wxT("people");

	ArraywxDbGridFieldDesc* fd =tree->GetFieldDescription();
	tree->ResetFieldPropertyByName( wxT("sex"), DB_FIELD_VISIBLE);
	tree->ResetFieldPropertyByName( wxT("id"), DB_FIELD_VISIBLE);

	tree->SetFieldPropertyByName( wxT("l_Name"), DB_FIELD_NOT_NULL );
	tree->SetFieldPropertyByName( wxT("sex1"), DB_FIELD_NOT_NULL );
	


	tree->ResetFieldPropertyByName( wxT("id"), DB_FIELD_QUERY);
	tree->ResetFieldPropertyByName( wxT("sex1"), DB_FIELD_QUERY);
	tree->ResetFieldPropertyByName( wxT("sex1"), DB_FIELD_QUERY);
	tree->ResetFieldPropertyByName( wxT("d_birth"), DB_FIELD_QUERY);

	tree->ResetFieldPropertyByName( wxT("id"), DB_FIELD_SEARCH);
	tree->ResetFieldPropertyByName( wxT("sex1"), DB_FIELD_SEARCH);
	tree->ResetFieldPropertyByName( wxT("sex1"), DB_FIELD_SEARCH);
	tree->ResetFieldPropertyByName( wxT("d_birth"), DB_FIELD_SEARCH);


	tree->SetColumnNameByName( wxT("l_Name"), wxT("Last name*") );
	tree->SetColumnNameByName( wxT("f_Name"), wxT("First name") );
	tree->SetColumnNameByName( wxT("m_name"), wxT("Middle name") );
	tree->SetColumnNameByName( wxT("d_birth"), wxT("Birth date") );
	tree->SetColumnNameByName( wxT("sex1"), wxT("Sex*") );
	
	tree->SetDefValueByName( wxT("sex"), wxT("M") );
	tree->SetDefValueByName( wxT("sex1"), wxT("M") );
	tree->SetValueTypeByName( wxT("sex1"), DB_FIELD_LIST );

	as.Add(wxT("M"));
	as.Add(wxT("F"));
	tree->SetValuesByName( wxT("sex1"), as );
	tree->SetValues2ByName( wxT("sex1"), as );

	tree->ResetFieldPropertyByName( wxT("sex1"), DB_FIELD_DBASE);
	tree->SetValueTypeByName( wxT("d_birth"), DB_FIELD_DATETIME );
	tree->SetValidatorByName( wxT("d_birth"), &dtvalidator );
	
	tree->SetDefValueByName( wxT("d_birth"), wxT("01.01.1900 00:00:00")) ;

	

	
	//Установка последоватнльности для генерации PK
	tree->seqName=wxT("GlobalSequence");
	//Создание колонок в гриде
	tree->CreateField();
	//Получение записей из БД
	tree->GetAllRow();
	//Выбор первой записи
	tree->SelectFirstRecord();
	//Добавляем грид в диспетчер
	dispatcher->AddGrid((wxDbQueryObject*)tree);
	
	//Читаем размеры объектов интерфейса из файла настроек
	SetName(wxT("PersonDialog"));
	tree->SetName(wxT("PersonTree"));
	


	//Устанавливаем главное окно для проверки на изменения при закрытии
	dispatcher->win = this;
	return true;

};

void PersonDialog::OnClose(wxCloseEvent &event){
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
	wxString ss;
	//Записываем размеры объектов интерфейса в файл


	

	Destroy();

}
