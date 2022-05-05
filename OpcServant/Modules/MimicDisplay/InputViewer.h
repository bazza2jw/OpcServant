#ifndef INPUTVIEWER_H
#define INPUTVIEWER_H
#include "MimicDisplayUI.h"
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/bobject.h>
#include "InputViewerHistory.h"

class InputViewer : public InputViewerBase, public MRL::BObject
{
    static constexpr int MaxHistorySize = 1000;
    std::map<std::string,InputViewerHistory *> _history;
public:
    InputViewer(wxWindow* parent, unsigned id);
    virtual ~InputViewer();
    bool processQueueItem(const MRL::Message &msg);
protected:
    virtual void DriveProcess(wxTimerEvent& event);
};
#endif // INPUTVIEWER_H
