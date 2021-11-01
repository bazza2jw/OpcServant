/////////////////////////////////////////////////////////////////////////////
// Name:        langdef.h
// Purpose:     National definitions and definition database type
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////


#ifndef __langdef__
#define __langdef__





#ifdef RUSSIAN_LANG

#define DEL_MENU_TEXT wxT("Удалить (Ctrl+Del)") 
#define ADD_MENU_TEXT wxT("Добавить (Ctrl+Ins)") 
#define ADD_CHILD_MENU_TEXT wxT("Добавить потомка (Ctrl+Alt+Ins)")
#define EXTERNAL_EDITOR_MENU_TEXT wxT("Внешний редактор (Ctrl+E)")
#define SEARCH_MENU_TEXT wxT("Поиск (Ctrl+F3)")
#define SEARCH_NEXT_MENU_TEXT wxT("Поиск следующего(Ctrl+Alt+F3)")
#define QUERY_MENU_TEXT wxT("Запрос (Ctrl+Q)")
#define SAVE_MENU_TEXT wxT("Сохранить (Ctrl+S)")
#define REFRESH_MENU_TEXT wxT("Обновить (Ctrl+R)")
#define PROPERTY_MENU_TEXT wxT("Свойства блока")
#define DUPLICATE_MENU_TEXT wxT("Дублировать запись (Ctrl+F4)")

#define TEXT_IS_CHANGE_IN_BLOCKS wxT("Есть изменеия в связанных блоках данных, изменения будут потеряны, продолжить? ")
#define TEXT_IS_CHANGE_IN_BLOCKS_DELETE wxT("Есть даные в связанных блоках данных, удалить запись нельзя ")
#define TEXT_IS_REQUE_VALUE wxT("CheckVal - Обязательное поле не заполнено")
#define TEXT_IS_STRING_OUT_OF_RANGE wxT("CheckVal - Строковое поле выходит за границы дипазона ")
#define TEXT_IS_WRONG_NUMBER wxT("CheckVal - Не верный формат целого числа")
#define TEXT_IS_INTEGER_OUT_OF_RANGE wxT("CheckVal - Целое поле выходит за границы дипазона ")
#define TEXT_IS_WRONG_FLOAT_NUMBER wxT("CheckVal - Не верный формат вещественного числа")
#define TEXT_IS_FLOAT_OUT_OF_RANGE wxT("CheckVal - Вещественное поле выходит за границы дипазона ")
#define TEXT_IS_WRONG_DATE  wxT("CheckVal - Не верное значение поля дата-время")
#define TEXT_IS_RECORD_PROCESS_ERROR  wxT("Ошибка обработки записи")
#define TEXT_IS_UNABLE_UPDATE_RECORD wxT("Нельзя обновить запись, не задана таблица для обновления")
#define TEXT_IS_UNABLE_UPDATE_RECORD_PK wxT("Нельзя обновить запись, не задан PK")
#define TEXT_IS_UNABLE_DELETE_RECORD wxT("Нельзя удалить запись, не задана таблица для обновления")
#define TEXT_IS_UNABLE_DELETE_RECORD_PK wxT("Нельзя удалить запись, не задан PK")
#define TEXT_IS_UNABLE_INSERT_RECORD wxT("Нельзя добавить запись, не задана таблица для обновления")
#define TEXT_IS_UNABLE_INSERT_RECORD_PK wxT("Нельзя добавить запись, не задан PK")
#define TEXT_IS_REPEAT_SET_FIELD wxT("Ошибка в wxDbTreeGrid повторная установка полей")
#define TEXT_IS_RECORD_HAS_SABORDINATE wxT("wxDbTreeGrid::OnDeleteRecord - В дереве у записи есть подчиненные данные, удалить запись нельзя ")
#define TEXT_IS_REQURE_CHOICE wxT("wxDbTreeGrid::DeleteCurrentRecord - Надо выбрать запись")
#define TEXT_IS_REQURE_CHOICE_CUR_REC wxT("wxDbTreeGrid::GetCurrentRecord- Надо выбрать запись")
#define TEXT_IS_SAVE_ERROR wxT("wxDbTreeGrid::Save- При записи изменений произошла ошибка")
#define TEXT_IS_DELETE_ERROR wxT("wxDbTreeGrid::Save- При удалении записей произошла ошибка")
#define TEXT_IS_CHANGES  wxT("wxDbTreeGrid::OnRefreshRecord - Есть изменеия, обновить без сохранения?")
#define TEXT_IS_CHANGES_PERFOM_QUERY wxT("wxDbTreeGrid::OnQueryRecord - Есть изменения, выполнить запрос без сохранения?")
#define TEXT_IS_LIST_NOT_SET wxT("wxDbTreeGrid::EditChoice- Не задан список значений для поля типа выбор из списка")
#define TEXT_IS_MOVE_NODE wxT("Невозможно переместить узел")
#define TEXT_IS_SAVE_FILE wxT("Ошибка не возможно сохранить файл - ")
#define TEXT_IS_BROWSER_ERROR wxT("Ошибка запуска Веб браузера")
#define DATE_FORMAT  wxT("%d.%m.%Y")	
#define DATETIME_FORMAT  wxT("%d.%m.%Y %H:%M:%S")	
#define FIREBIRD_DB

