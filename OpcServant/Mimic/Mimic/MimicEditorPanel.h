#ifndef MIMICEDITORPANEL_H
#define MIMICEDITORPANEL_H
#include "MimicUI.h"
#include "mimiccanvas.h"
class MimicEditorPanel : public MimicEditorPanelBase
{
    MimicCanvas * _canvas = nullptr;
    wxTreeItemId _root;

public:
    MimicEditorPanel(wxWindow* parent);
    virtual ~MimicEditorPanel();
    void onDragInit(wxTreeEvent& event);


    virtual void onNew(wxCommandEvent& event);
    virtual void onOpen(wxCommandEvent& event);
    virtual void onSave(wxCommandEvent& event);
    virtual void onSaveAs(wxCommandEvent& event);
protected:
    virtual void onAddBackground(wxCommandEvent& event);
};
#endif // MIMICEDITORPANEL_H
