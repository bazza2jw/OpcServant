#ifndef FLOWEDITORDIALOG_H
#define FLOWEDITORDIALOG_H
#include "NodeFlowUI.h"

class FlowEditorDialog : public FlowEditorDialogBase
{
    std::string _path;
public:
    FlowEditorDialog(wxWindow* parent, const wxString &path);
    virtual ~FlowEditorDialog();
protected:
    virtual void onOk(wxCommandEvent& event);
};
#endif // FLOWEDITORDIALOG_H
