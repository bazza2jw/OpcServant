/////////////////////////////////////////////////////////////////////////////
// Name:        dbstruct.h
// Purpose:     Classes to describe the data structures
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

//Объекты для представления структуры данных таблиц
#ifndef __dbstruct__
#define __dbstruct__
#include <wx/wx.h>
#include <wx/grid.h>
#include "db.h"
#include <wx/dynarray.h>
#include "dbfield.h"
#include <wx/validate.h>




///Массив записей удаленных
WX_DECLARE_OBJARRAY( wxArrayString, ArrayDbRec );



/**
@class wxDbSerializer
Класс для чтения и записи настроек GUI в массив
*/
///Класс для чтения и записи настроек GUI в массив
class wxDbSerializer
{
	public:
		///Конструктор
		wxDbSerializer(){};
		~wxDbSerializer(){};
	///Сохранить настройки GUI в массив
	virtual bool SaveGUI(ArrayDbRec &ar,wxString &pName){return true;};
	///Загрузить настройки GUI из массива
	virtual bool LoadGUI(ArrayDbRec &ar,wxString &pName){return true;};
	///Записать параметр в массив
	static bool SavePar(ArrayDbRec &ar,wxString &pname,wxString &name,wxString &parname, wxString &val);
	///Считать параметр из массива
	static wxString LoadPar(ArrayDbRec &ar,wxString &pname,wxString &name,wxString &parname);
	///Сохраняет координаты и размеры окна в массиве, идентификация по имени
	static bool SaveWindowGUI(wxWindow *w,ArrayDbRec &ar,wxString &pName);
	///Загружает координаты и размеры окна из массива, идентификация по имени
	static bool LoadWindowGUI(wxWindow *w,ArrayDbRec &ar,wxString &pName);

};


class wxDbDispatcher;

/**
@class wxDbPair
Класс соответствия колонок, используется в wxDbLOV
а также ширину поля в LOV, если i2==-1 то учитываем только ширину
*/
///Класс соответствия колонок, используется в wxDbLOV
class wxDbPair
{
public:
	///Конструктор
	wxDbPair(){i2=-1;w=-1;};
	///номер колонки в LOV 
	int i1;
	///номер колонки в вызываемом гриде
	int i2;
	///Ширина колнки в LOV
	int w;
	///Имя поля в БД
	wxString fieldName;
};

WX_DECLARE_OBJARRAY( wxDbPair, ArraywxDbPair );

/**
@class wxDbLOV
Класс описатель для списка выбора значений из БД
*/
///Класс описатель для списка выбора значений из БД

class wxDbLOV
{
public:
	///Конструктор
	wxDbLOV(){qTime=true;};
	///Начало запроса начиная с select до фразы from
	wxString selectClause;
	///Часть запроса начиная с from до фразы where
	wxString fromClause;
	///Часть запроса начина с where до фразы order 
	wxString whereClause;
	///Часть запроса начиная с order и до конца
	wxString orderClause;
	wxString query;
	///Размер диалога LOV
	wxSize size;
	///Описатель полей LOV
	ArraywxDbPair pair;
	///Выполнять запрос сразу по входу - true или после ввода строки поиска
	bool qTime;

};

WX_DECLARE_OBJARRAY( wxDbLOV, ArraywxDbLOV );




const int 		DB_FIELD_EDITABLE=0x1;
const int 		DB_FIELD_VISIBLE=0x2;
const int		DB_FIELD_KEY=0x4;//Поле ключ таблицы
const int 		DB_FIELD_TREE=0x8;//Поле ссылка на родителя для дерева
const int		DB_FIELD_DBASE=0x10;
///Разрешен запрос 
const int		DB_FIELD_QUERY=0x20;
///Поиск разрешен
const int		DB_FIELD_SEARCH=0x40;
///Внешний ключ
const int		DB_FIELD_FK=0x80;
///Пользовательский LOV
const int		DB_FIELD_USER_LOV=0x100;
///Не пустое
const int		DB_FIELD_NOT_NULL=0x200;
///Двойное редактирование через LOV или в гриде
const int		DB_FIELD_LOV_AND_EDIT=0x400;

///Типы значений для поля

