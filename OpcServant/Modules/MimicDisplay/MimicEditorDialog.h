#ifndef MIMICEDITORDIALOG_H
#define MIMICEDITORDIALOG_H
#include "MimicDisplayUI.h"
#include <Mimic/Mimic/MimicEditorPanel.h>

class MimicEditorDialog : public MimicEditorDialogBase
{
public:
    MimicEditorDialog(wxWindow* parent, const std::string &f);
    virtual ~MimicEditorDialog();
protected:
    virtual void onTimer(wxTimerEvent& event);
    virtual void onOk(wxCommandEvent& event);
};
#endif // MIMICEDITORDIALOG_H
