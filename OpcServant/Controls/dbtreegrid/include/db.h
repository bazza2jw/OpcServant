/////////////////////////////////////////////////////////////////////////////
// Name:        db.h
// Purpose:     Global variables
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////


//���������� ������� ��� ������ � ����
#ifndef __db__
#define __db__

#include "../../databaselayer/include/DatabaseLayer.h"
#include "../../databaselayer/include/DatabaseErrorCodes.h"
#include "../../databaselayer/include/DatabaseLayerException.h"
#include "../../databaselayer/include/ResultSetMetaData.h"


//������� � ����
extern DatabaseLayer* dbase;

//��������� ������ ���� ��  ������� � ���� ������
wxString DbGetValueAsStr(  DatabaseResultSet* rs,    ResultSetMetaData* md, int nf, int len=0, int presition=0, bool datetime=false );
#endif