const int DB_FIELD_STRING=0;
const int DB_FIELD_INT=1;
const int DB_FIELD_FLOAT=2;
const int DB_FIELD_DATE=3;
const int DB_FIELD_LIST=4;
const int DB_FIELD_BOOL=5;
const int DB_FIELD_FLAGS=6;//Поле свойств записи  
const int DB_FIELD_DATETIME=7;




///Типы состояния записи
const int DB_QUERY_RECORD=0x1;//Запись получена из запроса
const int DB_INSERTED_RECORD=0x2;//Запись для вставки
const int DB_DELETED_RECORD=0x4;//Удаленная запись
const int DB_UPDATED_RECORD=0x8;//Измененная запись
const int DB_TREE_QUERY=0x10;//Не были прочитаны подчиненные записи




/**
@class wxDbGridFieldDesc
Описатель поля для грида  @n

Константы описания поля @n
DB_FIELD_EDITABLE=0x1; @n
DB_FIELD_VISIBLE=0x2; @n
Поле ключ таблицы  DB_FIELD_KEY=0x4; @n
Поле ссылка на родителя для дерева DB_FIELD_TREE=0x8; @n
DB_FIELD_DBASE=0x10;@n
Разрешен запрос DB_FIELD_QUERY=0x20; @n
Поиск разрешен DB_FIELD_SEARCH=0x40; @n
Внешний ключ	DB_FIELD_FK=0x80; @n
Пользовательский LOV		DB_FIELD_USER_LOV=0x100; @n
Не пустое	DB_FIELD_NOT_NULL=0x200; @n

Типы значений для поля @n

DB_FIELD_STRING=0;@n
DB_FIELD_INT=1;@n
DB_FIELD_FLOAT=2;@n
DB_FIELD_DATE=3;@n
DB_FIELD_LIST=4;@n
DB_FIELD_BOOL=5;@n
Поле свойств пока DB_FIELD_FLAGS=6; @n
DB_FIELD_DATETIME=7;@n

*/
///Описатель поля для грида 
class wxDbGridFieldDesc {
public:
	
	///Конструктор
	wxDbGridFieldDesc(){boolProperty=(int)DB_FIELD_EDITABLE|DB_FIELD_VISIBLE|DB_FIELD_QUERY|DB_FIELD_SEARCH; valueType=DB_FIELD_STRING;lov=0;validator=0;minLen = 0; presition = 0;};
	///Конструктор
	wxDbGridFieldDesc(const wxString& label,const wxString& fname,unsigned long flag=DB_FIELD_EDITABLE|DB_FIELD_VISIBLE,
		int tp=DB_FIELD_STRING);
	~wxDbGridFieldDesc(){};
	///Сделалть поле видимым
	void SetVisible(bool value){  (value ) ? boolProperty|=DB_FIELD_VISIBLE :  boolProperty&=~DB_FIELD_VISIBLE;}
	bool GetVisible(){ if (boolProperty&DB_FIELD_VISIBLE) return true; else return false;}
	///Установить свойство поля - редактируемое
	void SetEditable(bool value){  (value ) ? boolProperty|=DB_FIELD_EDITABLE :  boolProperty&=~DB_FIELD_EDITABLE;}
	bool GetEditable(){return boolProperty&DB_FIELD_EDITABLE;}
	///Установить тип данных для поля
	void SetFieldValueType(int value){valueType=value;}
	int GetFieldValueType(){return valueType;}
	///Установить название поля в БД
	void SetFieldName(wxString value){fieldName=value;}
	wxString& GetFieldName(){return fieldName;};
	///Установить название колонки в гриде
	void SetColumnName(wxString value){columnName=value;}
	wxString& GetColumnName(){return columnName;};
	//Не помню ???
	//void SetColumnOperator(wxString value){columnOperator=value;}
	//wxString& SetColumnOperator(){return columnOperator;};
	///Установить длину поля, для строковых - количество символов, для целых длина , для вещественных количество целых
	void SetLength(int value){len=value;}
	int GetLength(){return len;}
	///Установить точность для вещественных чисел
	void SetPresition(int value){presition=value;}
	int GetPresition(){return presition;}
	
