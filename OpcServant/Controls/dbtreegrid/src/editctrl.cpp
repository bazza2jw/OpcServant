/////////////////////////////////////////////////////////////////////////////
// Name:        editctrl.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/editctrl.h"
#include "../include/dbstruct.h"
#include "../include/langdef.h"

//Ïðîâåðêà çíà÷åíèÿ ïîëÿ
extern bool CheckVal(const wxString& fname,const wxString& s, int valueType, int len =0, int presition=0,const wxString& maxVal=wxEmptyString,const wxString& minVal=wxEmptyString, bool notNull=false, int minLen = 0);


extern wxString FormatField(const wxString &ss,wxDbGridFieldDesc *fd);
//-----------------------------------------------------------------------------
// DbEditÅó÷åCtrl (internal)
//-----------------------------------------------------------------------------


BEGIN_EVENT_TABLE (wxDbEditTextCtrl,wxTextCtrl)
    EVT_CHAR           (wxDbEditTextCtrl::OnChar)
    EVT_KEY_UP         (wxDbEditTextCtrl::OnKeyUp)
    EVT_KILL_FOCUS     (wxDbEditTextCtrl::OnKillFocus)
	EVT_TEXT_ENTER(wxID_ANY,wxDbEditTextCtrl::OnEnter) 
END_EVENT_TABLE()


void wxDbEditTextCtrl::OnEnter(wxCommandEvent &event)
{

	if (m_finished)
    {
    
        return;
    }
    else
    {
        EndEdit(false);  // not cancelled
        return;
    }
	return;
};

wxDbEditTextCtrl::wxDbEditTextCtrl (wxWindow *parent,
                                const wxWindowID id,
                                bool *accept,
                                wxString *res,
                                wxDbTreeListMainWindow *owner,
                                const wxString &value,
                                const wxPoint &pos,
                                const wxSize &size,
                                int style,
                                const wxValidator& validator,
                                const wxString &name)
    : wxTextCtrl (parent, id, value, pos, size, style | wxSIMPLE_BORDER |wxTE_PROCESS_ENTER, validator, name)
{
    m_res = res;
    m_accept = accept;
    m_owner = owner;
    (*m_accept) = false;
    (*m_res) = wxEmptyString;
    m_startValue = value;
    m_finished = false;
}

wxDbEditTextCtrl::~wxDbEditTextCtrl() {
    EndEdit(true); // cancelled
}



void wxDbEditTextCtrl::EndEdit(bool isCancelled) {

	wxString s= GetValue();
	if( !CheckVal(fdesc->columnName,s,fdesc->valueType,fdesc->len,fdesc->presition,fdesc->maxVal,fdesc->minVal,fdesc->GetProperty()&DB_FIELD_NOT_NULL) ){m_finished = false; /*SetFocus();*/return;}
    if (m_finished) return;
    m_finished = true;
//	double d;
    if (m_owner) {
		s=FormatField(s,fdesc);

        (*m_accept) = ! isCancelled;
        (*m_res) = isCancelled ? m_startValue : s;
		m_owner->SetItemText(m_owner->m_editItem,m_owner->m_editCol,*m_res);
        m_owner->OnRenameAccept(*m_res == m_startValue);
        m_owner->m_editControl = NULL;
        m_owner->m_editItem = NULL;
        m_owner->SetFocus(); // This doesn't work. TODO.
        m_owner = NULL;
    }

    Destroy();
}

bool wxDbEditTextCtrl::Destroy() {
	return wxTextCtrl::Destroy();
    Hide();
    wxTheApp->GetTraits()->ScheduleForDestroy(this);
    return true;
}

void wxDbEditTextCtrl::OnChar( wxKeyEvent &event )
{
	wxString s;


    if (m_finished)
    {
        event.Skip();
        return;
    }
    if (event.GetKeyCode() == WXK_RETURN)
    {
        EndEdit(false);  // not cancelled
        return;
    }
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        EndEdit(true);  // cancelled
        return;
    }
	if (event.GetKeyCode() == WXK_TAB)
    {
        EndEdit(false);  // not cancelled
		//event.Skip();
        return;
    }
    event.Skip();
}

