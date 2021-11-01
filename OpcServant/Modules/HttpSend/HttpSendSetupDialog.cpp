#include "HttpSendSetupDialog.h"
#include <Common/common.h>
HttpSendSetupDialog::HttpSendSetupDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : HttpSendSetupDialogBase(parent),
      _path(path)
{
    // get the configuration from database
    Json::Value v;
    if (MRL::Common::configuration().getData(path, v)) {
        // do not always have to convert to tree - esp if simple property list
        Json::Value &ht = v["Host"];
        Json::Value &pt = v["Port"];
        Json::Value &hs = v["URL"];
        Json::Value &db = v["Filter"];
        Json::Value &ei = v["Enabled"];
        //
        GetUrl()->SetValue(hs.asString());
        GetFilter()->SetValue(db.asString());
        GetEnable()->SetValue(ei.asBool());
        GetPort()->SetValue(pt.asInt());
        GetHost()->SetValue(ht.asString());
    }
}

HttpSendSetupDialog::~HttpSendSetupDialog()
{
}

void HttpSendSetupDialog::OnOk(wxCommandEvent& /*event*/)
{
    Json::Value v;
    //
    v["URL"] = GetUrl()->GetValue().ToStdString();
    v["Filter"] = GetFilter()->GetValue().ToStdString();
    v["Enabled"]  = GetEnable()->GetValue();
    v["Port"] = GetPort()->GetValue();
    v["Host"] = GetHost()->GetValue().ToStdString();
    //
    MRL::Common::configuration().updateData(_path, v);
    //
    EndModal(wxID_OK);
}
