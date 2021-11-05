#ifndef WEBLOGINDIALOG_H
#define WEBLOGINDIALOG_H
#include <Common/Web/webdialog.h>
// use local username password for authentication

namespace MRL
{
    class WebLoginDialog :  public Wt::WContainerWidget
    {
        Wt::WLineEdit * _name = nullptr;
        Wt::WLineEdit * _password = nullptr;
        Wt::WToolBar * _toolBar = nullptr;
        bool _result = false;
    public:
        WebLoginDialog();
        void setup();
        void onOk();
        void onCancel();
        bool result() const { return _result;}
    };
}

#endif // WEBLOGINDIALOG_H