void wxDbEditTextCtrl::OnKeyUp( wxKeyEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    // auto-grow the textctrl:
    wxSize parentSize = m_owner->GetSize();
    wxPoint myPos = GetPosition();
    wxSize mySize = GetSize();
    int sx, sy;
    GetTextExtent(GetValue() + _T("M"), &sx, &sy);
    if (myPos.x + sx > parentSize.x) sx = parentSize.x - myPos.x;
    if (mySize.x > sx) sx = mySize.x;
    SetSize(sx, -1);

    event.Skip();
}

void wxDbEditTextCtrl::OnKillFocus( wxFocusEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    EndEdit(false);  // not cancelled
}


//-----------------------------------------------------------------------------
// wxDbEditListCtrl (internal)
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE (wxDbEditListCtrl,wxChoice)
    EVT_CHAR           (wxDbEditListCtrl::OnChar)
    EVT_KEY_UP         (wxDbEditListCtrl::OnKeyUp)
    EVT_KILL_FOCUS     (wxDbEditListCtrl::OnKillFocus)
	EVT_TEXT		(wxID_ANY, wxDbEditListCtrl::OnSelect ) 
 
END_EVENT_TABLE()

wxDbEditListCtrl::wxDbEditListCtrl ( wxWindow *parent,
					wxWindowID id,
					bool *accept,
                    wxString *res,
                    wxDbTreeListMainWindow *owner,
					const wxString &value,
					const wxPoint& pos, 
					const wxSize& size, 
					wxArrayString& choices, 
					const wxArrayString& values, 
					long style, 
					const wxValidator& validator, 
					const wxString& name )
	: wxChoice(parent,  id,   /*value, */  pos,   size,   choices,  style ,   validator  ,   name  )
{
    m_res = res;
    m_accept = accept;
    m_owner = owner;
    (*m_accept) = false;
    (*m_res) = wxEmptyString;
    m_startValue = value;
    m_finished = false;
	val=values;
	v=&choices;
}

wxDbEditListCtrl::~wxDbEditListCtrl() 
{
    EndEdit(true); // cancelled
}

void wxDbEditListCtrl::OnSelect( wxCommandEvent &event )
{

}


void wxDbEditListCtrl::EndEdit(bool isCancelled) {
//	int cs;
	
	//if( GetCurrentSelection()>=0 && GetCurrentSelection()<(int)(*v).Count() ) return;
    if (m_finished) return;
    m_finished = true;

    if (m_owner) {
        (*m_accept) = ! isCancelled;
		if( isCancelled )  (*m_res) =  m_startValue;
		else
		{
			if( GetCurrentSelection()>=0 && GetCurrentSelection()<(int)(*v).Count() ) (*m_res) = (*v)[GetCurrentSelection()];
		}

        //(*m_res) = isCancelled ? m_startValue : (*v)[GetCurrentSelection()];

		if( !isCancelled )
		{ 
			m_owner->SetItemText(m_owner->m_editItem,m_owner->m_editCol,*m_res);
			if( GetCurrentSelection()>=0 && GetCurrentSelection()<(int)val.Count() ) m_owner->SetItemText(m_owner->m_editItem,m_owner->m_editCol+1,val[GetCurrentSelection()]);
		};
        m_owner->OnRenameAccept(*m_res == m_startValue);
        m_owner->m_editControl = NULL;
        m_owner->m_editItem = NULL;
        m_owner->SetFocus(); // This doesn't work. TODO.
        m_owner = NULL;
    }

    Destroy();
}

bool wxDbEditListCtrl::Destroy() 
{
    Hide();
    wxTheApp->GetTraits()->ScheduleForDestroy(this);
    return true;
}

void wxDbEditListCtrl::OnChar( wxKeyEvent &event )
{

    if (m_finished)
    {
        event.Skip();
        return;
    }
    if (event.GetKeyCode() == WXK_RETURN)
    {
        EndEdit(false);  // not cancelled
        return;
    }
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        EndEdit(true);  // cancelled
        return;
    }
	if (event.GetKeyCode() == WXK_TAB)
    {
        EndEdit(false);  // not cancelled
		//event.Skip();
        return;
    }
    event.Skip();
}

