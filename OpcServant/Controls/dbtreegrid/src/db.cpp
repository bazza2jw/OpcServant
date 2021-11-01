/////////////////////////////////////////////////////////////////////////////
// Name:        db.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/db.h"
#include "../include/langdef.h"
DatabaseLayer* dbase;




wxString DbGetValueAsStr(   DatabaseResultSet* rs,    ResultSetMetaData* md, int nf, int len, int presition,bool datetime){
	wxString s;
	wxString fs;
	s.sprintf("DbGetValueAsStr %ld",md->GetColumnType(nf));

	try{
		switch (md->GetColumnType(nf)){

		case (int)ResultSetMetaData::COLUMN_DOUBLE :
			fs.sprintf("\%.%dLf",presition);
			s.sprintf(/*"%f"*/fs.c_str(),rs->GetResultDouble(nf));
			if(rs->IsFieldNull(nf)) s=wxT("");
//			wxLogMessage(" DOUBLE DbGetValueAsStr "+s);
			break;
		case ResultSetMetaData::COLUMN_INTEGER :
			s.sprintf("DbGetValueAsStr INTEGER %d",rs->GetResultLong(nf));
		//	wxLogMessage("DbGetValueAsStr "+s);
			s.sprintf("%ld",rs->GetResultLong(nf));
			if(rs->IsFieldNull(nf)) s=wxT("");
//			wxLogMessage(" DOUBLE DbGetValueAsStr "+s);
			break;
		case ResultSetMetaData::COLUMN_STRING :
			s=rs->GetResultString(nf);
		//	wxLogMessage("DbGetValueAsStr "+s);
			break;
		case ResultSetMetaData::COLUMN_DATE :
			wxDateTime t;
			t=rs->GetResultDate(nf);
			if(!rs->IsFieldNull(nf))
			{
				if(!datetime) s=t.Format(DATE_FORMAT);
				else 
				{
				//	wxLogMessage("DbGetValueAsStr DATETIME");
					s=t.Format(DATETIME_FORMAT);
				};
			}
			else s=wxT("");
			//wxLogMessage("DbGetValueAsStr "+s);
			//s=t.Format ("%d.%m.%Y");
			break;
	//	default:
	//		s=rs->GetResultString(nf);
		}
		
	}
		catch (DatabaseLayerException& e)
	{
		wxMessageBox(wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
	}
	return s;

};


wxString GetKladrAdr( const wxString &kId )
{
		wxString s,s1;
	
		DatabaseResultSet* dbq;
//		ResultSetMetaData *md;
		if ( kId == wxEmptyString ) return s;
		
		try{
				dbq=dbase->RunQueryWithResults("select * from kladr_name(" + kId +")" );
				while(dbq->Next())
				{
					s =  dbq->GetResultString(1) ;
				}

				dbase->CloseResultSet(dbq);
		}
		catch (DatabaseLayerException& e)
		{
					if(dbq )  {	dbase->CloseResultSet(dbq); dbq=0; } 
					wxMessageBox(wxString::Format(_("GetKladrAdr - Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
		
		}

		return s;
}

//«апись BLOB в поле таблицы, передаетс€ update где 1 поле blob
//например update tb set picture = ? where id=10;
bool WriteBlob(DatabaseLayer *db, wxString &sql, wxMemoryBuffer &buf)
{
//	wxLogMessage("WriteBlob " + sql);
	PreparedStatement* pStatement;
	DatabaseResultSet* pResultSet;
	try
	{
	      pStatement = db->PrepareStatement(sql);
	
          if (pStatement)
          {
			  pStatement->SetParamBlob(1, buf.GetData(), buf.GetDataLen());
		
		  }
		  pResultSet = pStatement->RunQueryWithResults();
		
		 // if(pResultSet )  {	dbase->CloseResultSet(pResultSet); } 
		 // if(pStatement )  {	dbase->CloseStatement(pStatement); } 


	}
	catch (DatabaseLayerException& e)
	{
		//if(pResultSet )  {	dbase->CloseResultSet(pResultSet); } 
		//if(pStatement )  {	dbase->CloseStatement(pStatement); } 
		
		wxMessageBox(wxString::Format(_("WriteBlob - Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
		return false;
	}
	return true;
}


//„тение blob из таблицы передаетс€ запрос с одним полем
//select picture from tb where id= 10
bool ReadDBlob(DatabaseLayer *db, wxString &sql, wxMemoryBuffer &buf)
{



	
//	wxLogMessage("ReadBlob " + sql);
	try
	{

		  db->GetSingleResultBlob ( sql, 1, buf, false);


	}
	catch (DatabaseLayerException& e)
	{
		
		
		wxMessageBox(wxString::Format(_("wxDbQueryObject::SetupField - Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
		return false;
	}
	return true;
}
