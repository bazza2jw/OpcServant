/////////////////////////////////////////////////////////////////////////////
// Name:        dbtreegrid.h
// Purpose:     Control for edit table data
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

//Грид для редактирования и просмотра данных в виде дерева
#ifndef __dbtreegrid__
#define __dbtreegrid__
#include "dbtreelistctrl.h"

#include "dbstruct.h"
#include "dbpopupmenu.h"

/**
@class wxDbTreeGrid
Класс просмотра и редактирования данных из СУБД.
Либо в виде таблицы, либо в виде дерева
*/
/// Класс просмотра и редактирования данных из СУБД


class wxDbTreeGrid : public wxDbTreeListCtrl, public wxDbQueryObject {
public:
	wxDbTreeGrid (wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTR_DEFAULT_STYLE, const wxValidator& validator = wxDefaultValidator, const wxString& name = "treelistctrl");
	~wxDbTreeGrid();
	///Создание полей грида на основе описания из wxDbQueryObject
	virtual void CreateField();
	///Вид данных для грида в виде дерева или в виде таблицы
	bool isTree;
	///Получить еще одну строку из базы и поместить в грид
	virtual bool GetNextRow();
	///Получить все строки из базы и поместить в грид
	virtual bool GetAllRow();
	///Считывает записи из БД, либо все либо часть в зависимости от настроек
	virtual bool GetRows(); 
	///Удалить текущую запись
	virtual bool DeleteCurrentRecord();
	///Вставить запись
	virtual bool InsertRecord();
	///Вставить дочернюю запись
	virtual bool InsertChildRecord();

	///Получить строку с текущей записью
	virtual void GetCurrentRecord(wxArrayString& as);
	///Получить строку из  записью
	virtual void GetCurrentRecord(wxArrayString& as,wxTreeItemId & id);
	///Назначить меню для редактирования данных
	virtual void SetMenu(wxMenu *m){menu=m;};
	///Передает все изменения в БД
	virtual bool Save();
	///Заполнение записи из массива строк
	virtual void FillRecord(wxArrayString& as,wxTreeItemId & id);
	///Установка скроллинга для отображения требуемой колонки
	virtual void ShowCol(int nf);
	///Вызов редактора для ячейки
	virtual void EditCell(const wxTreeItemId & id, int col, wxKeyEvent &event);
	///Определение положения требуемой ячейки
	virtual void GetRect(const wxTreeItemId & id, int col, int &x, int &y, int &w, int &h);
	///Обновляет грид, перечитывает записи из БД
	virtual void RefreshRecords();
	///Проверка можно ли закрыть окно , и подготавливаемся к закрытию, надо когда редактируется поле и в него введены неправильные данные
	virtual bool PrepareClose();
	///Вызвать окно ввода запроса и выполнить его
	virtual bool Query();
	///Вызвать окно ввода поиска и выполнить его
	virtual bool Search();
	///Вызвать окно ввода поиска и выполнить его
	virtual bool SearchRecord();
	///Установка полей
	virtual void Setup( wxDbQueryObject& obj, int property);
	///Сбросить все данные , привести грид к состоянию готовому выполнить новый запрос к БД (строка запроса не переформировывается)
	virtual void Reset();
	///Сделать активной первую запись в гриде
	virtual bool SelectFirstRecord();
	///Получить значение PK
	virtual wxString GetPkValue();
	///Проверка, есть ли записи в гриде
	virtual bool IsRecords();
	///Редактируется поле
	virtual bool IsEdit()
		{if(GetMainWindow()->m_editControl)return true; else return false;}
	///Отмена редактирования поля
	virtual void StopEdit();

	///Заполнение грида из массива
	virtual bool FillGrid(ArrayDbRec &ar);

	///Проверка активен ли пункт меню
	virtual bool MenuItemIsActive(int id);
	///Добавить связанное поле
	virtual void AddLinkField(wxDbField *f, int nf);
	///Обновление значений связанных полей из текущей записи
	virtual void RefreshLinkField();

	///Сохранить настройки GUI в массив
	virtual bool SaveGUI(ArrayDbRec &ar,wxString &pName);
	///Загрузить настройки GUI из массива
	virtual bool LoadGUI(ArrayDbRec &ar,wxString &pName);

	///Вызвать обработчик заполнив предварительно контекст события
	virtual void CallHandler(wxEvtHandler* handler, int tp);
	
	///Текущая обрабатываемая строка
	wxTreeItemId curId;
	///Пустая строка для определения что курсор достиг последней записи и надо дочитать данные
	wxTreeItemId EmptyId;
	///Установка мненю (разрешение , запрещение пунктов меню)
	void SetMenuItems();
	///Пересчет количества записей в гриде

