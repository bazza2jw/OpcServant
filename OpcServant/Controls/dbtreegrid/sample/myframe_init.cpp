#include "myframe.h"

#include "person.h"
#include "appointdialog.h"
#include "deptdialog.h"
#include "worker.h"
#include "test.h"

void MyFrame::Init()
{		

}


void MyFrame::OnPeople( wxCommandEvent& event )
{

	
	PersonDialog *d =new PersonDialog(this);
	if(!d->Init()) d->Destroy();
	else d->Show();

};


void MyFrame::OnAppointment( wxCommandEvent& event )
{

	
	AppointDialog *d =new AppointDialog(this);
	if(!d->Init()) d->Destroy();
	else d->Show();

};

void MyFrame::OnDepartment( wxCommandEvent& event )
{
	
	DeptDialog *d =new DeptDialog(this);
	if(!d->Init()) d->Destroy();
	else d->Show();

};

void MyFrame::OnWorker( wxCommandEvent& event ) 
{
	WorkerDialog *d =new WorkerDialog(this);
	if(!d->Init()) d->Destroy();
	else d->Show();

};

void MyFrame::OnTest( wxCommandEvent& event ) 
{
	TestFieldDialog *d =new TestFieldDialog(this);
	if(!d->Init()) d->Destroy();
	else d->Show();

};