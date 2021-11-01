/////////////////////////////////////////////////////////////////////////////
// Name:        validator.cpp
// Purpose:     
// Author:      Anatoly Zamyatin
// Created:     01/08/2010
// Licence:     wxWindows
/////////////////////////////////////////////////////////////////////////////

#include "../include/validator.h"
#include <wx/log.h>

StrValidator snvalidator(wxT("0123456789QWERTYUIOPASDFGHJKLZXCVBNM-"));

DateTimeValidator dtvalidator(wxT("0123456789"));

IMPLEMENT_DYNAMIC_CLASS(StrValidator,wxTextValidator)

BEGIN_EVENT_TABLE(StrValidator,wxTextValidator)
    EVT_CHAR(StrValidator::OnChar)
END_EVENT_TABLE()


StrValidator::StrValidator(const StrValidator& val)
    : wxTextValidator()
{
    Copy(val);
}

void StrValidator::OnChar(wxKeyEvent& event)
{
//	wxTextValidator::OnChar(event);
	int keyCode = event.GetKeyCode();
	wxString s((wxChar) keyCode,1);

	if(characters==wxEmptyString)
	{
		event.Skip();
	};
	if(keyCode < WXK_SPACE  ||  keyCode==WXK_LEFT || keyCode==WXK_RIGHT) event.Skip();
	if(wxNOT_FOUND==characters.Find((wxChar) keyCode)) return;
	//wxTextCtrl *control = (wxTextCtrl *) m_validatorWindow;
	event.Skip();
}

StrValidator::StrValidator(const wxString &s):
	wxTextValidator(wxFILTER_NONE, 0)
{
		characters=s;
};



bool StrValidator::Validate(wxWindow *parent)
{

	return true;
};
/*StrValidator::StrValidator(const StrValidator& val)
   : wxTextValidator()
{
	Copy(val);
};
*/
 
/////////////////////
IMPLEMENT_DYNAMIC_CLASS(DateTimeValidator,wxTextValidator)

BEGIN_EVENT_TABLE(DateTimeValidator,wxTextValidator)
    EVT_CHAR(DateTimeValidator::OnChar)
END_EVENT_TABLE()


DateTimeValidator::DateTimeValidator(const DateTimeValidator& val)
    : wxTextValidator()
{
    Copy(val);
}


void DateTimeValidator::OnChar(wxKeyEvent& event)
{
//	wxTextValidator::OnChar(event);
	wxString s1,s2;
	int keyCode = event.GetKeyCode();
	wxString s((wxChar) keyCode,1);

	wxTextCtrl *control = (wxTextCtrl *) m_validatorWindow;
	if(!control) return;
	control->SetMaxLength(19);

	int pos= control->GetInsertionPoint();
	if(keyCode==WXK_LEFT && (pos==3 || (pos==6) || (pos==11) || (pos==14) || (pos==17)) )
	{
		control->SetInsertionPoint(pos-2);
		return;
	};
	if(keyCode==WXK_RIGHT && (pos==1 || (pos==4) || (pos==9) || (pos==12) || (pos==15)) )
	{
		control->SetInsertionPoint(pos+2);
		return;
	};

	if(keyCode==WXK_RIGHT || keyCode==WXK_LEFT){event.Skip(); return;}
	if(keyCode==WXK_RETURN) {event.Skip(); return;}
	//if(keyCode < WXK_SPACE){event.Skip(); return;}
	wxString ss=control->GetValue();
	wxString sss;
	sss.sprintf("%d",pos);

	
	if(!(wxNOT_FOUND==characters.Find((wxChar) keyCode)))
	{
		control->Replace(pos,pos+1,s);
		ss=control->GetValue();

		//control->SetValue(ss);
		//if(pos==18){control->SetInsertionPoint(pos); return;}
		if(pos==1 || pos==4 || pos==9 || pos==12 || pos==15)
		{
			s1 = ss.Mid( pos+1, 1 );
			if( s1 != wxT(".") && ( pos == 1 || pos == 4 ) )
			{
				s1 = ss.SubString( 0, pos +1 );
				s2 = ss.SubString( pos +1, 100 );
				s1.Append( wxT(".") );
				s1.Append( s2 );
				control->ChangeValue( s1 );
			}

			if( s1 != wxT(" ") && ( pos == 9 ) )
			{
				s1 = ss.SubString( 0, pos +1 );
				s2 = ss.SubString( pos +1, 100 );
				s1.Append( wxT(" ") );
				s1.Append( s2 );
				control->ChangeValue( s1 );
			}

			if( s1 != wxT(":") && ( pos == 12 || pos == 15 ) )
			{
				s1 = ss.SubString( 0, pos +1 );
				s2 = ss.SubString( pos +1, 100 );
				s1.Append( wxT(":") );
				s1.Append( s2 );
				control->ChangeValue( s1 );
			}


			control->SetInsertionPoint(pos+2);
		}
		else
		{
			control->SetInsertionPoint(pos+1);
		}
		
		return;
	};

	return;
}

DateTimeValidator::DateTimeValidator(const wxString &s):
	wxTextValidator(wxFILTER_NONE, 0)
{
		characters=s;
};

bool DateTimeValidator::Validate(wxWindow *parent)
{

	return true;
};



