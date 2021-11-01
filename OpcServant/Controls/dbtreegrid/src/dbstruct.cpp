/////////////////////////////////////////////////////////////////////////////
// Name:        dbstruct.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////


//Объекты для представления структуры данных таблиц

#include "../include/dbstruct.h"
#include <wx/arrimpl.cpp>
#include <wx/wx.h>
#include "../include/dbtreegrid.h"
#include "../include/db.h"
#include "../include/langdef.h"


bool CheckVal(const wxString& fname, const wxString& s, int valueType, int len =0, int presition=0,const wxString& maxVal=wxEmptyString,const wxString& minVal=wxEmptyString, bool notNull=false, int minLen = 0){

	wxString ss;

	if ( s == wxEmptyString && !notNull ) return true;
	if ( s == wxEmptyString && notNull )
	{
		wxMessageBox(TEXT_IS_REQUE_VALUE);
		return false;
	};
	if( valueType==DB_FIELD_STRING)
	{
		if( (int)s.Len()>len  && len!=0)
		{
			ss.sprintf("CheckVal - Длина строкового поля превышает заданную %d",len);
			wxMessageBox(ss);
			return false; 
		};
		if( (int)s.Len()<minLen  && minLen!=0)
		{
			ss.sprintf("CheckVal - Длина строкового поля меньше заданной %d",minLen);
			wxMessageBox(ss);
			return false; 
		};

		if((s>maxVal && maxVal!=wxEmptyString) || (s<minVal && minVal!=wxEmptyString))
		{
			wxMessageBox(TEXT_IS_STRING_OUT_OF_RANGE+minVal+wxT(" - ")+maxVal);
			return false;
		};
		return true;
	}
	long l,lmax,lmin;
	double d,dmax,dmin;
	if( valueType==DB_FIELD_INT  && !s.ToLong(&l) )
	{
		wxMessageBox(TEXT_IS_WRONG_NUMBER);
		return false;
	}
	if( valueType==DB_FIELD_INT  )
	{
		maxVal.ToLong(&lmax);
		minVal.ToLong(&lmin);
		if((l<lmin &&  minVal!=wxEmptyString) || (l>lmax &&  maxVal!=wxEmptyString))
		{
				wxMessageBox(TEXT_IS_INTEGER_OUT_OF_RANGE+minVal+wxT(" - ")+maxVal);
				return false;
		};

	}
	if( valueType==DB_FIELD_FLOAT  && !s.ToDouble(&d) )
	{
		wxMessageBox(TEXT_IS_WRONG_FLOAT_NUMBER);
		return false;
	}
	if( valueType==DB_FIELD_FLOAT  )
	{
		maxVal.ToDouble(&dmax);
		minVal.ToDouble(&dmin);
		if((d<dmin &&  minVal!=wxEmptyString) || (d>dmax &&  maxVal!=wxEmptyString))
		{
				wxMessageBox(TEXT_IS_FLOAT_OUT_OF_RANGE+minVal+wxT(" - ")+maxVal);
				return false;
		};

	}

	if( valueType==DB_FIELD_DATETIME  )
	{
		wxDateTime  dt;
		if(!dt.ParseFormat(s,DATETIME_FORMAT))
		{
			wxMessageBox(TEXT_IS_WRONG_DATE);
			return false;
		};
	}
	return true;
}

wxString FormatField(const wxString &ss,wxDbGridFieldDesc *fd)
{
	wxString s,fs;
	double d;
	s=ss;
	if(fd->valueType!= DB_FIELD_INT && fd->valueType!= DB_FIELD_FLOAT) return s;
	if(fd->valueType==DB_FIELD_INT && s!=wxEmptyString )
	{
			s.ToDouble(&d);
			s.sprintf("%.0Lf",d);
	}
	if(fd->valueType==DB_FIELD_FLOAT && s!=wxEmptyString )
	{	
			wxString fs;
			if(fd->presition>0) fs.sprintf("\%.%dLf",fd->presition);
			else fs=wxT("%%ld");
			s.ToDouble(&d);
			s.sprintf(fs,d);
	};
	return s;

};



DEFINE_EVENT_TYPE(dbEVT_REC_HANDLE)
DEFINE_EVENT_TYPE(dbEVT_GRID_ACTIVE_HANDLE)




WX_DEFINE_OBJARRAY( ArraywxDbGridFieldDesc );
WX_DEFINE_OBJARRAY( ArraywxDbPair );
WX_DEFINE_OBJARRAY( ArraywxDbLOV );
WX_DEFINE_OBJARRAY( ArrayDbRec );



 wxDbRecEvent::wxDbRecEvent(wxEventType commandEventType, int id):wxNotifyEvent(commandEventType,id)
 {as=0;/*treeGrid=0;*/tp=0;veto=false;obj=0;};

 wxDbGridEvent::wxDbGridEvent(wxEventType commandEventType, int id):wxNotifyEvent(commandEventType,id)
 {grid=0;};