	///Задать список значений для поля типа список, отображаются в гриде
	void SetListValues(wxArrayString& value){values=value;}
	wxArrayString& GetListValues(){return values;}
	///Задает список реальных значений для списка , из БД
	void SetListValues2(wxArrayString& value){values2=value;}
	wxArrayString& SetListValues2(){return values2;}
	///Получить свойства
	unsigned long GetProperty(){return boolProperty;};
	///Установить свойство
	void SetProperty(unsigned long property){  boolProperty|=property ;}
	///Сбросить свойство
	void ResetProperty(unsigned long property){   boolProperty&=~property;}




	/// Имя поля в БД
	wxString fieldName;
	///Имя поля в гриде
	wxString columnName;
	///Когда значение поля получаем через функцию пример   SUM(price) total, пока не используется
	wxString columnOperator;
	///Тип значения поля
	int valueType;
	///Значения для списка (для типа DB_FIELD_LIST)
	wxArrayString values; 
	///Значения для списка  (для типа DB_FIELD_LIST)
	wxArrayString values2; 
	/// field length
	int len;
	///Минимальная длина поля 
	int minLen;
	///Количество знаков после запятой
	int presition;
	///Список значений для поля(для типа DB_FIELD_CHOICE)
	wxDbLOV *lov;
	///Значение по умолчанию
	wxString defValue;
	///Максимальное значение
	wxString maxVal;
	///Минимальное значение
	wxString minVal;
	///Валидатор
	wxValidator *validator;
private:
	//Свойства поля
	unsigned long boolProperty;

	




};


WX_DECLARE_OBJARRAY( wxDbGridFieldDesc, ArraywxDbGridFieldDesc );


class wxDbTreeGrid;


const int DB_SAVE_HANDLE=1;
const int DB_PRE_INSERT_HANDLE=2;
const int DB_ON_INSERT_HANDLE=3;
const int DB_POST_INSERT_HANDLE=4;
const int DB_PRE_UPDATE_HANDLE=5;
const int DB_ON_UPDATE_HANDLE=6;
const int DB_POST_UPDATE_HANDLE=7;
const int DB_PRE_DELETE_HANDLE=8;
const int DB_ON_DELETE_HANDLE=9;
const int DB_POST_DELETE_HANDLE=10;
const int DB_POST_QUERY_HANDLE=11;
const int DB_POST_RECORD_HANDLE=12;
const int DB_PRE_RECORD_HANDLE=13;
const int DB_DELETE_HANDLE=14;
const int DB_CUSTOM_LOV_HANDLE=15;
const int DB_POST_FIELD_CHANGED_HANDLE=16;


const int DB_OBJ_TYPE_TREE_GRID=1;
class wxDbQueryObject;


/**
@class wxDbRecEvent
Класс события для обработчиков измений в БД
*/
///Класс события для обработчиков измений в БД
class wxDbRecEvent : public wxNotifyEvent{
public:
   wxDbRecEvent():wxNotifyEvent(){as=0;objType=0;tp=0;veto=false;obj=0;};
   wxDbRecEvent(const wxDbRecEvent& event)
	   : wxNotifyEvent(event){
	   as=event.as;
	   tp=event.tp;
   };
   wxDbRecEvent (wxEventType commandEventType, int id=0);
   virtual wxEvent *Clone() const { return new wxDbRecEvent(*this); }
public:
	///Строка с полями обрабатываемой записи
	wxArrayString *as;
	///Тип объекта например wxDbTreeGrid
	int objType;
	///Тип события
	int tp;
	///Флаг если установлен обработчиком то запись нельзя записать прекращаем транзакцию (для обработчиков связанных с записью в БД)
	bool veto;
	wxDbQueryObject *obj;
};


/**
@class wxDbGridEvent
Класс события для грида
*/
///Класс события для грида

class wxDbGridEvent : public wxNotifyEvent{
public:
   wxDbGridEvent():wxNotifyEvent(){grid=0;};
   wxDbGridEvent(const wxDbGridEvent& event)
	   : wxNotifyEvent(event){
	   grid=event.grid;
   };
   wxDbGridEvent (wxEventType commandEventType, int id=0);
   virtual wxEvent *Clone() const { return new wxDbGridEvent(*this); }
public:
	///Грид
	wxDbQueryObject *grid;
};




