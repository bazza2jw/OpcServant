#ifndef GRIDPANEL_H
#define GRIDPANEL_H
#include "BourbonCommonUI.h"
#include <Common/Daq/daqcommon.h>
#include <Common/common.h>
class GridPanel : public GridPanelBase
{
    unsigned _id = 0;

public:
    GridPanel(wxWindow* parent, unsigned id);
    virtual ~GridPanel();
    MRL::RtObjectRef & object()
    {
         return MRL::Common::daq().objects()[_id];
    }
    virtual void setupTable();
    virtual void updateTable();
    void setStatus(int r, const std::string &item);

protected:
    virtual void onTimer(wxTimerEvent& event);
};
#endif // GRIDPANEL_H
