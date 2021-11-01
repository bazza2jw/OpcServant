#include "t02.h"
#include "../include/dbpopupmenu.h"
#include "../../databaselayer/include/FirebirdDatabaseLayer.h"
//wxLogNull lnull;//Подавляем вывод логов (wxLogMessage) !!!
IMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
	
	frame = new MyFrame((wxFrame*) NULL, -1, wxEmptyString );

	frame->Show(TRUE);
	frame->SetStatusText( wxT( "Left mouse botton show command menu" ) );
		//Создаем меню для редактирования данных
	createDbMenu();
	try
	{
		//Диалог соединения с БД
		dbase = new FirebirdDatabaseLayer(wxT("127.0.0.1"),wxT("c:\\dbtreegrid\\sample.fdb"),wxT("sysdba"),wxT("masterkey"));
	}
	catch (DatabaseLayerException& e)
	{
		wxMessageBox(wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));

	}
	return true;
}