BEGIN_DECLARE_EVENT_TYPES()

    DECLARE_EVENT_TYPE(dbEVT_REC_HANDLE, 7100)
	DECLARE_EVENT_TYPE(dbEVT_GRID_ACTIVE_HANDLE, 7103)


END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*dbRecEventFunction)(wxDbRecEvent&);
typedef void (wxEvtHandler::*dbGridEventFunction)(wxDbGridEvent&);

 #define dbRecEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(dbRecEventFunction, &func)
///Обработчик событий связанных с записью
 #define EVT_REC_HANDLE( winid, func) \
    wx__DECLARE_EVT1(dbEVT_REC_HANDLE, winid, dbRecEventHandler(func))

#define dbGridEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(dbGridEventFunction, &func)
///Обработчик событий связанных с активацией грида
#define EVT_GRID_ACTIVE_HANDLE( winid, func) \
    wx__DECLARE_EVT1(dbEVT_GRID_ACTIVE_HANDLE, winid, dbGridEventHandler(func))


/// Свойства блока данных
	const int DB_GRID_INSERT_ENABLE=0x1;
	const int DB_GRID_UPDATE_ENABLE=0x2;
	const int DB_GRID_DELET_ENABLE=0x4;
	const int DB_GRID_DELETE_ENABLE=0x4;
	///Грид есть дерево
	const int DB_GRID_IS_TREE=0x8;
	///Есть изменения надо записать
	const int DB_GRID_IS_CHANGED=0x10;
	///Считывать все записи из БД за одну операцию
	const int DB_GRID_READ_ONE=0x20;
	///Все записи получены из БД
	const int DB_GRID_ALL_RECORD_FETCHED=0x40;
	///Запрос к БД разрешен
	const int DB_GRID_QUERY_ENABLE=0x80;
	///Разрешено обновлять записи запросом
	const int DB_GRID_REFRESH_ENABLE=0x100;
	///Дублирование записей разрешено
	const int DB_GRID_DUBLICATE_ENABLE=0x200;
/// Количество записей считываемых из БД за одну операцию
const int NEXT_NUMBER_ROWS=10;

