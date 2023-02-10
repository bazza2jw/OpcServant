#ifndef SECONDREPORTPANEL_H
#define SECONDREPORTPANEL_H
#include "CommonGUI.h"

class SecondReportPanel : public SecondReportPanelBase
{
public:
    SecondReportPanel(wxWindow* parent);
    virtual ~SecondReportPanel();
protected:
    virtual void OnNew(wxCommandEvent& event);
    virtual void OnTimeframeChoice(wxCommandEvent& event);
    virtual void onExport(wxCommandEvent& event);
    virtual void onLoadReport(wxCommandEvent& event);
    virtual void onMakeReport(wxCommandEvent& event);
    virtual void onPrint(wxCommandEvent& event);
    virtual void onRemoveReport(wxCommandEvent& event);
    virtual void onSaveReport(wxCommandEvent& event);
    virtual void onRefreshValues(wxCommandEvent& event);
};
#endif // SECONDREPORTPANEL_H
