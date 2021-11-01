/////////////////////////////////////////////////////////////////////////////
// Name:        dbtreegrid.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

//Грид для редактирования и просмотра данных в виде дерева
#include "../include/dbtreegrid.h"
#include "dbtreelistctrl.cpp"
#include "../include/dbquerydialog.h"
#include "../include/dblovdialog.h"
#include "../include/dbdispatcher.h"
#include <wx/file.h>
#include <wx/stdpaths.h>
#include "../include/xpm/xpm.h"
#include "../include/dbedittextdialog.h"
#include "../include/langdef.h"
	wxDbTreeGrid::wxDbTreeGrid (wxWindow* parent, wxWindowID id,
			const wxPoint& pos , const wxSize& size , 
			long style , const wxValidator& validator , 
			const wxString& name ):
			wxDbTreeListCtrl( parent,  id,  pos ,  size ,	 style ,  validator ,  name)
	{
		isTree=false;
		menu=&dbmenu;
		eId=0;
		db=0;
		eCol=0;
		noCheck=false;
		noEdit=false;
//Создаем список иконок
		wxImageList* f_imageList;
		f_imageList = new wxImageList( 16, 16, true );
		f_imageList->Add( wxIcon( check_xpm ) ); // 0
		f_imageList->Add( wxIcon( uncheck_xpm ) ); // 1
		SetImageList( f_imageList );
	//	wxTreeItemId root = AddRoot( wxEmptyString);
	};

	wxDbTreeGrid::~wxDbTreeGrid()
	{
//			if(GetMainWindow()->m_editControl)
//	{
//			GetMainWindow()->m_editControl->Destroy();
		
//	}
	};


	//Создание полей грида на основе описания из wxDbQueryObject
	void wxDbTreeGrid::CreateField()
	{
		//Два варианта предполагается в виде дерева и простая таблица
		//Если есть в описание полей поле дерева то строим дерево если нет то в виде простой таблицы


		/*
		if(GetColumnCount()>0){
			wxMessageBox(TEXT_IS_REPEAT_SET_FIELD);
			return;
		};
		*/
		//Здесь проверяем поле дерева isTree
		//?????
		//Cоздаем колонки
		//Первое поле всегда в колонке дерева и всегда видимо !!!!
		
		for(int i=0; i<(int)fieldsDescription.Count(); i++){

			AddColumn(fieldsDescription[i].columnName);
			if(fieldsDescription[i].valueType==DB_FIELD_INT ||  fieldsDescription[i].valueType==DB_FIELD_FLOAT)
			{
				SetColumnAlignment(i,wxALIGN_RIGHT);
			};
			if(fieldsDescription[i].valueType==DB_FIELD_BOOL )
			{
				SetColumnAlignment(i,wxALIGN_CENTER);
			};
			if(!fieldsDescription[i].GetVisible()){
				SetColumnWidth(i,0);
			};
		};

		AddRoot( wxEmptyString);
	
		ExpandAll (GetRootItem());
		//Настраиваем меню может надо в другом месте но пока тут!!!
		SetMenuItems();
		
	};


	//Получить еще одну строку из базы и поместить в грид
	bool wxDbTreeGrid::GetNextRow()
	{
		wxArrayString sa;
		if(!GetNextRecord(sa)) return false;
		wxTreeItemId id=AppendItem (GetRootItem(), sa[0]);
		/*if(!eId.IsOk())
		{
			eId=id;
			SelectItem(eId);
			eCol=GetColumnCount()-1;
			for(int j=0; j<GetColumnCount(); j++)
			{
				if(fieldsDescription[j].GetProperty()&DB_FIELD_VISIBLE)
				{
					eCol=j; break;
				};
			};
		};
		*/
		for(int i=0; i<(int)sa.Count(); i++){
			if( fieldsDescription[i].valueType == DB_FIELD_BOOL )
			{
				
			
				if( sa[i] == wxEmptyString || sa[i] == wxT("0") )
				{
				
					SetItemImage( id, i, 1);
				}
				else
				{
				
					SetItemImage( id, i, 0);
				}
			}
			else
			{
				SetItemText( id,i,sa[i]);
			}
		}
		if(postQueryHandler)
		{
			wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
			ev.tp=DB_POST_QUERY_HANDLE;
			ev.SetEventObject( (wxWindow*)this );
			ev.objType=DB_OBJ_TYPE_TREE_GRID;
			ev.as=&sa;
			ev.obj=this;
			curId=id;
			postQueryHandler->ProcessEvent(ev);


		};

		return true;
	};
	//Получить все строки из базы и поместить в грид
	bool wxDbTreeGrid::GetAllRow(){
//		wxLogMessage("wxDbTreeGrid::GetAllRow "+query);
		bool flag = false;
		wxTreeItemId id1;
		
		//Если дерево то отдельно обрабатываем
		if(boolProperty&DB_GRID_IS_TREE && boolProperty&DB_GRID_READ_ONE)
		{
			
			wxTreeItemId id=GetRootItem();
			wxBeginBusyCursor();
			bool res= GetAllRowTree(id);
			wxEndBusyCursor();
			return res;
		};
		
		if(boolProperty&DB_GRID_IS_TREE && !(boolProperty&DB_GRID_READ_ONE))
		{
			wxTreeItemId id=eId;
			wxBeginBusyCursor();
			bool res= GetTreeLevel(id);
			wxEndBusyCursor();
			return res;
		}
		//Если есть пустая запись то удаляем
		if (EmptyId.IsOk()) 
		{
			wxTreeItemId id;
			
			if(EmptyId == eId){ //Если текущая запись была пустая(фиктивная) то устанавливаем флаг
				eId = id;//!!!
				flag = true;
				id1 = GetPrev( EmptyId );
			}
			Delete( EmptyId);
			
			EmptyId=id;	
			

		}
	
		bool fl=false;
		int cnt=NEXT_NUMBER_ROWS;

		while( (boolProperty&DB_GRID_READ_ONE || cnt>0) &&  GetNextRow()  ){
			fl=true;
			cnt--;
		}

	
		//Если не все записи прочитаны, то добавляем пустую
		if ( !(boolProperty&DB_GRID_READ_ONE || cnt>0) )
		{
			EmptyId=AppendItem (GetRootItem(), wxEmptyString);

		}
		//Если текущая запись была пустая(фиктивная) то выбераем первую прочитанную запись
		if( flag )
		{
				id1 = GetNext( id1 );
				if( id1.IsOk() ) SelectItem( id1 );
		}
		return fl;
	};

	//Обработчик удаления записи
	void wxDbTreeGrid::OnDeleteRecord(wxCommandEvent & event){
		if(!MenuItemIsActive(ID_DELETE)) return;
//		long ll;
		if(GetChildrenCount(eId))
		{
			wxMessageBox(TEXT_IS_RECORD_HAS_SABORDINATE,wxEmptyString,wxOK) ;
			return;
		};
		if(deleteHandler)
		{

			wxArrayString as;
			GetCurrentRecord(as,eId);
			wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
			ev.tp=DB_DELETE_HANDLE;
			ev.SetEventObject( (wxWindow*)this );
			ev.objType=DB_OBJ_TYPE_TREE_GRID;
			ev.as=&as;
			ev.obj=this;
			curId=eId;
			deleteHandler->ProcessEvent(ev);
			if(ev.veto) return;

		};
		DeleteCurrentRecord();
		
	}
	//Обработчик вставки записи
	void wxDbTreeGrid::OnInsertRecord(wxCommandEvent & event){
		if(!MenuItemIsActive(ID_INSERT)) return;
		InsertRecord();
	}

	bool wxDbTreeGrid::DeleteCurrentRecord(){
	//	
		wxTreeItemId id = GetSelection();
		if(!id.IsOk()){wxMessageBox(TEXT_IS_REQURE_CHOICE); return false;}
		wxArrayString as;
		GetCurrentRecord(as);
	
		if(!as.Count()) return false;
		wxString s=as[as.Count()-1];
		long st;
		s.ToLong(&st);
		st=st| DB_DELETED_RECORD;
		s.sprintf("%d",st);
		as[as.Count()-1]=s;
		//Запоминаем текущую запись

		curId=id;
		
		//if(ProcessRecord(as,this)){
			//Помечаем грид как измененный
			if( st&DB_QUERY_RECORD )
			{
				boolProperty |=DB_GRID_IS_CHANGED;
			}
		

			noCheck=true;
			recArray.Add(as);
			wxTreeItemId id1;
			if(id.IsOk())id1=GetNext(id);
			if(!id1.IsOk() && id.IsOk()) id1=GetPrev(id);
			if(id1.IsOk()  ) SelectItem(id1);
			eId=id1;
			Delete(id); 
			//Пересчет к-ва записей
			RowRecalc();
			noCheck=false;

			return true; 
		//};
		//return false;
 
	} 

	//Вставить запись
	bool wxDbTreeGrid::InsertRecord(){
	
		wxTreeItemId id = GetSelection();
		wxTreeItemId i1;
		wxTreeItemId parent;
		wxArrayString as;

		if( id==GetRootItem() ) UnselectAll();
		if( eId==GetRootItem() ) eId = i1;
		

		int col = -1;

		if(eId.IsOk() && eId!= id) id = eId;
		

		if( id.IsOk() && id != GetRootItem() )
		{
			int nf;
			bool st;
			GetCurrentRecord(as,id);
			CheckRecord(as,&nf,&st);
			if(!st) return false;
		}

		if(!id.IsOk())i1=AppendItem (GetRootItem(), wxT(""));
		else 
		{
			parent=GetItemParent(id);
			if(!parent.IsOk()) parent = GetRootItem();
			i1=InsertItem (parent, id, wxT(""));
			EnsureVisible(i1); 

		};
	

	//	if(!id.IsOk()){wxMessageBox(wxT("wxDbTreeGrid::InsertRecord- Надо выбрать запись")); return false;}
	
		for(int i=0; i<(int)GetColumnCount() && i<(int)fieldsDescription.Count(); i++)
		{
			SetItemText(i1,i,fieldsDescription[i].defValue);
			if( col == -1 && fieldsDescription[i].GetProperty()& DB_FIELD_VISIBLE)
			{
				col = i;
			}
		};
		if( col >=0 )
		{ 
		
			eCol=col;
			ShowCol(eCol);
			GetMainWindow()->m_curColumn=eCol; 
			GetMainWindow()->RefreshSelected(); 
		};

		if(boolProperty&DB_GRID_IS_TREE && parent.IsOk())
		{
			wxString tId=GetItemText(parent,GetPkNum());
			SetItemText(i1,GetTreeNum(),tId);
		}
		SetFlagsValue(i1,DB_INSERTED_RECORD);
		//Пересчет к-ва записей
		RowRecalc();
		SelectItem(i1);
		eId=i1;
		
		return true;
	}

	//Получить строку с текущей записью
	void wxDbTreeGrid::GetCurrentRecord(wxArrayString& as){

	
		wxTreeItemId id = GetSelection();
		if(!id.IsOk()){wxMessageBox(TEXT_IS_REQURE_CHOICE_CUR_REC); return;}
		GetCurrentRecord( as, id);

	}

	void wxDbTreeGrid::GetCurrentRecord(wxArrayString& as,wxTreeItemId & id){

		if(!id.IsOk()){wxMessageBox(TEXT_IS_REQURE_CHOICE_CUR_REC); return;}
		as.Clear();

		for (int i=0; i<GetColumnCount(); i++){
			as.Add(GetItemText (id, i));
		};
	
	}

	//Обработчик нажатия правой кнопки мыши
	void wxDbTreeGrid::OnMouseRightClick(wxMouseEvent& event){
		if(menu==0) return;

		SetMenuItems();
		PopupMenu(menu, event.GetX(), event.GetY());

	};
	//Передает все изменения в БД
	bool wxDbTreeGrid::Save(){
		wxString s;
		wxArrayString as;

		s.sprintf("%d",DB_QUERY_RECORD);
		//Проверка текущей записи
		if(eId.IsOk() && eId != GetRootItem() )
		{
		
			int nf;
			bool st;
			GetCurrentRecord(as,eId);
			CheckRecord(as,&nf,&st);
			if(!st) return false;
		};
		wxBeginBusyCursor();
		//Перебераем все записи и обрабатываем те у которых есть изменения
		wxTreeItemId id=GetRootItem();
		id=GetNext(id);
		while(id.IsOk()){
			GetCurrentRecord(as,id);
			curId=id;
			if(ProcessRecord(as,this)){
				//Копируем все строки назад в грид, возможно внутри обработчиков запись изменилась
				FillRecord(as,id);
			}
			else {
				wxMessageBox(TEXT_IS_SAVE_ERROR);
			//	SelectItem(id);
			//	ScrollTo(eId);
				wxEndBusyCursor();
				return false;
			}

			id=GetNext(id);
		};

		for(int i=0; i<(int)recArray.Count(); i++)
		{
				if(ProcessRecord(recArray[i],this)){
			}
			else {
				wxMessageBox(TEXT_IS_DELETE_ERROR);
			//	SelectItem(id);
			//	ScrollTo(eId);
				wxEndBusyCursor();
				return false;
			}
		};
		//if(!saveHandler) ResetFlag();
		ResetFlag();
		wxEndBusyCursor();
		return true;
	}

	///Сбросить флаги у всех записей
	void wxDbTreeGrid::ResetFlag()
	{
		wxTreeItemId id;
		wxString s,ss,s1;
		long v;
		s.sprintf("%d",DB_QUERY_RECORD);
		ss.sprintf("%d",DB_QUERY_RECORD|DB_TREE_QUERY);
		recArray.Clear();
		//if(db) db->Commit(); 
		//Сбрасываем все флаги изменений
		id=GetRootItem();
		id=GetNext(id);
		while(id.IsOk()){
			curId=id;
			s1 = GetItemText( id,GetColumnCount()-1);
			s1.ToLong( &v );
			if( v&DB_TREE_QUERY )
			{
					SetItemText( id,GetColumnCount()-1,ss);
			}
			else
			{
					SetItemText( id,GetColumnCount()-1,s);
			}
			id=GetNext(id);
		};
		//Помечаем грид что нету изменений для БД
		boolProperty &=~DB_GRID_IS_CHANGED;
	};

	///Проверка активен ли пункт меню
	bool wxDbTreeGrid::MenuItemIsActive(int id)
	{
		if(!menu) return false;
		wxMenuItemList il=menu->GetMenuItems() ;
		 wxMenuItemList::compatibility_iterator node = il.GetFirst();
		while (node)
		{
			wxMenuItem *it = node->GetData();
			if(it->GetId()== id)
			{

				if(it->IsEnabled()){	 return true;}
				else return false;

			}
       
			node = node->GetNext();
		}



		return false;
	};


	//Установка мненю (разрешение , запрещение пунктов меню)
	void wxDbTreeGrid::SetMenuItems(){
		wxMenuItemList il=menu->GetMenuItems() ;
		 wxMenuItemList::compatibility_iterator node = il.GetFirst();
		while (node)
		{
			wxMenuItem *it = node->GetData();

			switch(it->GetId()){
					case ID_EDIT: 
						if(boolProperty&DB_GRID_UPDATE_ENABLE) it->Enable(true); else it->Enable(false);
						break;
					case ID_DELETE:  
						if(boolProperty&DB_GRID_DELET_ENABLE) it->Enable(true); else it->Enable(false);
						break;
					case ID_INSERT: 
					
						if(boolProperty&DB_GRID_INSERT_ENABLE) it->Enable(true); else it->Enable(false);		
						break;
					case ID_SAVE: 
						if(boolProperty&DB_GRID_UPDATE_ENABLE || boolProperty&DB_GRID_DELET_ENABLE || boolProperty&DB_GRID_INSERT_ENABLE) it->Enable(true); else it->Enable(false);						
						break;
					case ID_INSERT_CHILD: 
						if(boolProperty&DB_GRID_INSERT_ENABLE && boolProperty&DB_GRID_IS_TREE) it->Enable(true); else it->Enable(false);		
						break;
					case ID_QUERY: 
						if(boolProperty&DB_GRID_QUERY_ENABLE ) it->Enable(true); else it->Enable(false);						
						break;
					case ID_REFRESH: 
						if(boolProperty&DB_GRID_REFRESH_ENABLE ) it->Enable(true); else it->Enable(false);						
					case ID_DUBLICATE_RECORD: 
						if(boolProperty&DB_GRID_DUBLICATE_ENABLE ) it->Enable(true); else it->Enable(false);						

						break;
						
				

			}
       
        
			node = node->GetNext();
		}


	
	};
