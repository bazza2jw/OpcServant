///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dbedittextdialog__
#define __dbedittextdialog__

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DbEditTextDialog
///////////////////////////////////////////////////////////////////////////////
class DbEditTextDialog : public wxDialog 
{
	private:
	
	protected:
	
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
	
	public:
		
		DbEditTextDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 417,346 ), long style = wxDEFAULT_DIALOG_STYLE );
		~DbEditTextDialog();
		wxTextCtrl* editor;
};

#endif //__dbedittextdialog__
