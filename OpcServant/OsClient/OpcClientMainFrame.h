#ifndef OPCCLIENTMAINFRAME_H
#define OPCCLIENTMAINFRAME_H
#include "OsClientUI.h"

class OpcClientMainFrame : public OpcClientMainFrameBase
{
public:
    OpcClientMainFrame(wxWindow* parent);
    virtual ~OpcClientMainFrame();
protected:
    virtual void onClearMessages(wxCommandEvent& event);
    virtual void OnExit(wxCommandEvent& event);
    virtual void OnOpen(wxCommandEvent& event);
    virtual void OnRestore(wxCommandEvent& event);
    virtual void OnSave(wxCommandEvent& event);
    virtual void OnSaveAs(wxCommandEvent& event);
    virtual void OnSetup(wxCommandEvent& event);
    virtual void onAddReportDataSource(wxCommandEvent& event);
    virtual void onMakeReport(wxCommandEvent& event);
    virtual void onNewReport(wxCommandEvent& event);
    virtual void onRemoveReport(wxCommandEvent& event);
    virtual void onRemoveReportDataSource(wxCommandEvent& event);
    virtual void onReportSelect(wxCommandEvent& event);
    virtual void onViewReport(wxCommandEvent& event);
};
#endif // OPCCLIENTMAINFRAME_H