	///Вызывается после удаления  вставки  и перемещения записи
	void RowRecalc();
	///Получить строку с тукущей позицией
	wxString GetCurPos();
	///Получить текущую запись
	wxTreeItemId GetCurrentItem(){ return eId;}
	///Установка значения поля свойств записи
	void SetFlagsValue(wxTreeItemId & id,int v);
	//Установка ширины поля по имени
	virtual bool SetColumnWidthByName( wxString fn, int val);


protected:
	///Обработчик показать свойства дерева
	void OnShowProperty(wxCommandEvent & event);
	//Замена непечатных символов на обозначения для HTML
	void ProcessStr( wxString &s );

	///Обработчик удаления записи
	void OnDeleteRecord(wxCommandEvent & event);
	///Обработчик вставки записи
	void OnInsertRecord(wxCommandEvent & event);
	///Обработчик сохранения
	void OnSaveRecord(wxCommandEvent & event);
	/// Обработчик редактирования записи
	void OnEditRecord(wxCommandEvent & event);
	/// Обработчик выбора записи
	void OnSelectItem(wxTreeEvent &event);
	/// Обработчик запроса
	void OnQueryItem(wxTreeEvent &event);
	///Обработчик вставки дочерней записи(для дерева)
	void OnInsertChildRecord(wxCommandEvent & event);
	///Обработчик дублирования записи
	void OnDuplicateRecord(wxCommandEvent & event);
	///Обработчик изменеия значения связанного поля
	void OnwxDbFieldChanged(wxDbFieldEvent &event);
	///Обработчик события от связанных полей (SetFocus ,LOV..)
	void OnwxDbFieldCmnd(wxDbFieldEvent &event);
	///Обработчик начала перетаскивания
	void OnBeginDrag(wxTreeEvent &event);	
	///Обработчик конца перетаскивания
	void OnEndDrag(wxTreeEvent &event);	
	///Обработчик клавиш
	void OnKey(wxKeyEvent &event);
	/// Обработчик вызова внешнего редактора
	void OnExternEditor(wxCommandEvent & event);

	void OnKeyDown(wxKeyEvent &event);
	///Обработчик окончания редактирования
	void OnEndEdit(wxTreeEvent &event);
	///Создает объект для редактирования даты
	void EditDate(const wxTreeItemId & id, int col);
	///Создает объект для редактирования поля списка
	void EditList(const wxTreeItemId & id, int col);
	///Редактирование двоичного поля
	void EditBool(const wxTreeItemId & id, int col);
	///Создает объект для редактирования текстового или числового поля
	void EditText(const wxTreeItemId & id, int col,wxKeyEvent &event);
	///Создает объект для выбора значения из списка (данные получаем из БД)
	void EditChoice(const wxTreeItemId & id, int col);
	///Создает объект для выбора значения из списка настроенного пользоателем
	void EditCustomChoice(const wxTreeItemId & id, int col);

	///Обрабтчик обновления грида перечитывает записи из БД
	void OnRefreshRecord(wxCommandEvent & event);
	///Обрвботчик ввода запроса
	void OnQueryRecord(wxCommandEvent & event);
	/// Обработчик поиска
	void OnSearchRecord(wxCommandEvent &event);
	/// Обработчик поиска следующей записи
	void OnSearchRecordNext(wxCommandEvent &event);

	///Проверка на отображение не считанных записей и досчитывание из БД
	void FetchNext();

	void OnTextEnter(wxCommandEvent &event);

	///Обработчик события по раскрытию узла
	void OnExpanded(  wxTreeEvent &event );


	///Обработчик нажатия правой кнопки мыши
	void OnMouseRightClick(wxMouseEvent& event);

	///Обработчик нажатия двойной кнопки мыши
	void OnMouseDoubleClick(wxMouseEvent& event);


	///Получить из базы все записи для дерева
	bool GetAllRowTree(wxTreeItemId& parent);
	///Получить значение Tree для элемента дерева
	wxString GetItemTreeValue(wxTreeItemId& id);


	virtual  SetFocus_(){GetMainWindow()->SetFocus(); };
	virtual wxEvtHandler* GetEvtHandler(){return this;} 
	///Переместить узел дерева
	bool MoveTreeNode(wxTreeItemId &src,wxTreeItemId &dst);

	///Переместить потомков узла дерева (рекурсия)
	bool Reparent(wxTreeItemId &src,wxTreeItemId &dst);

	///Скопировать данные их одной строки в другую
	void CopyRow(wxTreeItemId &src,wxTreeItemId &dst);
	///Сбросить флаги у всех записей
	virtual void ResetFlag();
	///Считать текущий уровень дерева из БД
	bool GetTreeLevel(wxTreeItemId &id);
	///флаг не проверять запись на ошибки
	bool noCheck;
	///Флаг не редактировать при выборе записи
	bool noEdit;
	///Перемещаемый узел
	wxTreeItemId dragItem;
	void OnSetFocus(wxFocusEvent& event);
	///Буфер для текущей записи
	wxArrayString buf;
	
public:
	DECLARE_EVENT_TABLE()

	
};


#endif