wxDbQueryObject::~wxDbQueryObject()
{
	try{
		if(dbq && db->IsOpen()){ db->CloseResultSet(dbq);}
	//	if (db->IsOpen())db->Close();
	}
	catch (DatabaseLayerException& e)
	{
		wxMessageBox(wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
	}
};


//Создание запроса из частей заданных в selectClause fromClause...
void wxDbQueryObject::CreateQuery()
{
	 

	//Если выражение select не задано то полагаем что запрос введен не разбит на части и введен целиком , не можем
	// в таком случае менять условие выборки
	if(selectClause==wxEmptyString) return;
	//Анализ полей из запроса - queryData и модификация whereClause
	//Собираем условие
	wxString s,sw;
	int fl=0;
	for(int i=0; i<(int)fieldsDescription.Count() && i<(int)queryData.Count(); i++)
	{

		if(queryData[i]!=wxEmptyString && fieldsDescription[i].GetProperty()&DB_FIELD_QUERY)
		{
			if(fl) sw=sw+wxT(" and ");
			sw=sw+fieldsDescription[i].fieldName;
			s=GetColumnValueForSQL(queryData,i );
			
			if (fieldsDescription[i].valueType==DB_FIELD_STRING)
			{
				sw=sw+wxT(" like ");
				sw=sw+s.SubString(0,s.Len()-2)+wxT("%%'");

			}
			else
			{
				sw=sw+wxT(" = ");
				sw=sw+s;
			}
			
			fl++;
		}
		
	}
	queryDataWhere.Trim();
	if( fl && queryDataWhere != wxEmptyString )
	{
		sw=sw+wxT(" and ");
	}
	if(  queryDataWhere != wxEmptyString )
	{
		sw += queryDataWhere;
	}

	//Устанавливаем услоия для фильтра по родительскому гриду
	if(parent && GetFkNum()>=0)
	{
		wxArrayString as;
		s=parent->GetPkValue();

		as.Add(s,parent->GetPkNum()+1);
		s=parent->GetColumnValueForSQL(as,parent->GetPkNum() );
		if(s==wxT("''") || s==wxT("'null'")  ) s=wxT("null");
		if(fl) sw=sw+wxT(" and ");
		sw=sw+fieldsDescription[GetFkNum()].fieldName;
		if (s!=wxT("null")) sw=sw+wxT("="); else sw=sw+wxT(" is ");
		sw=sw+s;
		fl++;
	}

	
	if(sw != wxEmptyString)
	{
		if( whereClause !=wxEmptyString ) 
		{
			sw=wxT(" and ")+sw;
		}
		else
		{
			sw=wxT(" where ")+sw;
		};
	}

	if(selectClause!=wxEmptyString)
	{
		query=selectClause+wxT(" ")+fromClause+wxT(" ")+whereClause+wxT(" ")+sw+wxT(" ")+orderClause;
	};

	
};

///Создание запроса для LOV
void wxDbQueryObject::CreateLOVQuery()
{


	//Если выражение select не задано то полагаем что запрос введен не разбит на части и введен целиком , не можем
	// в таком случае менять условие выборки
	if(selectClause==wxEmptyString) return;
	//Анализ полей из запроса - queryData и модификация whereClause
	//Собираем условие
	wxString s,sw;
	int fl=0;
	for(int i=0; i<(int)fieldsDescription.Count() && i<(int)queryData.Count(); i++)
	{


		if(queryData[i]!=wxEmptyString && fieldsDescription[i].GetProperty()&DB_FIELD_QUERY )
		{
			if(fl) sw=sw+wxT(" or "); else sw=wxT("(");
			//sw=sw+fieldsDescription[i].fieldName;
		//	s=GetColumnValueForSQL(queryData,i );
			//wxMessageBox("wxDbQueryObject::CreateLOVQuery -"+queryData[i]);
			if (fieldsDescription[i].valueType==DB_FIELD_STRING)
			{
				sw=sw+fieldsDescription[i].fieldName;
				sw=sw+wxT(" like '");
				if( queryData[i].Find( wxT('%') ) == 0) 
				{
					sw = sw+ wxT("%%");
				}
				sw=sw+queryData[i];
				sw=sw+wxT("%%'");

			}
			if (fieldsDescription[i].valueType!=DB_FIELD_STRING)
			{
				sw=sw+wxT(" cast (");
				sw=sw+fieldsDescription[i].fieldName;
				sw=sw+wxT(" as char(32) ) ");
				sw=sw+wxT(" like '");
				if( queryData[i].Find( wxT('%') ) == 0) 
				{
					sw = sw+ wxT("%%");
				}

				sw=sw+queryData[i];
				sw=sw+wxT("%%'");

			}
			//wxMessageBox("wxDbQueryObject::CreateLOVQuery -"+sw);
			fl++;
		}
		
	}

	if(sw != wxEmptyString)
	{
		if( whereClause !=wxEmptyString ) 
		{
			sw=wxT(" and ")+sw + wxT(" ) ");
		}
		else
		{
			sw=wxT(" where ")+sw+wxT(")");
		};
	}
	if(selectClause!=wxEmptyString)
	{
		query=selectClause+wxT(" ")+fromClause+wxT(" ")+whereClause+wxT(" ")+sw+wxT(" ")+orderClause;
	};
	//wxMessageBox("wxDbQueryObject::CreateLOVQuery -"+query);

};


//Формирование описателей полей из запроса
bool wxDbQueryObject::SetupField(){
	wxDbGridFieldDesc fd;
	ResultSetMetaData *md;
	try{
		if(db && dbq){
			db->CloseResultSet(dbq);
		}
		if(fieldsDescription.Count()>0) fieldsDescription.Clear();
	
		dbq=db->RunQueryWithResults(query);
		md=dbq->GetMetaData();
		wxString s;
		for ( int i=1; i<=md->GetColumnCount(); i++){
					s.sprintf(" wxDbQueryObject::SetupField() SIZE %d",md->GetColumnSize(i));

					fd.columnName=md->GetColumnName(i);
					fd.fieldName=md->GetColumnName(i);
					fd.len=md->GetColumnSize(i);
					fd.SetProperty(DB_FIELD_DBASE);
					
					switch(md->GetColumnType(i)){
					case ResultSetMetaData::COLUMN_UNKNOWN :  
						fd.valueType=DB_FIELD_STRING;
						break;
					case ResultSetMetaData::COLUMN_NULL :
						fd.valueType=DB_FIELD_STRING;
						break;
					case ResultSetMetaData::COLUMN_INTEGER   :
						fd.valueType=DB_FIELD_INT;
						break;
					case ResultSetMetaData::COLUMN_STRING   :
						fd.valueType=DB_FIELD_STRING;
						break;
					case ResultSetMetaData::COLUMN_DOUBLE   :
						fd.valueType=DB_FIELD_FLOAT;
						fd.presition=2;
						break;
					case ResultSetMetaData::COLUMN_BOOL   :
						fd.valueType=DB_FIELD_BOOL;
						break;
					case ResultSetMetaData::COLUMN_BLOB   :
						fd.valueType=DB_FIELD_STRING;
						break;
					case ResultSetMetaData::COLUMN_DATE  :
						fd.valueType=DB_FIELD_DATE;
						break;
					default :
						fd.valueType=DB_FIELD_STRING;
						break;

					}
					fieldsDescription.Add(fd);

		};
		//Добавляем поля статус, всегда в конец
		fd.fieldName=wxT("status");
		fd.valueType=DB_FIELD_FLAGS;
		fd.columnName=wxT("status");
		fd.ResetProperty(DB_FIELD_DBASE|DB_FIELD_VISIBLE|DB_FIELD_SEARCH|DB_FIELD_QUERY);
		fieldsDescription.Add(fd);
		boolProperty&=~DB_GRID_ALL_RECORD_FETCHED;
		return true;	


	}
	catch (DatabaseLayerException& e)
	{
		if(dbq && db)  {	db->CloseResultSet(dbq); dbq=0; }
		wxMessageBox(wxString::Format(_("wxDbQueryObject::SetupField - Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
		return false;
	}
};

// Создание selectClause на основании описания полей
void wxDbQueryObject::CreateSelectClause(){
	selectClause=wxT("select ");
	for (int i=0; i<(int)fieldsDescription.Count(); i++){
		selectClause=selectClause+fieldsDescription[i].columnOperator+wxT(" ")+selectClause+fieldsDescription[i].columnName+wxT(",");
	}

}; 

//Получение очередной строки из БД если строка считана то возвращает true
bool wxDbQueryObject::GetNextRecord(wxArrayString& lst){
//	wxLogMessage("wxDbQueryObject::GetNextRecord");
	lst.Clear();
	wxString s;
	ResultSetMetaData *md;

	try{
		if(dbq){
			md=dbq->GetMetaData();
			if (dbq->Next()){
				for ( int i=1; i<=md->GetColumnCount(); i++){
					

					if(fieldsDescription[i-1].valueType!=DB_FIELD_DATETIME)
						s=DbGetValueAsStr(   dbq,    md, i,fieldsDescription[i-1].len, fieldsDescription[i-1].presition);
					else 
					{

						s=DbGetValueAsStr(   dbq,    md, i,fieldsDescription[i-1].len, fieldsDescription[i-1].presition,true);
					};
			
					lst.Add(s);
					s.sprintf("%d",lst.Count());
				};
				//Подсчитываем количество записей
				rcount++;
				s.sprintf("%ld",rcount*1024+DB_QUERY_RECORD);
				lst.Add(s);//Свойство записи
			
//				wxLogMessage("wxDbQueryObject::GetNextRecord END " + s);
				return true;
			}
			else 
			{
				boolProperty|=DB_GRID_ALL_RECORD_FETCHED;
				//queryRunning=false;
				if(dbq && db) db->CloseResultSet(dbq);
				dbq=0;
				return false;
			}
		}
		else 
			return false;
		}
	catch (DatabaseLayerException& e)
	{
		wxMessageBox(wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str())+wxT(" wxDbQueryObject::GetNextRecord"));
	}
	return false;

};

//Выполняет SQL запрос
void wxDbQueryObject::ExecuteQuery(){
	//wxMessageBox("wxDbQueryObject::ExecuteQuery +"+query);
	try{
		if(dbq){
		 if(db)db->CloseResultSet(dbq);
			dbq=0;
		};
		dbq=db->RunQueryWithResults(query);
		boolProperty&=~DB_GRID_ALL_RECORD_FETCHED;
		rcount=0;
	//	queryRunning=true;
	//	db->BeginTransaction(); 
	}
	catch (DatabaseLayerException& e)
	{
		wxMessageBox(wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
	}

};

//Обработка записи вставка удаление изменение 
bool wxDbQueryObject::ProcessRecord(wxArrayString& as,wxDbTreeGrid *gr){
	long v;
	tree = gr;
	try{
	as[fieldsDescription.Count()-1].ToLong(&v);
//Проверяем тип операции
wxString s;
s.sprintf( "%d %d %d",DB_QUERY_RECORD, DB_UPDATED_RECORD, v );
//wxLogMessage( as[fieldsDescription.Count()-1] + "wxDbQueryObject::ProcessRecord " + as[0] +" "+ s);

if( (v&DB_QUERY_RECORD) && (v&DB_UPDATED_RECORD) )
{
//	wxLogMessage( " wxDbQueryObject::ProcessRecord update" );
}



	wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)gr)->GetId());
    ev.SetEventObject( (wxWindow*)gr );
	ev.objType=DB_OBJ_TYPE_TREE_GRID;
	ev.as=&as;
	ev.obj=this;
	//Вставка
	if(v&DB_INSERTED_RECORD  && !(v&DB_DELETED_RECORD)){
		if(preInsertHandler && !ev.veto ){
			ev.tp=DB_PRE_INSERT_HANDLE;
			preInsertHandler->ProcessEvent(ev);
		}
		else{
			if(!DoPreInsert(as)) ev.veto=true;
		}

		if(onInsertHandler && !ev.veto ){
			ev.tp=DB_ON_INSERT_HANDLE;
			onInsertHandler->ProcessEvent(ev);
		}
		else{
			if(!DoInsert(as)) ev.veto=true;
		}
		if(postInsertHandler && !ev.veto ){
			ev.tp=DB_POST_INSERT_HANDLE;
			postInsertHandler->ProcessEvent(ev);
		}
		if (ev.veto) return false;
		return true;
	}
	//Удаление
	if(v&DB_DELETED_RECORD && v&DB_QUERY_RECORD){
		if(preDeleteHandler && !ev.veto ){

			ev.tp=DB_PRE_DELETE_HANDLE;
			preDeleteHandler->ProcessEvent(ev);
		}
		if(onDeleteHandler && !ev.veto ) {
			ev.tp=DB_ON_DELETE_HANDLE;
			onDeleteHandler->ProcessEvent(ev);
		}
		else{

			if(!DoDelete(as))ev.veto=true;
		}
		if(postDeleteHandler && !ev.veto ){
			ev.tp=DB_POST_DELETE_HANDLE;
			postDeleteHandler->ProcessEvent(ev);
		}
		if (ev.veto) return false;
		return true;
	}
	//Изменение
	if(v&DB_QUERY_RECORD && v&DB_UPDATED_RECORD){

		if(preUpdateHandler && !ev.veto ){
			ev.tp=DB_PRE_UPDATE_HANDLE;
			preUpdateHandler->ProcessEvent(ev);
		}
		if(onUpdateHandler && !ev.veto ){
			ev.tp=DB_ON_UPDATE_HANDLE;
			onUpdateHandler->ProcessEvent(ev);
		}
		else{
			if(!DoUpdate(as))ev.veto=true;
		}
		if(postUpdateHandler && !ev.veto ){
			ev.tp=DB_POST_UPDATE_HANDLE;
			postUpdateHandler->ProcessEvent(ev);
		}
		if (ev.veto) return false;
		return true;
	}
	}
	catch(...)
	{
		wxMessageBox(TEXT_IS_RECORD_PROCESS_ERROR);
		return false;
	}


	return true;
};

//Обновить запись
bool wxDbQueryObject::DoUpdate(const wxArrayString& as){
	wxString s,s1;
//	wxLogMessage(GetColumnValueForSQL(as,1) + "wxDbQueryObject::DoUpdateGet");
	if(pkNum==-1){pkNum=GetPkNum();}
	if(dstName==wxT("")){wxMessageBox(TEXT_IS_UNABLE_UPDATE_RECORD); return false;};
	if(pkNum==-1){wxMessageBox(TEXT_IS_UNABLE_UPDATE_RECORD_PK); return false;};
	s=wxT("UPDATE ")+dstName+wxT(" SET ");
	int fl=0;
	for(int i=0; i<(int)fieldsDescription.Count(); i++){
		if(fieldsDescription[i].GetProperty()&DB_FIELD_DBASE){
			if(fl)s=s+wxT(", ");
			s1 = GetColumnValueForSQL(as,i);
			if( s1 == wxT("''") ) s1 = wxT("null") ;
			s=s+fieldsDescription[i].fieldName+"="+ s1;//GetColumnValueForSQL(as,i);
			fl++;
		};
	};
	s=s+wxT(" WHERE ")+fieldsDescription[pkNum].fieldName+"="+GetColumnValueForSQL(as,pkNum);

	try{
		if(db) db->RunQuery(s);
	}
		catch (DatabaseLayerException& e)
	{
		wxMessageBox(s+wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
		return false;
	}	
	return true;
};

//Удалить запись
bool wxDbQueryObject::DoDelete(const wxArrayString& as){
	wxString s;
	if(pkNum==-1){pkNum=GetPkNum();}
	if(dstName==wxT("")){wxMessageBox(TEXT_IS_UNABLE_DELETE_RECORD); return false;};
	if(pkNum==-1){wxMessageBox(TEXT_IS_UNABLE_DELETE_RECORD_PK); return false;};
	s=wxT("DELETE FROM ")+dstName+wxT(" WHERE ")+fieldsDescription[pkNum].fieldName+"="+GetColumnValueForSQL(as,pkNum);
//	wxLogMessage("wxDbQueryObject::DoDelete "+s);
	try{
		if(db) db->RunQuery(s);
	}
		catch (DatabaseLayerException& e)
	{
		wxMessageBox(s+wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
		return false;
	}
	return true;
};

//Добавить запись
bool wxDbQueryObject::DoInsert(const wxArrayString& as){
wxString s, s1;
//wxLogMessage("wxDbQueryObject::DoInsert");
	if(pkNum==-1){pkNum=GetPkNum();}
	if(dstName==wxT("")){wxMessageBox(TEXT_IS_UNABLE_INSERT_RECORD); return false;};
	//if(pkNum==-1){wxMessageBox(TEXT_IS_UNABLE_INSERT_RECORD_PK); return false;};
	s=wxT("INSERT INTO  ")+dstName+wxT(" ( ");
	int fl=0;
	for(int i=0; i<(int)fieldsDescription.Count(); i++){
		if(fieldsDescription[i].GetProperty()&DB_FIELD_DBASE){
			if(fl)s=s+wxT(",");
			s=s+fieldsDescription[i].fieldName;
			fl++;
		};
	};
	s=s+wxT(" ) VALUES ( ");
	fl=0;
	for( i=0; i<(int)fieldsDescription.Count(); i++){
		if(fieldsDescription[i].GetProperty()&DB_FIELD_DBASE){
			if(fl)s=s+wxT(",");
			s1 = GetColumnValueForSQL(as,i);
			if( s1 == wxT("''") ) s1 = wxT("null") ;
			s=s+s1;//GetColumnValueForSQL(as,i);
			fl++;
		};
	};
	s=s+wxT(" )");
//    wxLogMessage(s);
	try{
		if(db) db->RunQuery(s);
	}
		catch (DatabaseLayerException& e)
	{
		wxMessageBox(s+wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
		return false;
	}
	return true;
};
//Получить номер поля первичного ключа
int wxDbQueryObject::GetPkNum(){
	for (int i=0; i<(int)fieldsDescription.Count(); i++){
		if(fieldsDescription[i].GetProperty()&DB_FIELD_KEY){pkNum=i;break;}
	};
	return pkNum;
};
//Установить номер поля первичного ключа
void wxDbQueryObject::SetPkNum(int nf)
{
	//pkNum=nf;
	if((int)fieldsDescription.Count()>nf+1)fieldsDescription[nf].SetProperty(DB_FIELD_KEY);

}


//Получить номер поля внешнего ключа
int wxDbQueryObject::GetFkNum(){
	for (int i=0; i<(int)fieldsDescription.Count(); i++){
		if(fieldsDescription[i].GetProperty()&DB_FIELD_FK){fkNum=i;break;}
	};
	return fkNum;
};
//Установить номер поля внешнего ключа
void wxDbQueryObject::SetFkNum(int nf)
{
	//fkNum=nf;
	if((int)fieldsDescription.Count()>nf+1)	fieldsDescription[nf].SetProperty(DB_FIELD_FK);

}

//Плучить значение поля для запроса 
//Надо сделать для даты ???
wxString wxDbQueryObject::GetColumnValueForSQL(const wxArrayString& as, int nf)
{
	wxString s;

	if (fieldsDescription[nf].valueType==DB_FIELD_STRING)
	{
		s=as[nf];
		s.Replace(wxT("'"),wxT("''"));
		s=wxT("'")+s+wxT("'"); return s; 
	}
	if (fieldsDescription[nf].valueType==DB_FIELD_INT || fieldsDescription[nf].valueType==DB_FIELD_FLOAT )
	{
		s=as[nf];
		s.Replace(wxT(","),wxT("."));
		if( as[nf]==wxEmptyString ) s=wxT("null");
		return s;
	}
	if ( fieldsDescription[nf].valueType == DB_FIELD_DATE )
	{

		if( as[nf] == wxEmptyString ) 
		{
			s=wxT("null");
			return s;
		};
		wxDateTime dt;
		dt.ParseFormat( as[nf],DATE_FORMAT );
		s=dt.Format("'%Y.%m.%d 00:00:00'");



		return s;
	}
	if ( fieldsDescription[nf].valueType == DB_FIELD_DATETIME )
	{

		if( as[nf] == wxEmptyString ) 
		{
			s=wxT("null");
			return s;
		};
		wxDateTime dt;

		dt.ParseFormat( as[nf],DATETIME_FORMAT );
		s=dt.Format("'%Y.%m.%d %H:%M:%S'");



		return s;
	}
	if ( fieldsDescription[nf].valueType == DB_FIELD_BOOL )
	{
		
		if( tree && tree->GetItemImage(tree->curId, nf) == 1 ) 
		{
			s=wxT("0");

		}
		else
		{
			s=wxT("1");
		}
	
		return s;
	}
	return s;
};

//Проверка значения поля
bool wxDbQueryObject::CheckValue(const wxString& s, int nf)
{

	return CheckVal(fieldsDescription[nf].columnName,s,fieldsDescription[nf].valueType,fieldsDescription[nf].len,fieldsDescription[nf].presition,fieldsDescription[nf].maxVal,fieldsDescription[nf].minVal,fieldsDescription[nf].GetProperty()&DB_FIELD_NOT_NULL,fieldsDescription[nf].minLen);
}




///Проверка соответсвует ли запись критериям поиска 
bool wxDbQueryObject::CheckSearch(wxArrayString as){

	wxString s;
	for( int i=0; i<(int)as.Count() && i<(int)searchData.Count(); i++)
	{
		s.sprintf("%d",i);

		if(searchData[i]!=wxEmptyString && !as[i].Contains(searchData[i]) && fieldsDescription[i].GetProperty()&DB_FIELD_SEARCH)
		{
		

			return false;
		}
	};
	return true;
}

///Стандартный обработчик перед вставкой в БД (формируется PK и FK)
bool wxDbQueryObject::DoPreInsert(const wxArrayString& as)
{

//	wxLogMessage("wxDbQueryObject::DoPreInsert");
 int n=GetPkNum();
 if(seqName!=wxEmptyString && n>=0){

	
	try{
//Определяем PK (сделано для FIREBIRD)
#ifdef FIREBIRD_DB
		
		 DatabaseResultSet *rs=db->RunQueryWithResults(wxT("SELECT CAST( NEXT VALUE FOR ")+seqName +wxT(" AS VARCHAR(18) ) FROM RDB$DATABASE"));
		 
		 rs->Next();
		 wxString s=DbGetValueAsStr(  rs,    rs->GetMetaData(), 1);
//		 wxLogMessage(s);
		 as[n]=s;
		 if(rs && db) db->CloseResultSet(rs);
#endif
//Определяем PK (сделано для ORACLE)
#ifdef ORACLE_DB
#endif
//Определяем PK (сделано для MySQL)
#ifdef MYSQL_DB
#endif
//Определяем PK (сделано для PostgreSQL)
#ifdef POSTGRESQL_DB
#endif




	}
	catch (DatabaseLayerException& e)
	{
			wxMessageBox(wxString::Format(_("wxDbQueryObject::DoPreInsert- Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
			return false;
	}
 };
	n=GetFkNum();
	//Определяем FK
	if(n>=0 && parent && parent->GetPkNum()>=0)
	{
		wxArrayString aa;
		parent->GetCurrentRecord(aa);
		if(aa.Count()>0) as[n]=aa[parent->GetPkNum()];

	}

	return true;

};


///Установить номер поля ссылки на дерево
void wxDbQueryObject::SetTreeNum(int nf)
{
	if((int)fieldsDescription.Count()>nf+1)	fieldsDescription[nf].SetProperty(DB_FIELD_TREE);

}

///Получить номер поля ссылки на дерево
int wxDbQueryObject::GetTreeNum(){
	for (int i=0; i<(int)fieldsDescription.Count(); i++){
		if(fieldsDescription[i].GetProperty()&DB_FIELD_TREE){fkNum=i;break;}
	};
	return fkNum;
};

///Проверка записи на ошибки
///Возвращает статус и номер поля ошибки
//bool CheckVal(const wxString& s, int valueType, int len =0, int presition=0,const wxString& maxVal=wxEmptyString,const wxString& minVal=wxEmptyString, bool notNull=false){
void wxDbQueryObject::CheckRecord(wxArrayString& as, int *nf, bool *status)
{

	for(int i=0; i<(int)as.Count()-1 && i<(int)fieldsDescription.Count()-1; i++)
	{
		if(!CheckVal(fieldsDescription[i].columnName,as[i],fieldsDescription[i].valueType,fieldsDescription[i].len,fieldsDescription[i].presition,fieldsDescription[i].maxVal,fieldsDescription[i].minVal,fieldsDescription[i].GetProperty()&DB_FIELD_NOT_NULL))
		{
			*nf=i; *status=false; return;
		};

	};
	*status=true;
	return;
};

///Добавить условия в готовый запрос , надо например когда не известны поля а запрос уже есть, когда связанные гриды и надо ограничить запрос
void wxDbQueryObject::AddWhere(wxString &s)
{

	wxString ss;
	wxString w=query.MakeUpper();

	wxString s1;
	int i=w.First(wxT(" WHERE "));
	if(i>0) i++;
	if(i<0)
	{
		i=w.First(wxT(" GROUP "));
	}
	else {i=i+5; s1=wxT(" ")+s+wxT(" and ");}
	if(i<0)
	{
		i=w.First(wxT(" ORDER "));
	}
	if(i <0 ) s1=wxT(" where ")+s+wxT(" "); 

	if(s1==wxEmptyString)	s1= wxT(" where ")+s+wxT(" "); 

	//ss.sprintf("%d",i);
	//wxLogMessage("wxDbQueryObject::AddWhere "+ss +query.SubString(0,i));
	if(i<0) query+=s1;
	else query=query.SubString(0,i-1)+s1+query.SubString(i,10000);




};


///Записать параметр в массив
bool wxDbSerializer::SavePar(ArrayDbRec &ar,wxString &pname,wxString &name,wxString &parname, wxString &val)
{
	for(int i=0; i<(int)ar.Count(); i++)
	{
		if(ar[i].Count()>=4){
		//	wxLogMessage("wxDbSerializer::SavePar "+ar[i][0]+ar[i][1]+ar[i][2]+ar[i][3]);
			if(ar[i][0]==name && ar[i][1]==pname && ar[i][2]==parname)
			{
				ar[i][3]=val; return true;
			};
		}
	};

	wxArrayString as;
	as.Add(name);
	as.Add(pname);
	as.Add(parname);
	as.Add(val);
	ar.Add(as);
	return true;
}
	///Считать параметр из массива
wxString wxDbSerializer::LoadPar(ArrayDbRec &ar,wxString &pname,wxString &name,wxString &parname)
{
	wxString s;
	for(int i=0; i<(int)ar.Count(); i++)
	{
		if(ar[i].Count()>=4){
		//	wxLogMessage("wxDbSerializer::LoadPar " +ar[i][0]+ar[i][1]+ar[i][2]+ar[i][3]);
			if(ar[i][0]==name && ar[i][1]==pname && ar[i][2]==parname)
			{
				return ar[i][3];
			};
		}
	};
	return s;

};

///Сохраняет координаты и размеры окна в массиве, идентификация по имени
bool wxDbSerializer::SaveWindowGUI(wxWindow *w,ArrayDbRec &ar,wxString &pName)
{
	if(!w) return false;
	wxString s,v,pn;
	wxRect r;
	s=w->GetName();
	r=w->GetRect();
	v.sprintf("%d",r.x);
	pn=wxT("x");
	SavePar(ar,pName,s,pn,v);
	v.sprintf("%d",r.y);
	pn=wxT("y");
	SavePar(ar,pName,s,pn,v);
	v.sprintf("%d",r.width);
	pn=wxT("width");
	SavePar(ar,pName,s,pn,v);
	v.sprintf("%d",r.height);
	pn=wxT("height");
	SavePar(ar,pName,s,pn,v);
	return true;
};
	///Загружает координаты и размеры окна из массива, идентификация по имени
bool wxDbSerializer::LoadWindowGUI(wxWindow *w,ArrayDbRec &ar,wxString &pName)
{
	if(!w) return false;
	wxString s,v,pn;
	wxRect r;
	long l;
	s=w->GetName();
	pn=wxT("x");
	v=LoadPar(ar,pName,s,pn);
	v.ToLong(&l); 
	r.x=l;
	pn=wxT("y");
	v=LoadPar(ar,pName,s,pn);
	v.ToLong(&l); 
	r.y=l;
	pn=wxT("width");
	v=LoadPar(ar,pName,s,pn);
	v.ToLong(&l); 
	r.width=l;
	pn=wxT("height");
	v=LoadPar(ar,pName,s,pn);
	v.ToLong(&l); 
	r.height=l;
	if(r.width>0 && r.height>0) w->SetSize(r);
	
	return true;
};

///Получить индекс kолонки по названию , возвращает -1 если не найдено
int wxDbQueryObject::GetFieldIndex( wxString s )
{
	for ( int i = 0; i < (int)fieldsDescription.Count(); i++ )
	{
		if( s.Upper() == fieldsDescription[i].fieldName.Upper() ) return i;
	};
	return -1;
}


///Установить свойство поля 
bool wxDbQueryObject::SetFieldPropertyByName(wxString fn, unsigned long property)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].SetProperty( property );
	}
	return true;
};

///Сбросить свойство поля
bool wxDbQueryObject::ResetFieldPropertyByName(wxString fn, unsigned long property)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].ResetProperty( property );
	}
	return true;
};

///Установить название колонки по имени
bool wxDbQueryObject::SetColumnNameByName(wxString fn, wxString name)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].columnName = name;
	}
	return true;
}

