#include "InputViewerHistory.h"

InputViewerHistory::InputViewerHistory(wxWindow* parent)
    : InputViewerHistoryBase(parent)
{
    GetGraph()->graph().setMaxPoints(VIEW_MAX_POINTS);
}

InputViewerHistory::~InputViewerHistory()
{
}

