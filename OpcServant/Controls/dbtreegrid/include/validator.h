/////////////////////////////////////////////////////////////////////////////
// Name:        valtext.h
// Purpose:     wxTextValidator class
// Author:      Julian Smart
// Modified by: Anatoly Zamyatin
// Created:     29/01/98
// RCS-ID:      $Id: valtext.h 61872 2009-09-09 22:37:05Z VZ $
// Copyright:   (c) 1998 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _validator__
#define _validator__

#include <wx/valtext.h>
class WXDLLEXPORT StrValidator: public wxTextValidator
{
DECLARE_DYNAMIC_CLASS(StrValidator)
public:
	StrValidator(const wxString &s);
    //StrValidator(long style = wxFILTER_NONE, wxString *val = 0);
    StrValidator(const StrValidator& val);
	StrValidator():wxTextValidator(){};
    virtual ~StrValidator(){}

    // Make a clone of this validator (or return NULL) - currently necessary
    // if you're passing a reference to a validator.
    // Another possibility is to always pass a pointer to a new validator
    // (so the calling code can use a copy constructor of the relevant class).
    virtual wxObject *Clone() const { return new StrValidator(*this); }
    bool Copy(const StrValidator& val){
		wxTextValidator::Copy(val);
		characters=val.characters;
		return true;
	}

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow *parent);


    // Filter keystrokes
    void OnChar(wxKeyEvent& event);

	wxString characters;
DECLARE_EVENT_TABLE()


private:
// Cannot use
//  DECLARE_NO_COPY_CLASS(wxTextValidator)
// because copy constructor is explicitly declared above;
// but no copy assignment operator is defined, so declare
// it private to prevent the compiler from defining it:
    wxTextValidator& operator=(const wxTextValidator&);
};


class WXDLLEXPORT DateTimeValidator: public wxTextValidator
{
DECLARE_DYNAMIC_CLASS(DateTimeValidator)
public:
	DateTimeValidator(const wxString &s);
    //StrValidator(long style = wxFILTER_NONE, wxString *val = 0);
    DateTimeValidator(const DateTimeValidator& val);
	DateTimeValidator():wxTextValidator(){};
    virtual ~DateTimeValidator(){}

    // Make a clone of this validator (or return NULL) - currently necessary
    // if you're passing a reference to a validator.
    // Another possibility is to always pass a pointer to a new validator
    // (so the calling code can use a copy constructor of the relevant class).
    virtual wxObject *Clone() const { return new DateTimeValidator(*this); }
    bool Copy(const DateTimeValidator& val){
		wxTextValidator::Copy(val);
		characters=val.characters;
		return true;
	}

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow *parent);


    // Filter keystrokes
    void OnChar(wxKeyEvent& event);

	wxString characters;
DECLARE_EVENT_TABLE()


private:
// Cannot use
//  DECLARE_NO_COPY_CLASS(wxTextValidator)
// because copy constructor is explicitly declared above;
// but no copy assignment operator is defined, so declare
// it private to prevent the compiler from defining it:
    wxTextValidator& operator=(const wxTextValidator&);
};



extern StrValidator snvalidator;
extern DateTimeValidator dtvalidator;
#endif
  // _WX_VALTEXTH__
