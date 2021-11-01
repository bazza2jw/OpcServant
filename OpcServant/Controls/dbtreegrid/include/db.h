/////////////////////////////////////////////////////////////////////////////
// Name:        db.h
// Purpose:     Global variables
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////


//Глобальные объекты для работы с СУБД
#ifndef __db__
#define __db__

#include "../../databaselayer/include/DatabaseLayer.h"
#include "../../databaselayer/include/DatabaseErrorCodes.h"
#include "../../databaselayer/include/DatabaseLayerException.h"
#include "../../databaselayer/include/ResultSetMetaData.h"


//Коннест к СУБД
extern DatabaseLayer* dbase;

//Получение любого поля из  запроса в виде строки
wxString DbGetValueAsStr(  DatabaseResultSet* rs,    ResultSetMetaData* md, int nf, int len=0, int presition=0, bool datetime=false );
#endif