void wxDbEditListCtrl::OnKeyUp( wxKeyEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    // auto-grow the textctrl:
    wxSize parentSize = m_owner->GetSize();
    wxPoint myPos = GetPosition();
    wxSize mySize = GetSize();
    int sx, sy;
	wxString s;
	if( GetCurrentSelection()>=0 && GetCurrentSelection()<(int)(*v).Count() ) s = (*v)[GetCurrentSelection()];
    GetTextExtent(s+_T("M"), &sx, &sy);
    if (myPos.x + sx > parentSize.x) sx = parentSize.x - myPos.x;
    if (mySize.x > sx) sx = mySize.x;
    SetSize(sx, -1);

    event.Skip();
}

void wxDbEditListCtrl::OnKillFocus( wxFocusEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    EndEdit(false);  // not cancelled
}


//-----------------------------------------------------------------------------
// wxDbEditDateCtrl (internal)
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE (wxDbEditDateCtrl,wxDatePickerCtrl)
    EVT_CHAR           (wxDbEditDateCtrl::OnChar)
    EVT_KEY_UP         (wxDbEditDateCtrl::OnKeyUp)
    EVT_KILL_FOCUS     (wxDbEditDateCtrl::OnKillFocus)
END_EVENT_TABLE()

wxDbEditDateCtrl::wxDbEditDateCtrl (wxWindow *parent,
					wxWindowID id,
					bool *accept,
                    wxString *res,
                    wxDbTreeListMainWindow *owner,
					const wxDateTime& dt,
					const wxPoint& pos ,
					const wxSize& size ,
					long style ,
					const wxValidator& validator ,
					const wxString& name  )
    : wxDatePickerCtrl (parent, id, dt, pos, size, style | wxSIMPLE_BORDER, validator, name)
{
    m_res = res;
    m_accept = accept;
    m_owner = owner;
    (*m_accept) = false;
    (*m_res) = wxEmptyString;
    m_startValue = dt.Format(DATE_FORMAT);
    m_finished = false;
}

wxDbEditDateCtrl::~wxDbEditDateCtrl() {
    EndEdit(true); // cancelled
}

void wxDbEditDateCtrl::EndEdit(bool isCancelled) {
    if (m_finished) return;
    m_finished = true;

    if (m_owner) {
        (*m_accept) = ! isCancelled;
		if(fdesc->valueType!=DB_FIELD_DATETIME)
		{
			(*m_res) = isCancelled ? m_startValue : GetValue().Format(DATE_FORMAT);

		}
		else
		{
			(*m_res) = isCancelled ? m_startValue : GetValue().Format(DATETIME_FORMAT);

		}

		if( !isCancelled ) m_owner->SetItemText(m_owner->m_editItem,m_owner->m_editCol,*m_res);
        m_owner->OnRenameAccept(*m_res == m_startValue);
        m_owner->m_editControl = NULL;
        m_owner->m_editItem = NULL;
        m_owner->SetFocus(); // This doesn't work. TODO.
        m_owner = NULL;
    }

    Destroy();
}

bool wxDbEditDateCtrl::Destroy() {
    Hide();
    wxTheApp->GetTraits()->ScheduleForDestroy(this);
    return true;
}

void wxDbEditDateCtrl::OnChar( wxKeyEvent &event )
{
	
    if (m_finished)
    {
        event.Skip();
        return;
    }
    if (event.GetKeyCode() == WXK_RETURN)
    {
        EndEdit(false);  // not cancelled
        return;
    }
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        EndEdit(true);  // cancelled
        return;
    }
	if (event.GetKeyCode() == WXK_TAB)
    {
        EndEdit(false);  // not cancelled
		//event.Skip();
        return;
    }
    event.Skip();
}

void wxDbEditDateCtrl::OnKeyUp( wxKeyEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    // auto-grow the textctrl:
    wxSize parentSize = m_owner->GetSize();
    wxPoint myPos = GetPosition();
    wxSize mySize = GetSize();
    int sx, sy;
    GetTextExtent(GetValue().FormatDate() + _T("M"), &sx, &sy);
    if (myPos.x + sx > parentSize.x) sx = parentSize.x - myPos.x;
    if (mySize.x > sx) sx = mySize.x;
    SetSize(sx, -1);

    event.Skip();
}

void wxDbEditDateCtrl::OnKillFocus( wxFocusEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    EndEdit(false);  // not cancelled
}


