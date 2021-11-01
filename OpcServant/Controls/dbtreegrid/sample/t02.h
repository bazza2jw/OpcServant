#ifndef __t02__
#define __t02__




#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "../include/db.h"
#include "myframe.h"

#define DATE_FORMAT  wxT("%d.%m.%Y")	
#define DATETIME_FORMAT  wxT("%d.%m.%Y %H:%M:%S")	
#define FIREBIRD_DB


class MyApp : public wxApp
{


	public:
		bool OnInit();
		MyFrame *frame;
	
};
DECLARE_APP(MyApp);



#endif