#else

#define DEL_MENU_TEXT wxT("Delete (Ctrl+Del)") 
#define ADD_MENU_TEXT wxT("Insert (Ctrl+Ins)") 
#define ADD_CHILD_MENU_TEXT wxT("Insert child (Ctrl+Alt+Ins)")
#define EXTERNAL_EDITOR_MENU_TEXT wxT("External editor (Ctrl+E)")
#define SEARCH_MENU_TEXT wxT("Search (Ctrl+F3)")
#define SEARCH_NEXT_MENU_TEXT wxT("Search next (Ctrl+Alt+F3)")
#define QUERY_MENU_TEXT wxT("Query (Ctrl+Q)")
#define SAVE_MENU_TEXT wxT("Save (Ctrl+S)")
#define REFRESH_MENU_TEXT wxT("Refresh (Ctrl+R)")
#define PROPERTY_MENU_TEXT wxT("Property")
#define DUPLICATE_MENU_TEXT wxT("Duplicate record (Ctrl+F4)")


#define TEXT_IS_CHANGE_IN_BLOCKS wxT("There are changes in the associated data blocks, changes will be lost, continue? ")
#define TEXT_IS_CHANGE_IN_BLOCKS_DELETE wxT("There are changes in the associated data blocks, impossible to delete a record")
#define TEXT_IS_REQUE_VALUE wxT("CheckVal - Required field is not filled")
#define TEXT_IS_STRING_OUT_OF_RANGE wxT("CheckVal - The value of the string field is out of range ")
#define TEXT_IS_WRONG_NUMBER wxT("CheckVal - Wrong number format")
#define TEXT_IS_INTEGER_OUT_OF_RANGE wxT("CheckVal - The value of the integer field is out of range ")
#define TEXT_IS_WRONG_FLOAT_NUMBER wxT("CheckVal - Wrong float number format")
#define TEXT_IS_FLOAT_OUT_OF_RANGE wxT("CheckVal - The value of the float field is out of range ")
#define TEXT_IS_WRONG_DATE  wxT("CheckVal - Wrong date time format")
#define TEXT_IS_RECORD_PROCESS_ERROR  wxT("Error processing record")
#define TEXT_IS_UNABLE_UPDATE_RECORD wxT("Unable to update the record, the table not set")
#define TEXT_IS_UNABLE_UPDATE_RECORD_PK wxT("Unable to update the record, PK not set")
#define TEXT_IS_UNABLE_DELETE_RECORD wxT("Unable to delete the record, the table not set")
#define TEXT_IS_UNABLE_DELETE_RECORD_PK wxT("Unable to delete the record, PK not set")
#define TEXT_IS_UNABLE_INSERT_RECORD wxT("Unable to insert the record, the table not set")
#define TEXT_IS_UNABLE_INSERT_RECORD_PK wxT("Unable to insert the record, PK not set")
#define TEXT_IS_REPEAT_SET_FIELD wxT("Error in wxDbTreeGrid - reinstall  fields")
#define TEXT_IS_RECORD_HAS_SABORDINATE wxT("wxDbTreeGrid::OnDeleteRecord - Record has a subordinate, impossible to delete a record ")
#define TEXT_IS_REQURE_CHOICE wxT("wxDbTreeGrid::DeleteCurrentRecord - You must choose an record")
#define TEXT_IS_REQURE_CHOICE_CUR_REC wxT("wxDbTreeGrid::GetCurrentRecord- You must choose an record")
#define TEXT_IS_SAVE_ERROR wxT("wxDbTreeGrid::Save- SAVE ERROR")
#define TEXT_IS_DELETE_ERROR wxT("wxDbTreeGrid::Save- DELETE ERROR")
#define TEXT_IS_CHANGES  wxT("wxDbTreeGrid::OnRefreshRecord - There is a change, update without saving?")
#define TEXT_IS_CHANGES_PERFOM_QUERY wxT("wxDbTreeGrid::OnQueryRecord - There is a change, perform query without saving?")
#define TEXT_IS_LIST_NOT_SET wxT("wxDbTreeGrid::EditChoice- Do not set a list of values for field")
#define TEXT_IS_MOVE_NODE wxT("Unable to move node")
#define TEXT_IS_SAVE_FILE wxT("Error save file - ")
#define TEXT_IS_BROWSER_ERROR wxT("Error show browser")

#define DATE_FORMAT  wxT("%Y-%m-%d")	
#define DATETIME_FORMAT  wxT("%d.%m.%Y %H:%M:%S")	

#endif






#ifndef ORACLE_DB
#ifndef MYSQL_DB
#ifndef POSTGRESQL_DB

#define FIREBIRD_DB

#endif
#endif
#endif



#endif