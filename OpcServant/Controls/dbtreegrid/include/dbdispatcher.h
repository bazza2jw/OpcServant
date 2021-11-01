/////////////////////////////////////////////////////////////////////////////
// Name:        dbdispatcher.h
// Purpose:     Description of the class synchronization
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

//Класс для связи и управления несколькими гридами 
//гриды входят в единую форму и работают в пространстве единой транзакции
#ifndef __dbdispatcher__
#define __dbdispatcher__
#include <wx/wx.h>
#include <wx/grid.h>
#include "dbstruct.h"
//

class wxDbDispatcherEvent;
class wxDbGridEvent;

/**
@class wxDbGridLink
Определяет главный и подчиненный грид
*/
///Определяет главный и подчиненный грид
class wxDbGridLink 
{
public:
	wxDbGridLink(){parent=0; child=0;};
	~wxDbGridLink(){};
	///Главный(Родительский) грид
	wxDbQueryObject* parent;
	///Подчиненный грид
	wxDbQueryObject* child;
};
#include <wx/dynarray.h>

WX_DECLARE_OBJARRAY( wxDbGridLink, ArraywxDbGridLink );




/**
@class wxDbDispatcher
Класс для синхронизации работы нескольких связанных гридов в форме
*/
///Класс для синхронизации работы нескольких связанных гридов в форме

class  wxDbDispatcher : public wxEvtHandler
{

public:
	wxDbDispatcher(){activeGrid=0;win=0;};
	~wxDbDispatcher();
	///Обработчик событий от гридов, цепляется ко всем связанным гридам на требуемые виды событий
	void Handler_(wxDbRecEvent &event);
	ArraywxDbGridLink links;
	///Добавить связь между гридами
	void AddLink(wxDbQueryObject* p,wxDbQueryObject* c);
	///Обработчик ухода с текущей записи

	///Проверяется есть ли подчиненные гриды, если есть и в них есть изменения, то выдается предупреждение
	bool OnPostRecord(wxDbRecEvent& event);

	///Обработчик перехода на новую запись

	///Если у грида есть связанные, подчиненные гриды то производиться обновление подчиненных гридов
	bool OnPreRecord(wxDbRecEvent& event);


	///Обработчик записи

	///Производит поочередную запись связанных гридов начиная от главного к подчиненному
	bool OnSave(wxDbRecEvent& event);

	///Получить упорядоченный по дереву массив со связями
	void GetLinkTree(ArraywxDbGridLink &al,wxDbQueryObject * root);

	///Обработчик удаление записи пользователем

	///Проверяет есть ли в подчиненных гридах записи
	bool OnDelete(wxDbRecEvent& event);

	///Добавить грид

	///Добавляет связь между гридами и устанавливает стандартные обработчики у грида
	///Надо чтобы поле parent было установлено !!!
	void AddGrid(wxDbQueryObject *grid);

	///Есть ли изменения
	bool IsChanged();
	///Редактируется поле
	bool IsEdit();
	///Отменяет редактирования в гридах
	void DelEditField()	;
	///Очищает обработчики в гридах
	void Clear();
	///Получить текущую строку в гриде
	wxString GetCurPos();

	///Активный грид
	wxDbQueryObject *activeGrid;

	///Обработчик активации грида

	///Запоминает активный грид и передает событие в главное окно для установки меню
	void OnGridActive(wxDbGridEvent &event);


	///Обработчик основного тулбара пересылает события активному гриду 
	void OnToolBar(wxCommandEvent &event);

	///Получить окно формы
	wxEvtHandler * GetMainWindow();
	
	///Главное окно формы
	wxWindow *win;
protected:
	DECLARE_EVENT_TABLE()




};



/**
@class wxDbGridEvent
Класс события для диспетчера
*/
///Класс события для диспетчера

class wxDbDispatcherEvent : public wxNotifyEvent{
public:
   wxDbDispatcherEvent():wxNotifyEvent(){dispatcher=0;};
   wxDbDispatcherEvent(const wxDbDispatcherEvent& event)
	   : wxNotifyEvent(event){
	   dispatcher=event.dispatcher;
   };
   wxDbDispatcherEvent (wxEventType commandEventType, int id=0);
   virtual wxEvent *Clone() const { return new wxDbDispatcherEvent(*this); }
public:
	///Диспетчер
	wxDbDispatcher *dispatcher;
};


BEGIN_DECLARE_EVENT_TYPES()

	DECLARE_EVENT_TYPE(dbEVT_DISPATCHER_ACTIVE_HANDLE, 7102)

END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*dbDispatcharEventFunction)(wxDbDispatcherEvent&);


#define dbDispatcherEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(dbDispatcharEventFunction, &func)

///Обработчик событий связанных с активацией фомы(диалога) содержащей диспетчер
#define EVT_DISPATCHER_ACTIVE_HANDLE( winid, func) \
    wx__DECLARE_EVT1(dbEVT_DISPATCHER_ACTIVE_HANDLE, winid, dbDispatcherEventHandler(func))


#endif //__dbdispatcher__