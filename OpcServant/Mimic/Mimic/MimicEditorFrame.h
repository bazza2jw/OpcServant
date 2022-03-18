#ifndef MIMICEDITORFRAME_H
#define MIMICEDITORFRAME_H
#include "MimicUI.h"

class MimicEditorFrame : public MimicEditorFrameBase
{
public:
    MimicEditorFrame(wxWindow* parent);
    virtual ~MimicEditorFrame();
//
    virtual void onExit(wxCommandEvent& event);
    virtual void onNew(wxCommandEvent& event);
};
#endif // MIMICEDITORFRAME_H
