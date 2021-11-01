#ifndef HTTPSENDSETUPDIALOG_H
#define HTTPSENDSETUPDIALOG_H
#include "HttpSendUI.h"
#include <MrlLib/variantpropertytree.h>

class HttpSendSetupDialog : public HttpSendSetupDialogBase
{
    MRL::PropertyPath _path;
public:
    HttpSendSetupDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~HttpSendSetupDialog();
protected:
    virtual void OnOk(wxCommandEvent& event);
};
#endif // HTTPSENDSETUPDIALOG_H