/**
@class wxDbQueryObject
Класс связь грида с СУБД @n

Типы состояния записи @n

Запись получена из запроса DB_QUERY_RECORD=0x1;@n
Запись для вставки DB_INSERTED_RECORD=0x2;@n
Удаленная запись DB_DELETED_RECORD=0x4;@n
Измененная запись DB_UPDATED_RECORD=0x8;@n
Не были прочитаны подчиненные записи const int DB_TREE_QUERY=0x10;@n

Свойства блока данных @n

DB_GRID_INSERT_ENABLE=0x1; @n
DB_GRID_UPDATE_ENABLE=0x2; @n
DB_GRID_DELET_ENABLE=0x4; @n
Грид есть дерево DB_GRID_IS_TREE=0x8; @n
Есть изменения надо записать DB_GRID_IS_CHANGED=0x10; @n
Считывать все записи из БД за одну операцию  DB_GRID_READ_ONE=0x20; @n
Все записи получены из БД DB_GRID_ALL_RECORD_FETCHED=0x40; @n
Запрос к БД разрешен DB_GRID_QUERY_ENABLE=0x80; @n
Разрешено обновлять записи запросом  DB_GRID_REFRESH_ENABLE=0x100; @n

*/
///Класс связь грида с СУБД
class wxDbQueryObject : public wxDbSerializer {

public:
	///Конструктор
	wxDbQueryObject()
	{
		db=0;dbq=0;boolProperty=DB_GRID_DUBLICATE_ENABLE|DB_GRID_INSERT_ENABLE|DB_GRID_UPDATE_ENABLE|DB_GRID_DELET_ENABLE|DB_GRID_READ_ONE|DB_GRID_ALL_RECORD_FETCHED|DB_GRID_QUERY_ENABLE|DB_GRID_REFRESH_ENABLE;pkNum=-1;treeNum=-1;fkNum=-1;dstName=wxT("");
		saveHandler=0;
		preInsertHandler=0;
		onInsertHandler=0;
		postInsertHandler=0;
		preUpdateHandler=0;
		onUpdateHandler=0;
		postUpdateHandler=0;
		preDeleteHandler=0;
		onDeleteHandler=0;
		postDeleteHandler=0;
		postQueryHandler=0;
		postRecordHandler=0;
		preRecordHandler=0;
		deleteHandler=0;
		customLOVHandler=0;
		postFieldChangedHandle=0;
		parent=0;
		dispatcher=0;
		rcount = 0;
		tree = 0;
	};
	~wxDbQueryObject();
	///Получить обработчик событий, это порожденный класс
	virtual wxEvtHandler* GetEvtHandler(){return 0;} 
protected:
	///Свойства блока данных
	unsigned long boolProperty;

	
	///Массив со связанными полями 
	ArraywxDbFieldLink linkFields;
	///Количество записей в гриде
	int rcount;
	///Текущее дерево
	wxDbTreeGrid *tree;
	///Массив списков значений
	ArraywxDbLOV lov;
	/// Массив удаленных записей
	ArrayDbRec recArray;
public:
	///Начало запроса начиная с select до фразы from
	wxString selectClause;
	///Часть запроса начиная с from до фразы where
	wxString fromClause;
	///Часть запроса начина с where до фразы order 
	wxString whereClause;
	///Часть запроса начиная с order и до конца
	wxString orderClause;
	///Таблица куда записываем данные
	wxString dstName;
	///Значения для запроса введенные пользователем
	wxArrayString queryData;
	///Значения для поиска введенные пользователем
	wxArrayString searchData;
	///Значение фразы where  для поиска
	wxString queryDataWhere;
	///Сгенерированный запрос через createQuery(). Можно сразу сюда поместить запрос но тогда не будет работать фильтр и упорядочивание
	///Запрос не будет генериться автоматически если fromClause пусто
	///Надо определить как будут называться поля которые не будут отображаться!!! например когда оканчивается на ___
	wxString query;
	///БД дескриптор
	DatabaseLayer* db;
	///Результат запроса	
	DatabaseResultSet* dbq;
	///Обработчики событий классы  wxEvtHandler используется метод ProcessEvent для вызова обработчиков связанных с внесением изменений в БД
	///Обработчик сохранения, вызывается вместо стандартного, используется в основном когда 
	///несколько связанных гридов и надо выполнить сохранение в определенном порядке
	wxEvtHandler *saveHandler;
	///Вызывается перед вставкой записи в БД
	wxEvtHandler *preInsertHandler;
	///Вызывается вместо стандартной процедуры вставки записи
	wxEvtHandler *onInsertHandler;
	///Вызывается после вставки записи
	wxEvtHandler *postInsertHandler;
	///Вызывается перед обновлением записи
	wxEvtHandler *preUpdateHandler;
	///Вызывается вместо стандартной процедуры вставки записи
	wxEvtHandler *onUpdateHandler;
	///Вызывается после обновления записи
	wxEvtHandler *postUpdateHandler;
	///Вызывается перед удалением записи
	wxEvtHandler *preDeleteHandler;
	///Вызывается вместо стандартной процедуры удаления записи
	wxEvtHandler *onDeleteHandler;
	///Вызывается после удаления записи
	wxEvtHandler *postDeleteHandler;
	///Вызывается после получения очередной записи из БД
	wxEvtHandler *postQueryHandler;
	///Вызывается после ухода с записи(контекст запись с которой уходим)
	wxEvtHandler *postRecordHandler;
	///Вызывается при выборе новой записи (контекст запись которую выбрали)
	wxEvtHandler *preRecordHandler;
	///Вызывается при удалении записи пользователем
	wxEvtHandler *deleteHandler;
	///Обработчик пользовательского LOV
	wxEvtHandler *customLOVHandler;
	///Обработчик изменения значения поля, вызывается после изменения значения через интерфейс
	wxEvtHandler *postFieldChangedHandle;

	///Имя sequence для PK
	wxString seqName;
	///Ссылка на родительский грид(используется для формирования FK)
	wxDbQueryObject *parent;


	///Диспетчер
	wxDbDispatcher *dispatcher;
	///Контексное меню для редактирования
	wxMenu * menu;
	///Номер поля первичного ключа
	int pkNum;
	///Номер поля слыки на родителя в дереве
	int treeNum;
	///Номер поля ссылки на родительский грид
	int fkNum;

