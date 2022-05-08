#ifndef INPUTVIEWERHISTORY_H
#define INPUTVIEWERHISTORY_H
#include "MimicDisplayUI.h"

class InputViewerHistory : public InputViewerHistoryBase
{
    static constexpr int VIEW_MAX_POINTS = 1000;
public:
    InputViewerHistory(wxWindow* parent);
    virtual ~InputViewerHistory();
};
#endif // INPUTVIEWERHISTORY_H