//Обработчик сохранения
	void wxDbTreeGrid::OnSaveRecord(wxCommandEvent & event){

		if(!MenuItemIsActive(ID_SAVE)) return;

		if( saveHandler )
		{
			wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
			ev.tp=DB_SAVE_HANDLE;
			ev.SetEventObject( (wxWindow*)this );
			ev.objType=DB_OBJ_TYPE_TREE_GRID;
			ev.obj=this;
			saveHandler->ProcessEvent(ev);
		}
		else
		{	
			db->BeginTransaction();
			if(Save()) {if(db && db->IsOpen()) db->Commit(); }
			{if(db && db->IsOpen()) db->RollBack(); }
		}

	};

//Установка значения поля свойств записи
	void wxDbTreeGrid::SetFlagsValue(wxTreeItemId & id,int v){
		long st;
		wxString s;
		s=GetItemText( id,GetColumnCount()-1);
		s.ToLong(&st);
		st=st|v;
		//Помечаем грид как измененный
		if( st&DB_INSERTED_RECORD || st&DB_UPDATED_RECORD ){

			boolProperty |=DB_GRID_IS_CHANGED;
		}
		s.sprintf("%d",st);

		SetItemText( id,GetColumnCount()-1,s);
	};


//Заполнение записи из массива строк
	void wxDbTreeGrid::FillRecord(wxArrayString& as,wxTreeItemId & id){
	
		for(int i=0; i<(int)as.Count(); i++){
			SetItemText( id,i,as[i]);
		}
	};

