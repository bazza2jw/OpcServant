/////////////////////////////////////////////////////////////////////////////
// Name:        customlovdialog_init.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/customlovdialog.h"
#include "../include/dbdispatcher.h"
//#include "setup.h"




bool CustomLOVDialog::Init(DatabaseLayer* d)
{
	

	SetLabel(wxT(""));
	db = d;
	tree->db=d;
/*	try{
		db = new FirebirdDatabaseLayer(DB_SERVER,DB_NAME,USER_NAME,USER_PASSWORD);

	}
	catch (DatabaseLayerException& e)
	{
		wxMessageBox(wxString::Format(_("Error (%d): %s"), e.GetErrorCode(), e.GetErrorMessage().c_str()));
	}
	

	tree->db=db;
*/
	return true;

};

void CustomLOVDialog::OnClose(wxCloseEvent &event){
	
	EndModal(-1); 

//	Destroy();

}

void CustomLOVDialog::OnMouseDoubleClick(wxMouseEvent& event)
{

		if(((wxWindow*)event.GetEventObject())->GetParent()==tree && tree->GetSelection().IsOk()) EndModal(wxID_OK); 
};

void CustomLOVDialog::OnKey(wxKeyEvent& event)
{
	
		if(((wxWindow*)event.GetEventObject())->GetParent()==tree && tree->GetSelection().IsOk()) EndModal(wxID_OK); 
};

BEGIN_EVENT_TABLE( CustomLOVDialog, wxDialog )
//	EVT_CLOSE(PersonDialog::OnClose) 
	EVT_LEFT_DCLICK(CustomLOVDialog::OnMouseDoubleClick) 
	EVT_CHAR(CustomLOVDialog::OnKey) 
	EVT_KEY_DOWN(CustomLOVDialog::OnKey) 

END_EVENT_TABLE()
  