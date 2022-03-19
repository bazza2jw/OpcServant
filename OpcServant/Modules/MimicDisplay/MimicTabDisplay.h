#ifndef MIMICTABDISPLAY_H
#define MIMICTABDISPLAY_H
#include "MimicDisplayUI.h"
#include <Common/Daq/daqcommon.h>
#include <Mimic/Mimic/mimiccanvas.h>

#include <Mimic/Mimic/MimicEditorPanel.h>
#include <Mimic/Mimic/mimicbasetypes.h>
#include <Mimic/Mimic/mimicset.h>
#include <Common/common.h>

class MimicTabDisplay : public MimicTabDisplayBase, public MRL::BObject
{
    std::string _fileName; // the file name of th emimic file
    MIMIC::MimicSet _objects;
    MimicCanvas * _canvas = nullptr;
    bool _loaded = false;

public:
    MimicTabDisplay(wxWindow* parent, unsigned id);
    virtual ~MimicTabDisplay();
    bool processQueueItem(const MRL::Message &msg);

protected:
    virtual void onTimer(wxTimerEvent& event);
    void load();
};
#endif // MIMICTABDISPLAY_H
