#ifndef MIMICCANVAS_H
#define MIMICCANVAS_H

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <wx/overlay.h>
#include <wx/scrolwin.h>
#include <wx/dnd.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/stream.h>
#include <wx/wfstream.h>
#include <Mimic/Mimic/mimicset.h>






class MimicCanvas : public wxScrolled<wxWindow>
{
    int _nextId = 1000;
    //
    int _state = 0;
    wxRect _rect;
    wxPoint _currentpoint;
    wxPoint _startpoint;
    wxPoint lastPt;
    wxOverlay _overlay;
    bool _editMode = true;
    //
    MIMIC::MimicSet _objects; // the objects in the current layout
    MIMIC::MimicObject * _currentObject = nullptr;
    MIMIC::MimicSet::HitPoint _hitPoint = MIMIC::MimicSet::HIT_NONE;
    enum
    {
        NONE = 0,
        OBJECT_SELECT,
        ANCHOR_SELECT,
        OBJECT_MOVE
    };

    int _snapGrid = 10;

    wxPoint snapToGrid(wxPoint pt)
    {
        pt.x = pt.x - (pt.x % _snapGrid);
        pt.y = pt.y - (pt.y % _snapGrid);
        return pt;
    }

    wxBitmap _backgroundImage; // the background image


public:
    MimicCanvas(wxWindow * parent) :  wxScrolled<wxWindow>(parent,wxID_ANY)
    {
        _dropTarget = new CanvasDropTarget(this);
        SetDropTarget 	( _dropTarget );
        SetCursor(*wxCROSS_CURSOR);
        //
        wxColour background(wxTheColourDatabase->Find("WHEAT"));
        SetBackgroundColour(background);
        //
        Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MimicCanvas::onLeftDown), NULL, this);
        Connect(wxEVT_LEFT_UP, wxMouseEventHandler(MimicCanvas::onLeftUp), NULL, this);
        Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(MimicCanvas::onRightDown), NULL, this);
        Connect(wxEVT_CHAR, wxKeyEventHandler(MimicCanvas::onChar), NULL, this);
        Connect(wxEVT_MOTION, wxMouseEventHandler(MimicCanvas::onMotion), NULL, this);
    }

    /*!
     * \brief addObject
     * \param x
     * \param y
     * \param key
     */
    virtual void addObject(int x, int y , const std::string &key);

    /*!
     * \brief The CanvasDropTarget class
     */
    class CanvasDropTarget : public wxTextDropTarget
    {
        MimicCanvas * _parent = nullptr;
    public:
        CanvasDropTarget(MimicCanvas *p) : _parent(p)
        {
        }
        virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data)
        {
            _parent->addObject(x,y,data.ToStdString());
            return true;
        }
    };
    //
    CanvasDropTarget * _dropTarget = nullptr;

    void newMimic()
    {
        _objects.clear();
        _backgroundImage = wxBitmap();
        Refresh();
    }
    //
    bool editMode() const { return _editMode;}
    void setEditMode(bool f) { _editMode = f;}
    //
    void open();
    void readSet(const std::string &);
    void save();
    void saveAs();
    void setBackground(const std::string &s);
    //
    virtual void OnDraw (wxDC &dc);
    virtual void OnCaptureLost(wxMouseCaptureLostEvent& event);
    virtual void onChar(wxKeyEvent& event);
    virtual void onLeftDown(wxMouseEvent& event);
    virtual void onLeftUp(wxMouseEvent& event);
    virtual void onMotion(wxMouseEvent& event);
    virtual void onRightDown(wxMouseEvent& event);
    void onPopupClick(wxCommandEvent &evt);
    MIMIC::MimicSet & objects() { return  _objects;}
};

#endif // MIMICCANVAS_H
