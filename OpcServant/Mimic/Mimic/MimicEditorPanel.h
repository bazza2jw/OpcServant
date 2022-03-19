#ifndef MIMICEDITORPANEL_H
#define MIMICEDITORPANEL_H
#include "MimicUI.h"
#include "mimiccanvas.h"
class MimicEditorPanel : public MimicEditorPanelBase
{
    MimicCanvas * _canvas = nullptr;
    wxTreeItemId _root;

public:
    MimicEditorPanel(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size =  wxSize(-1,-1), long style = wxTAB_TRAVERSAL);
    virtual ~MimicEditorPanel();
    void onDragInit(wxTreeEvent& event);
    MimicCanvas * canvas() { return _canvas;}

    virtual void onNew(wxCommandEvent& event);
    virtual void onOpen(wxCommandEvent& event);
    virtual void onSave(wxCommandEvent& event);
    virtual void onSaveAs(wxCommandEvent& event);
protected:
    virtual void onAddBackground(wxCommandEvent& event);
};
#endif // MIMICEDITORPANEL_H