// Обработчик редактирования записи
	void wxDbTreeGrid::OnEditRecord(wxCommandEvent & event){
	}

	// Обработчик выбора записи
	void wxDbTreeGrid::OnSelectItem(wxTreeEvent &event){
		wxString s;
		long v;
		wxTreeItemId id=event.GetItem();
		///Если выбрали пустую(фиктивную) запись то пока не обрабатываем(обработка в GetAllRow)
		//if( EmptyId.IsOk() &&  id == EmptyId ) 
		//{
		//
		
		if(!eId.IsOk()){

		}
		if(id.IsOk() && eId!=event.GetItem() ){
			//Сдесь устанавливаем номер колонки для редактирования
			eCol=event.GetInt();
			s.sprintf("%d",eCol);


		}
		if( id.IsOk() )
		{
			s= GetItemText( id, GetColumnCount()-1 );
			s.ToLong( &v );
		}
		if(!noEdit && id.IsOk() && event.GetItem()==eId && eCol==event.GetInt() && (GetProperty(DB_GRID_UPDATE_ENABLE) || v&DB_INSERTED_RECORD) )
		{
			wxKeyEvent event;
			event.m_keyCode=0;
			EditCell(eId,eCol,event);
		}

		if(id.IsOk() ){ 
			this->GetMainWindow()->m_curItem = (wxDbTreeListItem*) id.m_pItem;//!!!!


			wxArrayString as;
			//Проверяем запись
			
			if(!noCheck && eId.IsOk() && eId!=event.GetItem())
			{
					bool st;
					int nf;
				
					GetCurrentRecord(as,eId);
					CheckRecord(as,&nf,&st);
					if(!st)
					{
						eCol=GetColumnCount()-1;
						//if(nf<GetColumnCount()&& GetColumnWidth(nf)  ){eCol=nf; }
						SelectItem(eId);
						ScrollTo(eId);
						if(nf<GetColumnCount()&& GetColumnWidth(nf)  ) ShowCol(nf);
						s.sprintf("wxDbTreeGrid::OnEditRecord nf %d",nf);

						return;
					}
			};
			
			//Вызов обработчика после ухода с записи
			if(eId.IsOk() && eId!=id && postRecordHandler)
			{
				GetCurrentRecord(as,eId);
				wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
				ev.tp=DB_POST_RECORD_HANDLE;
				ev.SetEventObject( (wxWindow*)this );
				ev.objType=DB_OBJ_TYPE_TREE_GRID;
				ev.as=&as;
				ev.obj=(wxDbQueryObject*) this;
				curId=eId;

				postRecordHandler->ProcessEvent(ev);
				//Если ошибка то переходим назад
				if(ev.veto){
					eCol=GetColumnCount()-1;
					SelectItem(eId);
					ScrollTo(eId);
					return;
				}

			};
			//Вызов обработчика когда попали на новую запись
			if(eId!=id && preRecordHandler)
			{

			
				eId=id;
				GetCurrentRecord(as,eId);
				wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
				ev.tp=DB_PRE_RECORD_HANDLE;
				ev.SetEventObject( (wxWindow*)this );
				ev.objType=DB_OBJ_TYPE_TREE_GRID;
				ev.as=&as;
				ev.obj=(wxDbQueryObject*) this;
				curId=id;

				preRecordHandler->ProcessEvent(ev);
				RefreshLinkField();

			};
			eCol=event.GetInt();
			eId=event.GetItem();
				
		};
	

	
		

	};

	//Обработчик клавиш
	void wxDbTreeGrid::OnKey(wxKeyEvent &event){
		wxTreeItemId id;
		int d=1;
		wxUint32 scode;
		scode=event.GetKeyCode();

		wxString s;
		s.sprintf("%d %d",scode,event.GetModifiers());

		wxCommandEvent ev;
/*		if(!eId.IsOk())
		{
			SelectFirstRecord();
			
			if(!eId.IsOk()) return;
			SelectItem(eId);
		};
		*/

		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==5)//Внешний редактор
		{

				OnExternEditor(ev);
				return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==342)//Поиск
		{

				OnSearchRecord(ev);
				return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL| wxMOD_ALT     && event.GetKeyCode()==342)//Поиск следующей записи
		{

				OnSearchRecordNext(ev);
				return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==17 && (boolProperty&DB_GRID_QUERY_ENABLE))//Запрос
		{

				OnQueryRecord(ev);
				return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==322 && (boolProperty&DB_GRID_DELET_ENABLE))//Добавить запись
		{

				OnInsertRecord(ev);
				return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==127 && (boolProperty&DB_GRID_DELET_ENABLE))//Удалить запись
		{

				OnDeleteRecord(ev);
				return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL| wxMOD_ALT     && event.GetKeyCode()==322 && (boolProperty&DB_GRID_INSERT_ENABLE))//Добавить запись потомка
		{

				OnInsertChildRecord(ev);
				return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==19)//Сохранить
		{

				OnSaveRecord(ev);
				return;
		}
		if(event.GetModifiers()==wxMOD_CONTROL   && event.GetKeyCode()==18 && (boolProperty&DB_GRID_REFRESH_ENABLE))//Обновить
		{

				OnRefreshRecord(ev);
				return;
		}

		if(event.GetModifiers()==wxMOD_CONTROL| wxMOD_ALT     && event.GetKeyCode()==343 && (boolProperty&DB_GRID_DUBLICATE_ENABLE))//Дублировать запись Ctrl+F4
		{

				OnDuplicateRecord(ev);
				return;
		}

		if(event.GetKeyCode() ==WXK_LEFT || (event.GetKeyCode() ==WXK_TAB && event.GetModifiers()== wxMOD_SHIFT ) ){

			id=eId;
			int tCol=eCol;
			while( eCol-d>=0 ){ if(GetColumnWidth(eCol-d)) break;   d=d+1;}
			if (eCol-d>=0){eCol=eCol-d;ShowCol(eCol);}
			if(tCol==eCol && event.GetKeyCode() ==WXK_TAB)
			{

				if(parent) {parent->SetFocus_();}
				else
				{
					wxWindow *w= ((wxWindow*)this)->GetPrevSibling();
					wxWindow *w1;
					if(w) w->SetFocus();
					if(!w)
					{
						w1=((wxWindow*)this)->GetNextSibling();
						if ( w1 ) w=w1->GetNextSibling();
						if(w)w->SetFocus();
					};
				};
			
			};
			
		//	GetMainWindow()->m_curColumn=eCol;
			return;
		}
		if(event.GetKeyCode() ==WXK_RIGHT || event.GetKeyCode() ==WXK_TAB){

			id=eId;
			int tCol=eCol;
			while( eCol+d<GetColumnCount() ){if(GetColumnWidth(eCol+d)) break; d=d+1;}
			if (d+eCol<GetColumnCount()){eCol=eCol+d;ShowCol(eCol);}
			if(tCol==eCol && event.GetKeyCode() ==WXK_TAB)
			{

			
				wxWindow *w1,*w;
				w = 0;
				w1 = ((wxWindow*)this)->GetNextSibling();
				if ( w1 ) w = w1->GetNextSibling();
				
				s.sprintf("%d %d %d",w,w1,GetHeaderWindow());

			
				if(w) w->SetFocus();
				if(!w)
				{
					w=((wxWindow*)this)->GetPrevSibling();
					s.sprintf("%d %d %d",w,w1,GetHeaderWindow());

			
					
					if(w)w->SetFocus();
				};
			
			};
			//if(d==0) event.Skip();
			return;


		}
	
		
		if( event.GetKeyCode() == WXK_DELETE  && eCol>=0 && !(fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV) && !fieldsDescription[eCol].lov  )
		{
			if( GetProperty(DB_GRID_UPDATE_ENABLE) && eCol>=0 && eId.IsOk())
			{
				SetItemText(eId,eCol,wxEmptyString);
				if(fieldsDescription[eCol].valueType==DB_FIELD_LIST)
				{
						SetItemText(eId,eCol+1,wxEmptyString);
				}
				SetFlagsValue(eId,DB_UPDATED_RECORD);
			}
			return;
		}
		//Обрабатываем выбор для LOVа
		if (scode==13 && GetParent()) GetParent()->GetEventHandler()->ProcessEvent(event);
/*
		if((scode==32 || scode==13 || scode==341 || (scode >=0 && scode<=255)) && GetProperty(DB_GRID_UPDATE_ENABLE) && scode!=9){
			if(eId.IsOk() && eCol>=0) EditCell(eId,eCol,event);
		
			return;
		}
		*/
		if(eId.IsOk() && eCol>=0) event.Skip();
	
	};

	/// Обработчик вызова внешнего редактора
	void wxDbTreeGrid::OnExternEditor(wxCommandEvent & event)
	{

		if( eCol >= (int)fieldsDescription.Count() ) return;
		if( fieldsDescription[eCol].valueType==DB_FIELD_STRING && ( !fieldsDescription[eCol].lov ||  (fieldsDescription[eCol].GetProperty() & DB_FIELD_LOV_AND_EDIT) ) )
		{
			DbEditTextDialog d(this);
			if( eId.IsOk() )
			{
				d.editor->SetValue( GetItemText(eId,eCol) );
				GetCurrentRecord(buf);
				if( d.ShowModal() == wxID_OK )
				{
					if( d.editor->GetValue() != GetItemText(eId,eCol) )
					{
						SetItemText( eId, eCol, d.editor->GetValue() );
						wxTreeEvent event;
						event.SetItem(eId);
						OnEndEdit(event);
					};
				};
			}

		}
	};
		


    //Вызов редактора для ячейки
	void wxDbTreeGrid::EditCell(const wxTreeItemId & id, int col, wxKeyEvent &event){
			//Запоминаем текущую запись
 			GetCurrentRecord(buf);
			wxUint32 scode;
			scode=event.GetKeyCode();
			bool localEdit = false;


	


		if( !(fieldsDescription[eCol].GetProperty() & DB_FIELD_EDITABLE) && !fieldsDescription[eCol].lov && !(fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV)) return; 
			
		while(1==1)
		{
			//Поле с двойным редактированием
			if( (scode == 13 || scode==32) && fieldsDescription[eCol].lov && (fieldsDescription[eCol].GetProperty() & DB_FIELD_LOV_AND_EDIT)  )
			{
				 EditChoice(eId,eCol);
				 break;
			}

			if((scode == 13 || scode==32) && customLOVHandler && (fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV)  && (fieldsDescription[eCol].GetProperty() & DB_FIELD_LOV_AND_EDIT) )
			{
			
				EditCustomChoice(eId,eCol);
				break;
			}
			if( !(scode == 13 || scode==32) && fieldsDescription[eCol].lov && (fieldsDescription[eCol].GetProperty() & DB_FIELD_LOV_AND_EDIT)  )
			{
			
				 localEdit = true;
			}
			if( !(scode == 13 || scode==32) && (fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV)  && (fieldsDescription[eCol].GetProperty() & DB_FIELD_LOV_AND_EDIT)  )
			{
			
				 localEdit = true;
			}
			//Тут анализ как редактировать с учетом двойного редактирования
			if(
				 (
				   fieldsDescription[eCol].valueType==DB_FIELD_BOOL 
				 ) && 
				 (
				   (!fieldsDescription[eCol].lov &&!(fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV))   || localEdit 
				 )
			   )
			{
				 
				 EditBool(eId,eCol);
				 break;

			}
			if(
				
				(
				 fieldsDescription[eCol].valueType==DB_FIELD_STRING ||
				 fieldsDescription[eCol].valueType==DB_FIELD_INT ||
				 fieldsDescription[eCol].valueType==DB_FIELD_FLOAT 
				) && 
				(
				   (!fieldsDescription[eCol].lov &&!(fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV) ) || localEdit 
				)

			  )
			{ 	
				int l=0;
				EditText(eId,eCol,event);
				if(fieldsDescription[eCol].valueType&DB_FIELD_STRING && fieldsDescription[eCol].len>0)
				{
					l=fieldsDescription[eCol].len;
				};
				if(fieldsDescription[eCol].valueType&DB_FIELD_INT && fieldsDescription[eCol].len>0)
				{
					l=fieldsDescription[eCol].len+1;
				};
				if(fieldsDescription[eCol].valueType&DB_FIELD_FLOAT && fieldsDescription[eCol].len>0)
				{
					l=fieldsDescription[eCol].len+2;
				};


				wxDbEditTextControl *tc= (wxDbEditTextControl*) GetMainWindow()->m_editControl;
				if( l>0 )tc->SetMaxLength(l);
				break;
			}
			if(
				(
				  fieldsDescription[eCol].valueType==DB_FIELD_DATE 
				) && 
				(
				   (!fieldsDescription[eCol].lov &&!(fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV)) || localEdit

				)
			  )
			{
				 
				 EditDate(eId,eCol);
				 break;
			}

			if(
				( fieldsDescription[eCol].valueType==DB_FIELD_DATETIME) &&
				(
				   (!fieldsDescription[eCol].lov &&!(fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV)) || localEdit
				)
			  )
			{
				 EditText(eId,eCol,event);
				 break;
			}
			if(
				fieldsDescription[eCol].valueType==DB_FIELD_LIST && 
				(
				  (!fieldsDescription[eCol].lov && !(fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV)) || localEdit
				 )
			  )
			{
				 EditList(eId,eCol);
				 break;
			}
			if( fieldsDescription[eCol].lov && (fieldsDescription[eCol].GetProperty() & DB_FIELD_EDITABLE))
			{
				 EditChoice(eId,eCol);
				 break;
			}

			if(customLOVHandler && fieldsDescription[eCol].GetProperty() & DB_FIELD_USER_LOV)
			{
				EditCustomChoice(eId,eCol);
				break;
			}
			break;
		}	

		RefreshLinkField();
	};


	//Установка скроллинга для отображения требуемой колонки
	void wxDbTreeGrid::ShowCol(int nf){

		wxString s;
		wxScrolledWindow *w=GetMainWindow();
		 int x_colstart = 0;
		 int col_w=0;
		 int x1,x2,x0,y0,ww,h,y;
		 int ux,uy;
			for (int i = 0; i < GetColumnCount(); ++i ) {
				if (!GetHeaderWindow()->IsColumnShown(i)) continue;
				col_w = GetHeaderWindow()->GetColumnWidth(i);
				s.sprintf("start %d w %d",x_colstart,col_w);

				if(i==nf) break;
				x_colstart+=col_w;
			}
		w->CalcScrolledPosition(x_colstart,0,&x1,&y);
		w->CalcScrolledPosition(x_colstart+col_w,0,&x2,&y);
		w->GetViewStart(&x0,&y0);
		w->GetClientSize(&ww,&h);
		w->GetScrollPixelsPerUnit(&ux,&uy);
		x0=x0*ux;
		s.sprintf("x1 %d x2 %d x0 %d y0 %d ww %d ux %d",x1,x2,x0,y0,ww,ux);


		if(x0<x1 && x2>ww){//Если справа 
			if(col_w<ww){w->Scroll((x_colstart+col_w-ww)/ux,y0);}//Размещаем справа
			else {w->Scroll(x_colstart/ux,y0);}//Размещаем слева
			GetHeaderWindow()->Refresh();
            GetHeaderWindow()->Update();
		}
		if(x1<0){//Если слева 
			w->Scroll(x_colstart/ux,y0);//Размещаем слева
			GetHeaderWindow()->Refresh();
            GetHeaderWindow()->Update();
		}
			w->GetViewStart(&x0,&y0);
			s.sprintf("x1 %d x2 %d x0 %d y0 %d ww %d ",x1,x2,x0*ux,y0,ww);


	}