///Установить Pk по имени
bool wxDbQueryObject::SetPkNumByName(wxString fn)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		SetPkNum( i );
	}
	return true;
}
///Установить Fk по имени
bool wxDbQueryObject::SetFkNumByName(wxString fn)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		SetFkNum( i );
	}
	return true;
}
///Установить TreeKey по имени
bool wxDbQueryObject::SetTreeNumByName(wxString fn)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		SetTreeNum( i );
	}
	return true;
}

///Установить LOV по имени
bool wxDbQueryObject::SetLOVByName( wxString fn, wxDbLOV *lov )
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].lov = lov;
	}
	return true;
};

///Добавление связанного поля
bool wxDbQueryObject::AddLinkFieldByName( wxString fn, wxDbField *f)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		AddLinkField( f, i );
	}
	return true;
}



///Установить тип значения поля по имени - int valueType;
bool wxDbQueryObject::SetValueTypeByName(   wxString fn, int val)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].valueType = val;
	}
	return true;
}
///Установить значение для списка по имени (для типа DB_FIELD_LIST) - wxArrayString values; 
bool wxDbQueryObject::SetValuesByName(    wxString fn, wxArrayString val )
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].values = val;
	}
	return true;
}
///Установить значение для списка по имени  (для типа DB_FIELD_LIST) - 	wxArrayString values2; 
bool wxDbQueryObject::SetValues2ByName(    wxString fn, wxArrayString val )
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].values2 = val;
	}
	return true;
}
///Установить  длину поля по имени -	int len;
bool wxDbQueryObject::SetLenByName(   wxString fn, int val)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].len = val;
	}
	return true;
}
///Установить минимальная длина поля по имени  -	int minLen;
bool wxDbQueryObject::SetMinLenByName(   wxString fn, int val)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].minLen = val;
	}
	return true;
}
///Установить количество знаков после запятой по имени -	int presition;
bool wxDbQueryObject::SetPrecisionByName(   wxString fn, int val)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].presition = val;
	}
	return true;
}

///Установить значение по умолчанию по имени  -	wxString defValue;
bool wxDbQueryObject::SetDefValueByName( wxString fn, wxString val )
{
int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].defValue = val;
	}
	return true;
}
///Установить максимальное значение по имени -	wxString maxVal;
bool wxDbQueryObject::SetMaxValueByName( wxString fn, wxString val )
{
int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].maxVal = val;
	}
	return true;
}
///Установить минимальное значение по имени -	wxString minVal;
bool wxDbQueryObject::SetMinValueByName( wxString fn, wxString val )
{
int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].minVal = val;
	}
	return true;
}
///Установить валидатор по имени - 	wxValidator *validator;
bool wxDbQueryObject::SetValidatorByName( wxString fn, wxValidator *val )
{
int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		fieldsDescription[i].validator = val;
	}
	return true;
}