	///Количество записей получаемых за одну операцию считывания, когда не установлено св-во получить все записи за раз
	int nRows;

	///Описания полей

	///Описания полей, либо задаем вручную, можно на основании списка полей сгенерить select фразу.
	///Либо получить список полей из запроса c помощью setupField()
	
	ArraywxDbGridFieldDesc fieldsDescription;	
public:
	///Добавление описателя полей
	virtual void SetFieldDescription(ArraywxDbGridFieldDesc& value){fieldsDescription=value;};
	///Добавление описателя полей
	virtual void SetFieldDescription(ArraywxDbGridFieldDesc* value){fieldsDescription=*value;};
	///Добавление описателя полей
	virtual void AddFieldDescription(wxDbGridFieldDesc* value){fieldsDescription.Add(value);};
	///Получение массива описания полей
	virtual ArraywxDbGridFieldDesc* GetFieldDescription(){return &fieldsDescription;};
	///Формирование описателей полей из запроса
	virtual bool SetupField();
	///Заполнить запись данными из массива строк
	virtual void FillRecord(wxArrayString&,  long ){};
	///Установить свойство
	virtual void SetProperty(unsigned long property){  boolProperty|=property ;}
	///Сбросить свойство
	virtual void ResetProperty(unsigned long property){   boolProperty&=~property;}
	///Получить определенное свойство 
	virtual bool GetProperty(unsigned long property){return (boolProperty&property)>0;}
	///Получить все свойства
	virtual unsigned long GetProperty(){return boolProperty;}
	///Установить таблицу для записи
	virtual void SetDstName(const wxString& s){dstName=s;}
	///Получиь таблицу для записи
	virtual wxString GetDscName(){return dstName;}
	///Создание запроса из частей заданных в selectClause fromClause...
	virtual void CreateQuery();
	///Создание запроса для LOV
	virtual void CreateLOVQuery();
	///Создание selectClause на основании описания полей
	virtual void CreateSelectClause();
	///Выполняет SQL запрос
	virtual void ExecuteQuery();
	///Получение очередной строки из БД если строка считана то возвращает true
	virtual bool GetNextRecord(wxArrayString& lst);
	///Обработка записи вставка удаление изменение 
	virtual bool ProcessRecord(wxArrayString& as,wxDbTreeGrid *gr);
	///Проверка соответсвует ли запись критериям поиска 
	virtual bool CheckSearch(wxArrayString as);
	///Обновить запись
	bool DoUpdate(const wxArrayString& as);
	///Добавить запись
	bool DoInsert(const wxArrayString& as);
	///Удалить запись
	bool DoDelete(const wxArrayString& as);
	///Стандартный обработчик перед вставкой в БД (формируется PK и FK)
	bool DoPreInsert(const wxArrayString& as);

	///Получить количество обрабатываемых записей
	int GetRowCount(){ return rcount; };
	///Получить номер поля первичного ключа
	int GetPkNum();
	///Установить номер поля первичного ключа
	void SetPkNum(int nf);
    ///Получить номер поля внешнего ключа
	int GetFkNum();
	///Установить номер поля внешнего ключа
	void SetFkNum(int nf);
    ///Получить номер поля ссылки на дерево
	int GetTreeNum();
	///Установить номер поля ссылки на дерево
	void SetTreeNum(int nf);

	///Получить значение PK
	virtual wxString GetPkValue(){return wxEmptyString;};
	///Плучить значение поля для запроса
	wxString GetColumnValueForSQL(const wxArrayString& as, int nf);

	///Является ли структура данных деревом и строится ли дерево
	bool IsTree(){return (boolProperty&DB_GRID_IS_TREE)>0;};

	///Есть ли изменения для записи в БД
	bool HasUpdate(){return (boolProperty&DB_GRID_IS_CHANGED)>0;};