//Определение положения требуемой ячейки
	void wxDbTreeGrid::GetRect(const wxTreeItemId & id, int col, int &x, int &y, int &w, int &h){

		if (!id.IsOk()) return;
		if (!((col >= 0) && (col < GetColumnCount()))) return;
		wxDbTreeListItem* m_editItem = (wxDbTreeListItem*) id.m_pItem;

		wxTreeEvent te( wxEVT_COMMAND_TREE_BEGIN_LABEL_EDIT, 0 );
		te.SetInt (col);
		GetMainWindow()->SendEvent(0, m_editItem, &te); if (!te.IsAllowed()) return;

	
    // ensure that the position of the item it calculated in any case
    if (GetMainWindow()->m_dirty) GetMainWindow()->CalculatePositions();

	wxDbTreeListHeaderWindow* header_win = GetHeaderWindow();

    // position & size are rather unpredictable (tsssk, tssssk) so were
    //  set by trial & error (on Win 2003 pre-XP style)
     x = 0;
	 y = m_editItem->GetY();  // this is cell, not text
     h = m_editItem->GetHeight();  // consequence from above
	 w=0;
        for (int i = 0; i < col; ++i) x += header_win->GetColumnWidth (i); // start of column
        w += header_win->GetColumnWidth (col);  // currently non-main column width not pre-computed
  
		int xx,yy;

	GetMainWindow()->CalcScrolledPosition(x,y,&xx,&yy);
    x=xx;y=yy;



	}

//Создает объект для редактирования даты
	void wxDbTreeGrid::EditDate(const wxTreeItemId & id, int col){

		int x,y,w,h;
		long style=0;
		if (GetMainWindow()->m_editControl) 
		{
			GetMainWindow()->m_editControl->EndEdit(true);  // cancelled
		}
		if (GetMainWindow()->m_editControl) return;

		if (!id.IsOk()) return;
		if (!((col >= 0) && (col < GetColumnCount()))) return;

		GetRect( id, col, x,  y, w, h);
		
	
		GetMainWindow()->m_editCol = col;  // only used in OnRenameAccept()
		style = wxTE_LEFT;
		
		wxDateTime  dt;
		if( GetItemText( id,col ) == wxEmptyString ) dt.SetToCurrent();
		else dt.ParseFormat( GetItemText( id,col ),DATE_FORMAT);//"%d.%m.%Y");
		//wxLogMessage("wxDbTreeGrid::EditDate x %d y %d w %d h %d col %d date %s",x,y,w,h,col,dt.FormatDate());

		GetMainWindow()->m_editItem = ( wxDbTreeListItem* ) id.m_pItem;
		
		//GetMainWindow()->m_editControl = new wxDbEditTextControl (GetMainWindow(), -1, &GetMainWindow()->m_editAccept, &GetMainWindow()->m_editRes,
         //                                      GetMainWindow(), wxT("@@@"),
         //                                      wxPoint (x, y), wxSize (w, h), style);
		wxDbEditDateCtrl *dc = new wxDbEditDateCtrl(  	GetMainWindow(), -1, &GetMainWindow()->m_editAccept, &GetMainWindow()->m_editRes,
                                               GetMainWindow(),	dt,	wxPoint (x, y), wxSize (-1, -1), style);	
	
		GetMainWindow()->m_editControl  = dc;
		dc->fdesc=&fieldsDescription[col];
	
		
    										   
		GetMainWindow()->m_editControl->SetFocus();

	};

	//Создает объект для редактирования поля списка
	void wxDbTreeGrid::EditList(const wxTreeItemId & id, int col){
		int x,y,w,h;
		long style=0;
		if (GetMainWindow()->m_editControl) 
		{
			GetMainWindow()->m_editControl->EndEdit(true);  // cancelled
		}
		if (GetMainWindow()->m_editControl) return;

		if (!id.IsOk()) return;
		if (!((col >= 0) && (col < GetColumnCount()))) return;
	
		GetRect( id, col, x,  y, w, h);
	
	
		GetMainWindow()->m_editCol = col;  // only used in OnRenameAccept()
	
		style = 0 ;

	
		GetMainWindow()->m_editItem = ( wxDbTreeListItem* ) id.m_pItem;
		
		
		
		wxDbEditListCtrl *dc = new wxDbEditListCtrl(  	GetMainWindow(), -1, &GetMainWindow()->m_editAccept, &GetMainWindow()->m_editRes,  GetMainWindow(),	
												GetItemText(id,col),	wxPoint (x, y), wxSize (w, -1),fieldsDescription[eCol].values ,fieldsDescription[eCol].values2,style);	
		GetMainWindow()->m_editControl  = dc;
	
		dc->SetSelection(0);
		dc->SetStringSelection( GetItemText(id,col) );//Почемуто не работает ???
	
    	dc->Show();									   
		GetMainWindow()->m_editControl->SetFocus();
		
	};

//Создает объект для редактирования текстового или числового поля
	void wxDbTreeGrid::EditText(const wxTreeItemId & id, int col,wxKeyEvent &event)
	{
		int x,y,w,h;
		long style=wxTE_PROCESS_ENTER   ;
		if (GetMainWindow()->m_editControl) 
		{
			GetMainWindow()->m_editControl->EndEdit(true);  // cancelled
		}
		if (GetMainWindow()->m_editControl) return;
		if (!id.IsOk()) return;
		if (!((col >= 0) && (col < GetColumnCount()))) return;
	
		GetRect( id, col, x,  y, w, h);
	
		GetMainWindow()->m_editItem = (wxDbTreeListItem*) id.m_pItem;
		GetMainWindow()->m_editCol = col;  // only used in OnRenameAccept()


    switch (GetHeaderWindow()->GetColumnAlignment (col)) 
	{
        case wxALIGN_LEFT:   {style = wxTE_LEFT;   /*x -= 1; */break;}
        case wxALIGN_CENTER: {style = wxTE_CENTER; /*x -= 1; */break;}
        case wxALIGN_RIGHT:  {style = wxTE_RIGHT;  x += 0; break;}  // yes, strange but that's the way it is
    }
    

	
// now do edit (change state, show control)
    GetMainWindow()->m_editCol = col;  // only used in OnRenameAccept()
    GetMainWindow()->m_editControl = new wxDbEditTextCtrl (GetMainWindow(), -1, &GetMainWindow()->m_editAccept, &GetMainWindow()->m_editRes,
                                               GetMainWindow(), GetMainWindow()->m_editItem->GetText (col),
                                               wxPoint (x, y), wxSize (w, h), style);
    										   
    wxDbEditTextCtrl *tc=	(wxDbEditTextCtrl*)GetMainWindow()->m_editControl ;
	tc->fdesc=&fieldsDescription[col];
	if(fieldsDescription[col].len>0)	tc->SetMaxLength(fieldsDescription[col].len);
	if(fieldsDescription[col].validator) tc->SetValidator(*fieldsDescription[col].validator);
	//Если поле время не заполнено то заполняем текущим
	if(fieldsDescription[col].valueType==DB_FIELD_DATETIME)
	{
		if(GetMainWindow()->m_editItem->GetText(col)==wxEmptyString)
		{
			wxDateTime dt;
			dt=dt.Today();
			tc->SetValue(dt.Format(DATETIME_FORMAT));
		};
	};

					
    GetMainWindow()->m_editControl->SetFocus();
	//Если нажата текстовая клавиша, то устанавливаем текст поля
	
	if(event.GetKeyCode()!=0  && event.GetKeyCode()!=32 && event.GetKeyCode()!=13 && event.GetKeyCode()!=341)
	{
		tc->SetValue(wxT(""));
		tc->EmulateKeyPress(event);

	}

	};
	



//Обработчик окончания редактирования ячейки
	void wxDbTreeGrid::OnEndEdit(wxTreeEvent &event)
	{

		//Сравниваем были ли изменения
	
		int fl=0;
		for(int i=0; i<(int)GetColumnCount() && i<(int)buf.Count(); i++)
		{
			if(buf[i]!=GetItemText(eId,i)) fl++;
		};
		buf.Clear();
		if(!fl && !( eCol>=0 && fieldsDescription[eCol].valueType == DB_FIELD_BOOL) ) return;//Изменений не было

		SetFlagsValue(eId,DB_UPDATED_RECORD);
		RefreshLinkField();
		bool ck = CheckValue(GetItemText(eId,eCol), eCol);
		if ( !ck )
		{

		}
		if( postFieldChangedHandle )
		{
			wxArrayString as;
			GetCurrentRecord(as,eId);
			wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
			ev.tp=DB_POST_FIELD_CHANGED_HANDLE;
			ev.SetEventObject( (wxWindow*)this );
			ev.objType=DB_OBJ_TYPE_TREE_GRID;
			ev.as=&as;
			ev.obj=this;
			curId=eId;
			postFieldChangedHandle->ProcessEvent(ev);
		}

	};

//Обрабтчик обновления грида перечитывает записи из БД
	void wxDbTreeGrid::OnRefreshRecord(wxCommandEvent & event)
	{
		if(!MenuItemIsActive(ID_REFRESH)) return;
		if( HasUpdate() )
		{
				int answer=wxMessageBox(TEXT_IS_CHANGES,wxEmptyString,wxOK|wxCANCEL) ;
				if( !(wxOK == answer )) return ;
		}
		RefreshRecords();				

	}

