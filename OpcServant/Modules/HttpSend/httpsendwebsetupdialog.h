#ifndef HTTPSENDWEBSETUPDIALOG_H
#define HTTPSENDWEBSETUPDIALOG_H
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>

namespace MRL {
    class HttpSendWebSetupDialog   : public WebDialogBase {
            PropertyPath _path;
            Wt::WLineEdit * _filter = nullptr;
            Wt::WLineEdit * _url = nullptr;
            Wt::WLineEdit * _host = nullptr;
            Wt::WSpinBox * _port = nullptr;
            Wt::WCheckBox * _enabled = nullptr;
        public:
            HttpSendWebSetupDialog(const PropertyPath &path) : _path(path) {}
            void setup();
            void onOk();
    };
}

#endif // HTTPSENDWEBSETUPDIALOG_H