	///Проверка значения поля
	virtual bool CheckValue(const wxString& s, int nf);
	///Установить LOV
	virtual void SetLOV(ArraywxDbLOV& value){lov=value;};
	virtual wxDbLOV* AddLOV(wxDbLOV& value){lov.Add(value); return &(lov[lov.Count()-1]);}
	///Получить строку с текущей записью
	virtual void GetCurrentRecord(wxArrayString& as){};
	///Сбросить все данные , привести  к состоянию готовому выполнить новый запрос к БД (строка запроса не переформировывается)
	virtual void Reset(){};
	///Считывает записи из БД, либо все либо часть в зависимости от настроек
	virtual bool GetRows(){return false;}; 
	///Сделать активной первую запись Обработчики смены записи не вызываются, используется для инициализации
	virtual bool SelectFirstRecord(){return false;}
	///Сохранить данные
	virtual bool Save(){return false;};
	///Проверка, есть ли записи в гриде
	virtual bool IsRecords(){return false;}
	///Редактируется поле
	virtual bool IsEdit(){return false;}
	///Отмена редактирования поля
	virtual void StopEdit(){};
	///Заполнение грида из массива
	virtual bool FillGrid(ArrayDbRec &ar){return false;}
	///Добавить связанное поле

	///Происходит так же инициализация  поля для связи с гридом
	virtual void AddLinkField(wxDbField *f, int nf){};
	///Обновление значений связанных полей из текущей записи
	virtual void RefreshLinkField(){};
	///Проверка записи на ошибки

	///Возвращает статус и номер поля ошибки
	virtual void CheckRecord(wxArrayString& as, int *nf, bool *status);

	///
	virtual  SetFocus_(){};

	///Добавить условия в готовый запрос , надо например когда не известны поля а запрос уже есть, когда связанные гриды и надо ограничить запрос
	virtual void AddWhere(wxString &s);
	///Сбросить флаги у всех записей
	virtual void ResetFlag(){};
	///Сохранить настройки GUI в массив
	virtual bool SaveGUI(ArrayDbRec &ar,wxString &pName){return true;};
	///Загрузить настройки GUI из массива
	virtual bool LoadGUI(ArrayDbRec &ar,wxString &pName){return true;};
	///Получить индекс kолонки по названию , возвращает -1 если не найдено
	int GetFieldIndex( wxString s );
	///Установить свойство поля 
	bool SetFieldPropertyByName(wxString fn, unsigned long property);
	///Сбросить свойство поля
	bool ResetFieldPropertyByName(wxString fn, unsigned long property);
	
	///Установить название колонки по имени
	bool SetColumnNameByName(wxString fn, wxString name);

	///Установить Pk по имени
	bool SetPkNumByName(wxString fn);
	///Установить Fk по имени
	bool SetFkNumByName(wxString fn);
	///Установить TreeKey по имени
	bool SetTreeNumByName(wxString fn);

	///Установить LOV по имени
	bool SetLOVByName( wxString fn, wxDbLOV *lov );

	///Добавление связанного поля
	bool AddLinkFieldByName( wxString fn, wxDbField *f);

	///Установка ширины поля
	virtual bool SetColumnWidthByName( wxString fn, int val){return false;}
	///Установить тип значения поля по имени - int valueType;
	bool SetValueTypeByName(   wxString fn, int val);
	///Установить значение для списка по имени (для типа DB_FIELD_LIST) - wxArrayString values; 
	bool SetValuesByName(    wxString fn, wxArrayString val );
	///Установить значение для списка по имени  (для типа DB_FIELD_LIST) - 	wxArrayString values2; 
	bool SetValues2ByName(    wxString fn, wxArrayString val );
	///Установить  длину поля по имени -	int len;
	bool SetLenByName(   wxString fn, int val);
	///Установить минимальная длина поля по имени  -	int minLen;
	bool SetMinLenByName(   wxString fn, int val);
	///Установить количество знаков после запятой по имени -	int presition;
	bool SetPrecisionByName(   wxString fn, int val);
	///Установить значение по умолчанию по имени  -	wxString defValue;
	bool SetDefValueByName( wxString fn, wxString val );
	///Установить максимальное значение по имени -	wxString maxVal;
	bool SetMaxValueByName( wxString fn, wxString val );
	///Установить минимальное значение по имени -	wxString minVal;
	bool SetMinValueByName( wxString fn, wxString val );
	///Установить валидатор по имени - 	wxValidator *validator;
	bool SetValidatorByName( wxString fn, wxValidator *validator );


};
///////////////////////////


/////////////////////////////


///Массив связей


#endif