//Обновляет грид, перечитывает записи из БД
	void wxDbTreeGrid::RefreshRecords(){

		Reset();
		CreateQuery();
		ExecuteQuery();

		GetRows();
		noEdit=true;
		SelectFirstRecord();
		noEdit=false;
	};


	//Считывает записи из БД, либо все либо часть в зависимости от настроек
	bool wxDbTreeGrid::GetRows(){
		wxBeginBusyCursor();
		bool res=GetAllRow();
		wxEndBusyCursor();
		return res;
		
	};
	//Проверка можно ли закрыть окно , и подготавливаемся к закрытию, надо когда редактируется поле и в него введены неправильные данные
	bool wxDbTreeGrid::PrepareClose()
	{
		if( GetMainWindow()->m_editControl ) return false;
		return true;
	}

	// Обработчик запроса
	void wxDbTreeGrid::OnQueryRecord(wxCommandEvent &event)
	{

		if(!MenuItemIsActive(ID_QUERY)) return;
		if( HasUpdate() )
		{
			int answer=wxMessageBox(TEXT_IS_CHANGES_PERFOM_QUERY,wxEmptyString,wxOK|wxCANCEL) ;
			if((wxOK == answer ))
			{  
				Query();
				return;
			}
			else return;
		
		};
		Query();
	};

	//Вызвать окно ввода запроса и выполнить его
	bool wxDbTreeGrid::Query(){
		DbQueryDialog d(this,wxID_ANY,wxT("Ввод запроса"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
		d.Init();
		d.tree->Setup(*this,DB_FIELD_QUERY);
		d.tree->menu=0;
		d.where->SetValue(queryDataWhere);
	
		//Устанвливаем ширину голонок как в родительском гриде
		for(int i=0; i<GetColumnCount() && i<d.tree->GetColumnCount(); i++)
		{
			if(d.tree->GetColumnWidth(i) && GetColumnWidth(i)>0)
			{
  

				d.tree->SetColumnWidth(i,GetColumnWidth(i));
			};
		}
			d.tree->noEdit=true;
			d.tree->SelectFirstRecord();
			d.tree->noEdit=false;
		if(d.ShowModal() == wxID_OK  ) {
			Reset();
			queryData=d.GetQueryData();
		
			queryDataWhere = d.where->GetValue();
			CreateQuery();
			ExecuteQuery();
			GetRows();
			d.Destroy();
			noEdit=true;
			SelectFirstRecord();
			noEdit=false;
			return true;
		}; 
		d.Destroy();
		return false;
	};
	//Установка полей
	void wxDbTreeGrid::Setup(wxDbQueryObject& obj, int property){
		SetFieldDescription(obj.GetFieldDescription());
		queryData=obj.queryData;
		searchData=obj.searchData;
		wxString s;
		int len;

		//Устанавливаем поля которые не нужны - невидимыми
		for(int i=0;i<(int)fieldsDescription.Count();i++)
		{
			s.sprintf("%d %d %s",i,fieldsDescription[i].GetProperty()&property,fieldsDescription[i].fieldName);
			fieldsDescription[i].lov=0;//Удаляем список значений
			fieldsDescription[i].ResetProperty(DB_FIELD_USER_LOV);//Очищаем свойство пользовательский LOV
			fieldsDescription[i].ResetProperty(DB_FIELD_NOT_NULL);//Очищаем свойство пользовательский LOV

			if(!(fieldsDescription[i].GetProperty()&property)) 
			{
				fieldsDescription[i].ResetProperty(DB_FIELD_VISIBLE);
			}
			else
			{

				fieldsDescription[i].SetProperty(DB_FIELD_VISIBLE);
			};
		};

		CreateField();

		wxTreeItemId it=AppendItem(GetRootItem(),wxEmptyString);

		if(property==DB_FIELD_QUERY) len = (int)queryData.Count();
		else len = (int)searchData.Count();

		for (i=0;i<len && i<(int)fieldsDescription.Count(); i++)
		{
			if(property==DB_FIELD_QUERY) SetItemText(it,i,queryData[i]);
			else SetItemText(it,i,searchData[i]);

		};
		
	}

	//Сбросить все данные , привести грид к состоянию готовому выполнить новый запрос к БД (строка запроса не переформировывается)
	void wxDbTreeGrid::Reset()
	{
			wxTreeItemId id = GetRootItem();
			if(dbq) {db->CloseResultSet(dbq); dbq=0;}
			//if( db ) db->RollBack();
			wxTreeItemId id1;
			EmptyId=id1;
			eId=id1;
			curId=id1;
			eCol=0;
			rcount = 0;
			SetProperty(DB_GRID_ALL_RECORD_FETCHED);
			if( id.IsOk()) DeleteChildren(id);
			ResetProperty(DB_GRID_IS_CHANGED);
			queryData.Clear();
			queryDataWhere = wxEmptyString;
			recArray.Clear();

	}

/// Обработчик поиска
	void wxDbTreeGrid::OnSearchRecord(wxCommandEvent &event)
	{
		Search();
	};
		/// Обработчик поиска следующей записи
	void wxDbTreeGrid::OnSearchRecordNext(wxCommandEvent &event)
	{
		if(!MenuItemIsActive(ID_SEARCH_NEXT)) return;
		SearchRecord();
	};




	///Вызвать окно ввода поиска и выполнить его
	bool wxDbTreeGrid::Search()
	{
		DbQueryDialog d(this,wxID_ANY,wxT("Поиск"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
		d.Init();
		d.tree->Setup(*this,DB_FIELD_SEARCH);
		d.where->SetEditable( false );
		d.where->Disable();
		//Устанвливаем ширину голонок как в родительском гриде
		for(int i=0; i<GetColumnCount() && i<d.tree->GetColumnCount(); i++)
		{
				if(d.tree->GetColumnWidth(i) && GetColumnWidth(i)>0) d.tree->SetColumnWidth(i,GetColumnWidth(i));
		}
		d.tree->menu=0;
		d.tree->SelectFirstRecord();
		
		if(d.ShowModal() == wxID_OK  ) {
			searchData=d.GetQueryData();
			d.Destroy();
			return SearchRecord();
		};
		d.Destroy();
		return false;

	};

	///Найдти запись соответствующую 
	bool wxDbTreeGrid::SearchRecord()
	{
		wxArrayString as;
		wxTreeItemId id;
		wxTreeItemId id1;
		if(!eId.IsOk())
		{
			id=GetRootItem();
		}
		else
		{
			id=eId;
		}
		id=GetNext(id);
		while(id.IsOk())
		{
			GetCurrentRecord(as,id);
			if(CheckSearch(as))
			{
				//eId=id;
				GetMainWindow()->m_curColumn=eCol;
				SelectItem(id);
				if(GetMainWindow()->m_editControl)
				{
					//Запоминаем состояние чтоб лишний раз свойство редактирования блока не вылезло
					//int fl=boolProperty;
					GetMainWindow()->m_editControl->EndEdit(false);
					//boolProperty=fl;
				};
				ScrollTo(eId);
				return true;
			};
			id1=id;
			id=GetNext(id);
			//Считываем следующую порцию данных
			if(!id.IsOk() && !(DB_GRID_ALL_RECORD_FETCHED&boolProperty))
			{
				GetRows();
				id=GetNext(id1);
			}
			

		};
		return false;
	};

	///Создает объект для выбора значения из списка (данные получаем из БД)
	void wxDbTreeGrid::EditChoice(const wxTreeItemId & id, int col)
	{


			if( !fieldsDescription[col].lov)
			{
				wxMessageBox(TEXT_IS_LIST_NOT_SET);
				return;
			};
			wxDbLOV* l=fieldsDescription[col].lov;
			DbLovDialog d(this,wxID_ANY,wxT("Выбор значения"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
			d.tree->db=db;
			d.Setup(*fieldsDescription[col].lov);
			
			if(d.ShowModal()== wxID_OK){
				if(d.tree->eId.IsOk()){

					for(int i=0; i<(int)(*l).pair.Count(); i++)
					{
						if((*l).pair[i].i2>=0)
						{
							SetItemText (eId,(*l).pair[i].i2,  d.tree->GetItemText(d.tree->eId,(*l).pair[i].i1));
						}

					};
				};
				SetFlagsValue(eId,DB_UPDATED_RECORD);
				
			};
			
		//	d.Close();
		//	d.Destroy();
			noEdit = true;
			
			if(eId.IsOk())SelectItem( eId );
			noEdit = false;
			
	};

	void wxDbTreeGrid::OnTextEnter(wxCommandEvent &event)
	{
		wxMessageBox(wxT("wxDbTreeGrid::EditChoice"));

	};

	//Обработчик нажатия двойной кнопки мыши
	void wxDbTreeGrid::OnMouseDoubleClick(wxMouseEvent& event)
	{

		//Передаем событие родительскому окну
		if (GetParent()) GetParent()->GetEventHandler()->ProcessEvent(event);
	}
	
	///Сделать активной первую запись в гриде
	bool wxDbTreeGrid::SelectFirstRecord()
	{

		wxTreeItemId id=GetNext(GetRootItem());
		if(id.IsOk())
		{
			
				noEdit=true;
				SelectItem(id);
				eId=id;//было первым !!!
				RefreshLinkField();
				noEdit=false;
		}
		else
		{
			RefreshLinkField();
			return false;
		}

		for(int i=0; i<(int)fieldsDescription.Count(); i++)
		{
			if(fieldsDescription[i].GetProperty()&DB_FIELD_VISIBLE){ eCol=i; break;}
		};
		return true;
	};
	///Получить значение PK
	wxString wxDbTreeGrid::GetPkValue()
	{
		wxString s(wxT("null"));
		if(eId.IsOk()){
			s=GetItemText(eId,GetPkNum());

		}
		return s;
	};

	///Получить из базы все записи для дерева
	bool wxDbTreeGrid::GetAllRowTree(wxTreeItemId& parent){
		wxTreeItemId id=parent;
		wxTreeItemId id1;
		wxArrayString sa;
		while(GetNextRecord(sa))
		{
			if(GetItemText(id,GetPkNum())==sa[GetTreeNum()])
			{
				id=AppendItem(id,wxEmptyString);
			}
			else
			{
				//Ищем вверх только там , данные уже в виде дерева
				while(GetItemText(id,GetPkNum())!=sa[GetTreeNum()] &&  (id)!=GetRootItem())
				{
					id=GetItemParent(id);
				}
				id=AppendItem(id,wxEmptyString);

			};
			for(int i=0; i<(int)sa.Count(); i++){
				SetItemText( id,i,sa[i]);
			}
			if(postQueryHandler)
			{
				wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
				ev.tp=DB_POST_QUERY_HANDLE;
				ev.SetEventObject( (wxWindow*)this );
				ev.objType=DB_OBJ_TYPE_TREE_GRID;
				ev.as=&sa;
				ev.obj=this;
				curId=id;
				postQueryHandler->ProcessEvent(ev);
			};
		};

		return true;
	}


	
	wxString wxDbTreeGrid::GetItemTreeValue(wxTreeItemId& id)
	{
		int tnum=GetTreeNum();
		if(!id.IsOk() || id==GetRootItem()){
			return wxEmptyString;
		}
		else
		{
			return GetItemText(id,tnum);
		};
		return wxEmptyString;

	};
///Проверка, есть ли записи в гриде
	bool wxDbTreeGrid::IsRecords()
	{	
		if(GetRootItem().IsOk() && GetNext(GetRootItem()).IsOk()) return true;
		return false;
		
	};

	///Обработчик вставки дочерней записи(для дерева)
	void wxDbTreeGrid::OnInsertChildRecord(wxCommandEvent & event)
	{
		if(!MenuItemIsActive(ID_INSERT_CHILD)) return;
		InsertChildRecord();
	};
	//Вставить дочернюю запись
	bool wxDbTreeGrid::InsertChildRecord(){
		
		wxTreeItemId id = GetSelection();
		wxString tId;
		wxTreeItemId i1;
		int col=-1;
		if(!id.IsOk())i1=AppendItem (GetRootItem(), wxT(""));
		else 
		{
			//i1=AppendItem (id, wxT(""));
			i1 = InsertItem (id, 0, wxT(""));
			tId=GetItemText(id,GetPkNum());
		};
	//	if(!i1.IsOk()){wxMessageBox(wxT("wxDbTreeGrid::InsertRecord- Надо выбрать запись")); return false;}

	
		for(int i=0; i<(int)GetColumnCount() && i<(int)fieldsDescription.Count(); i++)
		{
			SetItemText(i1,i,fieldsDescription[i].defValue);
			if( col == -1 && fieldsDescription[i].GetProperty()&DB_FIELD_VISIBLE )
			{
				col = i;
			}
		};
		if( col >=0 )
		{ 
		
			eCol=col;
			ShowCol(eCol);
			GetMainWindow()->m_curColumn=eCol; 
			GetMainWindow()->RefreshSelected(); 
			
		};

		if(tId!=wxEmptyString)
		{
			SetItemText(i1,GetTreeNum(),tId);
		};
		SetFlagsValue(i1,DB_INSERTED_RECORD);
		SelectItem(i1);
		EnsureVisible(i1); 
		return true;
	}
	///Заполнение грида из массива
	bool wxDbTreeGrid::FillGrid(ArrayDbRec &ar)
	{

		if(ar.Count()<=0) return false;
		int ncol;
		ncol=(int)ar[0].Count();
		wxString s;
		wxDbGridFieldDesc fd;
		for(int i=0; i<ncol; i++)
		{	
			s.sprintf("Col%d",i);
			AddColumn(s);
			fd.columnName=s;
			fd.fieldName=s;
			fd.len=0;
			fd.SetProperty(DB_FIELD_DBASE);
			fd.valueType=DB_FIELD_STRING;
			fieldsDescription.Add(fd);

		};
		AddColumn(wxT("status"));
		fd.fieldName=wxT("status");
		fd.valueType=DB_FIELD_FLAGS;
		fd.columnName=wxT("status");
		fd.ResetProperty(DB_FIELD_DBASE|DB_FIELD_VISIBLE|DB_FIELD_SEARCH|DB_FIELD_QUERY);
		fieldsDescription.Add(fd);
		SetColumnWidth(ncol,0);

		wxTreeItemId rid=GetRootItem();
		wxTreeItemId id;
		if(!rid.IsOk())
		{
			rid=AddRoot(wxEmptyString);
		}
		for( i=0; i<(int)ar.Count(); i++)
		{
			id=AppendItem(rid,wxEmptyString);
			for(int j=0; j<ncol; j++)
			{
				SetItemText(id,j,ar[i][j]);
			};
			s.sprintf("%d",DB_QUERY_RECORD);
			SetItemText(id,ncol,s);
		};
		boolProperty|=DB_GRID_ALL_RECORD_FETCHED;
		return true;
	};



///Создает объект для выбора значения из списка настроенного пользоателем
	void wxDbTreeGrid::EditCustomChoice(const wxTreeItemId & id, int col)
	{
				if(!customLOVHandler) return;

				wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
				wxArrayString as;
				GetCurrentRecord( as);
				ev.tp=DB_CUSTOM_LOV_HANDLE;
				ev.SetEventObject( (wxWindow*)this );
				ev.objType=DB_OBJ_TYPE_TREE_GRID;
				ev.as=&as;
				ev.obj=this;
				curId=eId;
				customLOVHandler->ProcessEvent(ev);
				if(!ev.veto) 
				{
					FillRecord(as,eId);
					SetFlagsValue(eId,DB_UPDATED_RECORD);
				}

	};

///Добавить связанное поле
void wxDbTreeGrid::AddLinkField(wxDbField* f, int nf)
{

	wxDbFieldLink p;
	p.obj=f->GetEvtHandler();
	p.fnum=nf;
	linkFields.Add(p);
	f->owner=this;
	f->fdesc=&fieldsDescription[nf];

	
};
///Обновление значений связанных полей из текущей записи
void wxDbTreeGrid::RefreshLinkField()
{
//	wxMessageBox("wxDbTreeGrid::RefreshLinkField1");!!!!!
	//if(!eId.IsOk()) return;

	wxDbFieldEvent  ev=wxDbFieldEvent(dbEVT_COMMAND_REFRESH_FIELD);
	ev.field=this;
	int nf;
	for(int i=0; i<(int)linkFields.Count();i++)
	{
		nf=linkFields[i].fnum;
		if( eId.IsOk() )
		{
			ev.value1=GetItemText(eId,nf);
			ev.value2=GetItemText(eId,nf+1);
		}
		else
		{
			ev.value1=wxEmptyString;
			ev.value2=wxEmptyString;
		}
		if(linkFields[i].obj) linkFields[i].obj->ProcessEvent(ev);
	};
//	wxMessageBox("wxDbTreeGrid::RefreshLinkField");
}

void wxDbTreeGrid::OnwxDbFieldChanged(wxDbFieldEvent &event)
{
	for(int i=0; i<(int)linkFields.Count();i++)
	{
		if(linkFields[i].obj==event.field && eId.IsOk())
		{
			SetItemText(eId,linkFields[i].fnum,event.value1);
			if(fieldsDescription[i].valueType==DB_FIELD_LIST) SetItemText(eId,linkFields[i+1].fnum,event.value2);
			SetFlagsValue(eId,DB_UPDATED_RECORD);
		};
	};
};

void wxDbTreeGrid::OnwxDbFieldCmnd(wxDbFieldEvent &event)
{
	int fn=-1;
	for(int i=0; i<(int)linkFields.Count();i++)
	{
		if(linkFields[i].obj==event.field && eId.IsOk())
		{
			fn=linkFields[i].fnum;
		};
	};
	if(fn<0) return;
	if(event.eType==DB_FIELD_SET_FOCUS_EVT)
	{
		eCol=fn;
		Refresh();
		SetMenuItems();
		wxDbGridEvent ev(dbEVT_GRID_ACTIVE_HANDLE);
		ev.grid=this;
		if(dispatcher)
		{
			dispatcher->ProcessEvent(ev);
		};
	};
	if(event.eType==DB_FIELD_LOV_EVT)
	{
			if( fieldsDescription[eCol].lov && eCol>=0 && eId.IsOk()){
				 wxKeyEvent event;
				 event.m_keyCode=0;
				 EditCell(eId,eCol,event);
			}
	};
	if(event.eType==DB_FIELD_CUSTOM_LOV_EVT)
	{
			if( fieldsDescription[eCol].GetProperty()&DB_FIELD_USER_LOV && customLOVHandler)
			{
				 wxKeyEvent event;
			     event.m_keyCode=0;
				 EditCell(eId,eCol,event);
			}
	};
	



};

void wxDbTreeGrid::OnSetFocus(wxFocusEvent& event){


	event.Skip();

};
void wxDbTreeGrid::OnKeyDown(wxKeyEvent &event){
		//event.Skip();
		//return;
		wxUint32 scode;
		scode=event.GetKeyCode();
		//Обрабатываем выбор для LOVа
		wxString s;
		long v;
		

		if( eId.IsOk() )
		{
			s= GetItemText( eId, GetColumnCount()-1 );
			s.ToLong( &v );
		}
		


		if (event.GetModifiers()&wxMOD_CONTROL || event.GetModifiers()&wxMOD_ALT || scode==WXK_DOWN || scode==WXK_UP)
		{
			event.Skip();
			return;
		};


		if (scode==13 && GetParent()) GetParent()->GetEventHandler()->ProcessEvent(event);

	/*	if(( eCol>=0 && fieldsDescription[eId].valueType == DB_FIELD_BOOL && scode==13 || scode==341 || (scode >32 && scode<=255)) && (GetProperty(DB_GRID_UPDATE_ENABLE) || v&DB_INSERTED_RECORD) && scode!=9 && scode !=WXK_DELETE ){
			if(eId.IsOk() && eCol>=0) EditBool(eId,eCol);
			wxLogMessage("EDIT BOOL");
			return;
		}
*/
		if(( (eCol>=0 && scode==32 && this->fieldsDescription[eCol].valueType== DB_FIELD_BOOL) ||  scode==13 || scode==341 || (scode >32 && scode<=255)) && (GetProperty(DB_GRID_UPDATE_ENABLE) || v&DB_INSERTED_RECORD) && scode!=9 && scode !=WXK_DELETE ){
			if(eId.IsOk() && eCol>=0 && this->fieldsDescription[eCol].valueType!= DB_FIELD_BOOL ) EditCell(eId,eCol,event);
			if(eId.IsOk() && eCol>=0 && this->fieldsDescription[eCol].valueType== DB_FIELD_BOOL ) EditBool(eId,eCol);
		
			return;
		}
		event.Skip();
};

void wxDbTreeGrid::OnBeginDrag(wxTreeEvent &event)
{

		wxTreeItemId  id=event.GetItem();
	
		dragItem=id;
	//	SetDragItem(id);
};

void wxDbTreeGrid::OnEndDrag(wxTreeEvent &event)
{

		wxTreeItemId  id=event.GetItem();
		if( !(boolProperty & DB_GRID_UPDATE_ENABLE) ) return;
		MoveTreeNode(dragItem,id);
		Refresh();
		wxTreeItemId id1;
		dragItem=id1;
};

///Переместить узел дерева
bool wxDbTreeGrid::MoveTreeNode(wxTreeItemId &src,wxTreeItemId &dst)
{
	if(!src.IsOk() || !dst.IsOk()) return false;
	if(src==dst) return false;
	if(GetTreeNum()<0 || GetTreeNum()>=GetColumnCount()) return false;
	if(!(GetProperty(DB_GRID_IS_TREE))) return false;
	wxMessageDialog d(this,wxT("Переместить на уровень ниже ?"), wxEmptyString,	wxYES_NO | wxCANCEL );
	int res=d.ShowModal();
	wxTreeItemId parent=dst;
	if(res==wxID_CANCEL) return false;
	if(res==wxID_NO)
	{
		parent=GetItemParent(dst);
	}
	if(!parent.IsOk()) return false;
	//Проверка на вхождение , если перемещаемый лежит выше куда перемещаем то ошибка
	wxTreeItemId id=parent;
	while(id.IsOk()){
		if(id==src)
		{
			wxMessageBox(TEXT_IS_MOVE_NODE);
			return false;
		}
		id=GetItemParent(id);
	}
	
	id=AppendItem(parent,wxEmptyString);
	CopyRow(src,id);
	//Reparent(src,id);
	//return true;
	if(!Reparent(src,id))
	{
		Delete(id);
		return false;
	}
	else
	{

		int tn=GetTreeNum();
		if(tn>=0 && tn<GetColumnCount() && GetPkNum()>=0)
		{
			wxString s=GetItemText(parent,GetPkNum());
			SetItemText(id,tn,s);
		}
		else
		{
			Delete(id);
			return false;
		}
		Expand(parent);
		SelectItem(id);
		eId=id;
		Delete(src);
		SetFlagsValue(eId,DB_UPDATED_RECORD);
		RefreshLinkField();
		return true;
		
	};
	return true;

};

///Переместить потомков узла дерева (рекурсия)
bool wxDbTreeGrid::Reparent(wxTreeItemId &src,wxTreeItemId &dst)
{
	if(!src.IsOk() || !dst.IsOk()) return false;
	wxTreeItemIdValue l=0;
	wxTreeItemId id=GetFirstChild(src,l);
	wxTreeItemId id1;
	while(id.IsOk())
	{
		id1=AppendItem(dst,wxEmptyString);
		CopyRow(id,id1);
		if(!Reparent(id,id1)) return false;
		id=GetNextSibling(id);
	};
	return true;
};

///Скопировать данные их одной строки в другую
void wxDbTreeGrid::CopyRow(wxTreeItemId &src,wxTreeItemId &dst)
{
	if(!src.IsOk() || !dst.IsOk()) return ;
	for(int i=0; i<GetColumnCount(); i++)
	{
		SetItemText(dst,i,GetItemText(src,i));
	};

}

void wxDbTreeGrid::StopEdit()
{
	if(GetMainWindow()->m_editControl)GetMainWindow()->m_editControl->Destroy();
};


///Сохранить настройки GUI в массив
bool wxDbTreeGrid::SaveGUI(ArrayDbRec &ar,wxString &pName)
{
	wxString s=GetName();
	wxString s1,s2;
	for (int i=0; i<GetColumnCount(); i++)
	{

			s1.sprintf("col%d",i);
			s2.sprintf("%d",GetColumnWidth(i));
			SavePar(ar,pName,s,s1,s2);
	};
	return true;
};
///Загрузить настройки GUI из массива
bool wxDbTreeGrid::LoadGUI(ArrayDbRec &ar,wxString &pName)
{
	wxString s=GetName();
	wxString s1,s2;
	long l;
	for (int i=0; i<GetColumnCount(); i++)
	{

			s1.sprintf("col%d",i);
			s2=LoadPar(ar,pName,s,s1);
			s2.ToLong(&l);

			if(l>0 && fieldsDescription[i].GetProperty()&DB_FIELD_VISIBLE) SetColumnWidth(i,l);
	};
	return true;
};


///Вызывается после удаления  вставки  и перемещения записи
void wxDbTreeGrid::RowRecalc()
{
	
	wxTreeItemId id = GetRootItem();
	int i = 1;
	long l;
	wxString s;
	id =  GetNext( id );
	while( id.IsOk() )
	{
		s = GetItemText( id, GetColumnCount()-1);
		s.ToLong( &l );
		l = l - (int( l/1024 ))*1024;
		l = l+i*1024;
		s.sprintf( "%d", l );
		SetItemText(id, GetColumnCount()-1, s );
		id = GetNext( id );
		i++;
	}
	rcount = i-1;
};

///Получить строку с тукущей позицией
wxString wxDbTreeGrid::GetCurPos()
{
	
	wxString s,ss;
	long l;
	s = wxT("?/?");
	if( rcount <= 0 )
	{
		
		return s;
	};
	if( eId.IsOk() && GetColumnCount() >0   )
	{

		ss = GetItemText( eId, GetColumnCount()-1);
		ss.ToLong( &l );
		l =   l/1024;		
		if( GetProperty()&DB_GRID_ALL_RECORD_FETCHED )
		{
			s.sprintf( "%d/%d", l, rcount);
		}
		else
		{
			s.sprintf( "%d/?", l, rcount);
		};

		return s;
	};

	return s;

};

///Обработчик события по раскрытию узла
void wxDbTreeGrid::OnExpanded(  wxTreeEvent &event )
{


	wxTreeItemId eid = event.GetItem();
	//Если дерево и записи не сразу все читаются то пытаемся считать подчиненные записи
	if(!(boolProperty&DB_GRID_IS_TREE && !(boolProperty&DB_GRID_READ_ONE))) return;
	wxArrayString aa;
	wxString s;
	wxTreeItemId id;
	wxTreeItemIdValue cookie;
	long v;
	int n=GetPkNum();
	int k = GetTreeNum();
	if ( k< 0  || n< 0 ) return ;

	if( ! eid.IsOk() && k >=0)
	{
		s = fieldsDescription[k].fieldName + wxT(" is null ");
	}
	if( eid.IsOk() && k >=0) 
	{

	
		
		s = GetItemText( eid, fieldsDescription.Count()-1);
		s.ToLong( &v );

		//проверяем признак и удаляем лишний елемент
		if( v&DB_TREE_QUERY )
		{
			
			id = GetFirstChild( eid , cookie );
			Delete ( id );
			v&=~DB_TREE_QUERY;
			s.sprintf("%d",v);
			SetItemText( eid, fieldsDescription.Count()-1, s);
		}
		else 
		{
			return;
		}

		s = GetItemText( eid, n);
		s = fieldsDescription[k].fieldName + wxT(" = '") + s + wxT("'");
	}
	

	CreateQuery();
	AddWhere ( s );
	ExecuteQuery();
	wxBeginBusyCursor();
	bool res= GetTreeLevel(eid);
	wxEndBusyCursor();
	
	return ;
};

///Считать текущий уровень дерева из БД
bool wxDbTreeGrid::GetTreeLevel(wxTreeItemId &id)
{


	wxTreeItemId root;
	wxTreeItemId id1,id2;
	wxArrayString sa;
	wxString s;
	long v;
	root = id;
	if( !id.IsOk() )
	{
		root = GetRootItem();
	};
	if( !root.IsOk() )
	{
		root = AddRoot( wxEmptyString );
	};

	while(GetNextRecord(sa))
	{

		id1 = AppendItem(root,wxEmptyString);
		for(int i=0; i<(int)sa.Count(); i++){
				SetItemText( id1,i,sa[i]);
		};
		s = sa[sa.Count() -1]; 
		s.ToLong( &v );
		v|=DB_TREE_QUERY;
		s.sprintf( "%d", v );
		SetItemText( id1, sa.Count()-1, s);
		//SetFlagsValue(id1,DB_TREE_QUERY);
		id2 = AppendItem(id1,wxEmptyString);
		
		if(postQueryHandler)
		{
				wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
				ev.tp=DB_POST_QUERY_HANDLE;
				ev.SetEventObject( (wxWindow*)this );
				ev.objType=DB_OBJ_TYPE_TREE_GRID;
				ev.as=&sa;
				ev.obj=this;
				curId=id;
				postQueryHandler->ProcessEvent(ev);
		};

			
	};

	return true;
}

///Обработчик дублирования записи
void wxDbTreeGrid::OnDuplicateRecord(wxCommandEvent & event)
{
	wxTreeItemId id, id1;
	long v;
	wxString s;


	//Дублирует вернюю по отношению к текущей запись
	if( !eId.IsOk() ) return;
	s = GetItemText( eId, fieldsDescription.Count()-1 );
	s.ToLong(&v);
	if(!(v&DB_INSERTED_RECORD)) return;

	id = GetRootItem();
	if( !id.IsOk() ) return;
	id1 = GetPrev( eId );
	if( !id1.IsOk() ) return;
	if( id == id1 ) return;
	for( int i = 0; i < GetColumnCount() - 1; i++ )
	{
		if ( 
			  !( fieldsDescription[i].GetProperty()&DB_FIELD_KEY || 
		        fieldsDescription[i].GetProperty()&DB_FIELD_TREE || 
		        fieldsDescription[i].GetProperty()&DB_FIELD_FK  
		      )
		   )
		{
			SetItemText( eId, i, GetItemText( id1, i ) );
		}
	};
	RefreshLinkField();

	



};


///Вызвать обработчик заполнив предварительно контекст
void wxDbTreeGrid::CallHandler(wxEvtHandler* handler, int tp)
{
	wxArrayString sa;
	if(handler)
	{
				wxDbRecEvent ev(dbEVT_REC_HANDLE,((wxWindow*)this)->GetId());
				ev.tp=tp;
				ev.SetEventObject( (wxWindow*)this );
				ev.objType=DB_OBJ_TYPE_TREE_GRID;
				ev.as=&sa;
				ev.obj=this;
				curId=eId;
				handler->ProcessEvent(ev);
	};
}


///Обработчик показать свойства дерева
void wxDbTreeGrid::OnShowProperty(wxCommandEvent & event)
{
	wxFile f;
	wxString fn, pname ;
	int j;
	wxStandardPaths path;

	wxString s,ss,sss;
	
	ss = wxT("\\block_rep_db_test.html");

	fn = path.GetTempDir();
	fn += ss;
	


	

	
	//wxMessageBox(fn);
	if( !f.Open( fn, wxFile::write ) )
	{
		wxMessageBox( TEXT_IS_SAVE_FILE + fn );
	}

	f.Write("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n");
	f.Write("<html>\n");
	f.Write("<head>\n");
	f.Write("<meta content=\"text/html; charset=Windows-1251\" http-equiv=\"content-type\">\n");
	f.Write("<title  >отчет</title>\n");
	f.Write("</head>\n");
	f.Write("<body style=\"height: 62px;\">\n");
	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("Query:\n");
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s = query;
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("SelectClause:\n");
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s = selectClause;
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("FromClause:\n");
	f.Write("<br>\n </div> \n");
	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s = fromClause;
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("WhereClause:\n");
	f.Write("<br>\n </div> \n");
	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s = whereClause;
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("OrderClause:\n");
	f.Write("<br>\n </div> \n");
	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s = orderClause;
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("DstName:\n");
	f.Write("<br>\n </div> \n");
	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s = dstName;
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("PK:\n");
	f.Write("<br>\n </div> \n");
	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s.sprintf("%d",GetPkNum());
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("FK:\n");
	f.Write("<br>\n </div> \n");
	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s.sprintf("%d",GetFkNum());
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("TreeFK:\n");
	f.Write("<br>\n </div> \n");
	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
	s.sprintf("%d",GetTreeNum());
	ProcessStr(s);
	f.Write(s);
	f.Write("<br>\n </div> \n");

	f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
	f.Write("Columns:\n");
	f.Write("<br>\n </div> \n");
    f.Write("<table style=\"text-align: center; width: 28cm; font-size: 8pt; font-family: arial;  font-style: normal;\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\">\n");
	f.Write("<tbody>\n");
	f.Write("<tr>\n");
	f.Write("<td>\n");
	f.Write("№\n");
	f.Write("</td>\n");
	f.Write("<td>\n");
	f.Write("fieldName\n");
	f.Write("</td>\n");
	f.Write("<td>\n");
	f.Write("columnName\n");
	f.Write("</td>\n");
	f.Write("<td>\n");
	f.Write("valueType\n");
	f.Write("</td>\n");
	f.Write("<td>\n");
	f.Write("length\n");
	f.Write("</td>\n");
	f.Write("<td>\n");
	f.Write("presition\n");
	f.Write("</td>\n");
	f.Write("<td>\n");
	f.Write("LOV number\n");
	f.Write("</td>\n");
	f.Write("<td>\n");
	f.Write("minLen\n");
	f.Write("</td>\n");


	
	f.Write("</tr>\n");


	
	for ( int i = 0; i < (int)fieldsDescription.Count() - 1; i ++ )
	{
		s.sprintf("%d\n",i);
		f.Write("<tr>\n");
		
		f.Write("<td>\n");
		f.Write(s);
		f.Write("</td>\n");
		
		f.Write("<td>\n");
		f.Write(fieldsDescription[i].fieldName);
		f.Write("</td>\n");

		f.Write("<td>\n");
		f.Write(fieldsDescription[i].columnName);
		
		f.Write("</td>\n");
		switch (fieldsDescription[i].valueType)
		{
			case DB_FIELD_STRING:
				s = wxT("STRING");
				break;
			case DB_FIELD_INT:
				s = wxT("INT");
				break;
			case DB_FIELD_FLOAT:
				s = wxT("FLOAT");
				break;
			case DB_FIELD_DATE:
				s = wxT("DATE");
				break;
			case DB_FIELD_LIST:
				s = wxT("LIST");
				break;
			case DB_FIELD_BOOL:
				s = wxT("BOOL");
				break;
			case DB_FIELD_FLAGS:
				s = wxT("FLAGS");
				break;
			case DB_FIELD_DATETIME:
				s = wxT("DATETIME");
				break;

			default:
				s = wxT("?");
		}
		
		f.Write("<td>\n");
		f.Write(s);
		f.Write("</td>\n");

		s.sprintf("%d",fieldsDescription[i].len);
		f.Write("<td>\n");
		f.Write(s);
		f.Write("</td>\n");

		s.sprintf("%d",fieldsDescription[i].presition);
		f.Write("<td>\n");
		f.Write(s);
		f.Write("</td>\n");
		for( int j =0; j < (int)lov.Count(); j++ )
		{
			if( fieldsDescription[i].lov == &(lov[j]) )
			{
					s.sprintf("%d",j);
					f.Write("<td>\n");
					f.Write(s);
					f.Write("</td>\n");

				break;
			};

		}
		if( j >= (int)lov.Count())
		{
					f.Write("<td>\n");
					f.Write(wxT("-"));
					f.Write("</td>\n");
		}

		s.sprintf("%d",fieldsDescription[i].minLen);
		f.Write("<td>\n");
		f.Write(s);
		f.Write("</td>\n");
	

		f.Write("</tr>\n");


	};

	f.Write("</tbody>\n");
	f.Write("</table>\n");
	f.Write("<td>\n");
	f.Write("<br>\n" );
	f.Write(wxT("LOVs:"));
	f.Write("</td>\n");
	for(  j =0; j < (int)lov.Count(); j++ )
	{


		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
		f.Write("№:\n");
		f.Write("<br>\n </div> \n");
		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
		s.sprintf("%d",j);
		f.Write(s);
		f.Write("<br>\n </div> \n");

		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
		f.Write("Query:\n");
		f.Write("<br>\n </div> \n");
		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
		s = lov[j].query;
		ProcessStr(s);
		f.Write(s);
		f.Write("<br>\n </div> \n");

		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
		f.Write("SelectClause:\n");
		f.Write("<br>\n </div> \n");
		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
		s = lov[j].selectClause;
		ProcessStr(s);
		f.Write(s);
		f.Write("<br>\n </div> \n");


		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
		f.Write("FromClause:\n");
		f.Write("<br>\n </div> \n");
		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
		s = lov[j].fromClause;
		ProcessStr(s);
		f.Write(s);
		f.Write("<br>\n </div> \n");

		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
		f.Write("WhereClause:\n");
		f.Write("<br>\n </div> \n");
		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
		s = lov[j].whereClause;
		ProcessStr(s);
		f.Write(s);
		f.Write("<br>\n </div> \n");

		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
		f.Write("OrderClause:\n");
		f.Write("<br>\n </div> \n");
		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: normal; font-size: 8pt;\">\n");
		s = lov[j].orderClause;
		ProcessStr(s);
		f.Write(s);
		f.Write("<br>\n </div> \n");

	

		f.Write("<div style=\"text-align: left;\"><span style=\"font-weight: bold; font-size: 12pt;\">\n");
		f.Write("LOVColumns:\n");
		f.Write("<br>\n </div> \n");		

		f.Write("<table style=\"text-align: center; width: 28cm; font-size: 8pt; font-family: arial;  font-style: normal;\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\">\n");
		f.Write("<tbody>\n");
		f.Write("<tr>\n");
		f.Write("<td>\n");
		f.Write("№\n");
		f.Write("</td>\n");

		f.Write("<td>\n");
		f.Write("№ field LOV\n");
		f.Write("</td>\n");

		f.Write("<td>\n");
		f.Write("№ field Grid\n");
		f.Write("</td>\n");

		f.Write("<td>\n");
		f.Write("Width\n");
		f.Write("</td>\n");

		f.Write("<td>\n");
		f.Write("Column Name\n");
		f.Write("</td>\n");

	
		f.Write("</tr>\n");
		for ( int k = 0; k < (int)lov[j].pair.Count(); k++)
		{
			s.sprintf("%d\n",k);
			f.Write("<tr>\n");
		
			f.Write("<td>\n");
			f.Write(s);
			f.Write("</td>\n");
		
			f.Write("<td>\n");
			s.sprintf("%d\n",lov[j].pair[k].i1);
			f.Write(s);
			f.Write("</td>\n");

			f.Write("<td>\n");
			s.sprintf("%d\n",lov[j].pair[k].i2);
			f.Write(s);
			f.Write("</td>\n");

			f.Write("<td>\n");
			s.sprintf("%d\n",lov[j].pair[k].w);
			f.Write(s);
			f.Write("</td>\n");

			f.Write("<td>\n");
			f.Write(lov[j].pair[k].fieldName);
			f.Write("</td>\n");


		};


		f.Write("</tbody>\n");
		f.Write("</table>\n");

	}


	f.Write("</body>\n");
	f.Write("</html>\n");
	
	f.Close();
	if( !wxLaunchDefaultBrowser( fn, wxBROWSER_NEW_WINDOW ) )
	{
		wxMessageBox(TEXT_IS_BROWSER_ERROR);
	}

};




//Замена непечатных символов на обозначения для HTML
void wxDbTreeGrid::ProcessStr( wxString &s )
{

		s.Replace(wxT("<"), wxT("&lt;") );
		s.Replace(wxT(">"), wxT("&gt;") );

};


///Редактирование двоичного поля
void wxDbTreeGrid::EditBool(const wxTreeItemId & id, int col)
{

	wxTreeItemId eId;
	if( GetItemImage( id, col ) == 0 )
	{
		SetItemImage( id, col, 1 );
	}
	else
	{
		SetItemImage( id, col, 0 );
	}
	eId = id;
	wxTreeEvent event;
	event.SetItem(eId);
	//SetFlagsValue(eId,DB_UPDATED_RECORD);
	OnEndEdit(event);
	
}

//Установка ширины поля по имени
bool wxDbTreeGrid::SetColumnWidthByName( wxString fn, int val)
{
	int i = GetFieldIndex( fn );
	if( i < 0 )
	{
		return false;
	}
	else 
	{
		SetColumnWidth( i, val );
	}
	return true;
}

BEGIN_EVENT_TABLE( wxDbTreeGrid, wxDbTreeListCtrl )
	EVT_MENU( ID_DELETE,wxDbTreeGrid::OnDeleteRecord )
	EVT_MENU( ID_INSERT,wxDbTreeGrid::OnInsertRecord )
	EVT_MENU( ID_SAVE,wxDbTreeGrid::OnSaveRecord )
	EVT_MENU( ID_REFRESH,wxDbTreeGrid::OnRefreshRecord )
	EVT_MENU( ID_QUERY,wxDbTreeGrid::OnQueryRecord )
	EVT_MENU( ID_SEARCH,wxDbTreeGrid::OnSearchRecord )
	EVT_MENU( ID_INSERT_CHILD,wxDbTreeGrid::OnInsertChildRecord )
    EVT_MENU( ID_SEARCH_NEXT,wxDbTreeGrid::OnSearchRecordNext )
	EVT_MENU( ID_SHOW_PROPERTY,wxDbTreeGrid::OnShowProperty )
	EVT_MENU( ID_DUBLICATE_RECORD,wxDbTreeGrid::OnDuplicateRecord )
	EVT_MENU( ID_EXTERN_EDITOR,wxDbTreeGrid::OnExternEditor )
	

	EVT_RIGHT_DOWN(wxDbTreeGrid::OnMouseRightClick)
	EVT_TREE_SEL_CHANGED(wxID_ANY, wxDbTreeGrid::OnSelectItem)
	EVT_KEY_DOWN(wxDbTreeGrid::OnKeyDown) 
	EVT_CHAR(wxDbTreeGrid::OnKey) 
	EVT_TREE_END_LABEL_EDIT(wxID_ANY, OnEndEdit)
	EVT_LEFT_DCLICK(wxDbTreeGrid::OnMouseDoubleClick) 
	EVT_FIELD_CHANGED(wxID_ANY,wxDbTreeGrid::OnwxDbFieldChanged)
	EVT_FIELD_CMND(wxID_ANY,wxDbTreeGrid::OnwxDbFieldCmnd)
	EVT_SET_FOCUS(wxDbTreeGrid::OnSetFocus)

	EVT_TREE_BEGIN_RDRAG(wxID_ANY, wxDbTreeGrid::OnBeginDrag)
	EVT_TREE_BEGIN_DRAG(wxID_ANY, wxDbTreeGrid::OnBeginDrag)
	EVT_TREE_END_DRAG(wxID_ANY, wxDbTreeGrid::OnEndDrag)
	EVT_TREE_ITEM_EXPANDED(wxID_ANY, OnExpanded)
//	EVT_TEXT_ENTER(wxID_ANY, OnTextEnter) 	
  //  EVT_PAINT         (wxDbTreeGrid::OnPaint)
END_EVENT_TABLE()
