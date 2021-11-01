/////////////////////////////////////////////////////////////////////////////
// Name:        editctrl.h
// Purpose:     Controls for edit data in wxDbTreeGrid
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#ifndef __editctrl__
#define __editctrl__

#include "dbtreelistctrl.h"
#include <wx/datectrl.h>
#include <wx/listbox.h>
#include <wx/combobox.h>
class wxDbGridFieldDesc;
/**
@class wxDbEditCtrl
Виртуальный базовый класс для объектов редактирования в гриде
*/
///Виртуальный базовый класс для объектов редактирования в гриде
class wxDbEditCtrl {
public:
	wxDbEditCtrl(){};
	virtual  void EndEdit(bool isCancelled){};
	virtual void SetOwner(wxDbTreeListMainWindow *owner) {};
	virtual void SetFocus(){};
	virtual void Show(){};
	virtual bool Destroy(){return true;};

};



//wxDatePickerCtrl(wxWindow *parent, wxWindowID id, const wxDateTime& dt = wxDefaultDateTime, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDP_DEFAULT | wxDP_SHOWCENTURY, const wxValidator& validator = wxDefaultValidator, const wxString& name = "datectrl")

/**
@class wxDbEditTextCtrl
Класс редактирования текста с проверкой введенных данных  в гриде
*/

///Класс редактирования текста с проверкой введенных данных  в гриде
class  wxDbEditTextCtrl: public wxTextCtrl, public wxDbEditCtrl
{
public:
    wxDbEditTextCtrl (wxWindow *parent,
                    const wxWindowID id,
                    bool *accept,
                    wxString *res,
                    wxDbTreeListMainWindow *owner,
                    const wxString &value = wxEmptyString,
                    const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize,
                    int style = 0,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString &name = wxTextCtrlNameStr );
    ~wxDbEditTextCtrl();

    virtual bool Destroy();  // wxWindow override
    virtual void EndEdit(bool isCancelled);
    void SetOwner(wxDbTreeListMainWindow *owner) { m_owner = owner; }
	virtual void SetFocus(){wxTextCtrl::SetFocus();};
    void OnChar( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnKillFocus( wxFocusEvent &event );
	void OnEnter(wxCommandEvent &event);
	wxDbGridFieldDesc *fdesc;
private:
    wxDbTreeListMainWindow  *m_owner;
    bool               *m_accept;
    wxString           *m_res;
    wxString            m_startValue;
    bool                m_finished;  // true==deleting, don't process events anymore

    DECLARE_EVENT_TABLE()
};

/**
@class wxDbEditListCtrl
Редактор выпадающий список  в гриде
*/
///Редактор выпадающий список  в гриде

class  wxDbEditListCtrl: public wxChoice, public wxDbEditCtrl
{
public:






    wxDbEditListCtrl (wxWindow *parent,
					wxWindowID id,
					bool *accept,
                    wxString *res,
                    wxDbTreeListMainWindow *owner,
					const wxString &value,
					const wxPoint& pos, 
					const wxSize& size, 
					wxArrayString& choices, 
					const wxArrayString& values, 
					long style = 0, 
					const wxValidator& validator = wxDefaultValidator, 
					const wxString& name = "listBox");
    ~wxDbEditListCtrl();

    virtual bool Destroy();  // wxWindow override
    virtual void EndEdit(bool isCancelled);
    void SetOwner(wxDbTreeListMainWindow *owner) { m_owner = owner; }
	virtual void SetFocus(){wxChoice::SetFocus();};
    void OnChar( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnKillFocus( wxFocusEvent &event );
	void OnSelect( wxCommandEvent &event );
	virtual void Show(){wxChoice::Show();};
	wxDbGridFieldDesc *fdesc;


private:
    wxDbTreeListMainWindow  *m_owner;
    bool               *m_accept;
    wxString           *m_res;
    wxString            m_startValue;
    bool                m_finished;  // true==deleting, don't process events anymore
	wxArrayString val;
	wxArrayString *v;
    DECLARE_EVENT_TABLE()
};

/**
@class wxDbEditDateCtrl
Редактор даты  в гриде
*/
///Редактор даты  в гриде

class  wxDbEditDateCtrl: public wxDatePickerCtrl, public wxDbEditCtrl
{
public:
    wxDbEditDateCtrl (wxWindow *parent,
					wxWindowID id,
					bool *accept,
                    wxString *res,
                    wxDbTreeListMainWindow *owner,
					const wxDateTime& dt = wxDefaultDateTime,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxDP_DEFAULT | wxDP_SHOWCENTURY,
					const wxValidator& validator = wxDefaultValidator,
					const wxString& name = "datectrl");
    ~wxDbEditDateCtrl();

    virtual bool Destroy();  // wxWindow override
    virtual void EndEdit(bool isCancelled);
    void SetOwner(wxDbTreeListMainWindow *owner) { m_owner = owner; }
	virtual void SetFocus(){wxDatePickerCtrl::SetFocus();};
    void OnChar( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnKillFocus( wxFocusEvent &event );
	wxDbGridFieldDesc *fdesc;
private:
    wxDbTreeListMainWindow  *m_owner;
    bool               *m_accept;
    wxString           *m_res;
    wxString            m_startValue;
    bool                m_finished;  // true==deleting, don't process events anymore

    DECLARE_EVENT_TABLE()
};


/**
@class wxDbEditTextControl
Родной класс  редактирования из wxTreeListCtrl
*/
///Родной класс  редактирования из wxTreeListCtrl

class  wxDbEditTextControl: public wxTextCtrl, public wxDbEditCtrl
{
public:
    wxDbEditTextControl (wxWindow *parent,
                    const wxWindowID id,
                    bool *accept,
                    wxString *res,
                    wxDbTreeListMainWindow *owner,
                    const wxString &value = wxEmptyString,
                    const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize,
                    int style = 0,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString &name = wxTextCtrlNameStr );
    ~wxDbEditTextControl();

    virtual bool Destroy();  // wxWindow override
    virtual void EndEdit(bool isCancelled);
    void SetOwner(wxDbTreeListMainWindow *owner) { m_owner = owner; }
	virtual void SetFocus(){wxTextCtrl::SetFocus();};
    void OnChar( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnKillFocus( wxFocusEvent &event );


private:
    wxDbTreeListMainWindow  *m_owner;
    bool               *m_accept;
    wxString           *m_res;
    wxString            m_startValue;
    bool                m_finished;  // true==deleting, don't process events anymore

    DECLARE_EVENT_TABLE()
};


#endif