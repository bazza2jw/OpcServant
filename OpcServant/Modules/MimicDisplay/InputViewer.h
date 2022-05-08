#ifndef INPUTVIEWER_H
#define INPUTVIEWER_H
#include "MimicDisplayUI.h"
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/bobject.h>
#include "InputViewerHistory.h"
#include <Common/reporter.h>
#include <Common/Daq/rtobject.h>

/*!
 * \brief The InputViewer class
 */
class InputViewer : public InputViewerBase, public MRL::BObject
{
    static int _rId;
    static constexpr int MaxHistorySize = 1000;
    std::map<std::string,InputViewerHistory *> _history;
public:
    InputViewer(wxWindow* parent, unsigned id);
    virtual ~InputViewer();
    bool processQueueItem(const MRL::Message &msg);
    void fetch(MRL::RtObjectRef &r); // load past hour of data
protected:
    virtual void DriveProcess(wxTimerEvent& event);
};
#endif // INPUTVIEWER_H
