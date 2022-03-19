#ifndef MIMICBASETYPES_H
#define MIMICBASETYPES_H

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
#include <Mimic/Mimic/mimicobject.h>



namespace MIMIC
{
/*!
     * \brief The CircleObject class
     */
class CircleObject : public MimicObject
{
public:
    CircleObject(unsigned i = 0, unsigned t = 0) : MimicObject(i,t) {}
    virtual void draw(wxDC &dc)
    {
        wxDCBrushChanger bc(dc,*wxTheBrushList->FindOrCreateBrush(colour(),wxBRUSHSTYLE_SOLID));
        wxDCPenChanger pc(dc,*wxThePenList->FindOrCreatePen(textColour(), 1, wxPENSTYLE_SOLID));
        //
        dc.DrawEllipse(rect());
        MimicObject::draw(dc);
    }
};

/*!
 * \brief The RectObject class
 */
class RectObject : public MimicObject
{
public:
    RectObject(unsigned i = 0, unsigned t = 0) : MimicObject(i,t) {}
    virtual void draw(wxDC &dc)
    {
        wxDCBrushChanger bc(dc,*wxTheBrushList->FindOrCreateBrush(colour(),wxBRUSHSTYLE_SOLID));
        wxDCPenChanger pc(dc,*wxThePenList->FindOrCreatePen(textColour(), 1, wxPENSTYLE_SOLID));
        //
        dc.DrawRectangle(rect());
        MimicObject::draw(dc);
    }
};
/*!
 * \brief The TextObject class
 */
class TextObject : public RectObject
{
    std::string _text = "TextLabel";
public:
    TextObject(unsigned i = 0, unsigned t = 0) : RectObject(i,t) {}
    virtual void draw(wxDC &dc)
    {
        RectObject::draw(dc);
        //
        wxDCFontChanger fc(dc,*wxNORMAL_FONT); // set the font - TBD make this editable
        //
        dc.SetTextBackground(colour());
        dc.SetTextForeground(textColour());
        dc.DrawText(_text,rect().GetTopLeft());
        //
    }
    virtual void load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p);
    virtual void save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p);
    virtual void fromData(MimicSet *set)
    {
        MimicObject::fromData(set);
        MRL::PropertyPath p;
        toPath(p);
        _text = set->data().getValue<std::string>(p,"TEXT");
    }
};

/*!
 * \brief The IconObject class
 */
class IconObject : public RectObject
{
    std::string _icon;
    static std::map<std::string,wxBitmap> _map;
public:
    IconObject(unsigned i = 0, unsigned t = 0) : RectObject(i,t) {}
    /*!
     * \brief draw
     * \param dc
     */
    virtual void draw(wxDC &dc)
    {
        if(_map.find(_icon) != _map.end())
        {
            wxBitmap &b = _map[_icon];
            if(b.IsOk()) dc.DrawBitmap(b, rect().GetTopLeft());
        }
        else
        {
            RectObject::draw(dc);
        }
    }
    virtual void load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p);
    virtual void save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p);
    /*!
     * \brief fromData
     * \param set
     */
    virtual void fromData(MimicSet *set)
    {
        MimicObject::fromData(set);
        MRL::PropertyPath p;
        toPath(p);
        //
        // get the icon name
        setIcon(set->data().getValue<std::string>(p,"ICON"));
        // load into cache
    }
    //
    bool canResize() const { return false;}
    const std::string & icon() { return _icon;}
    void setIcon(const std::string &s)
    {
        if(!s.empty())
        {
            _icon = s;
            if(_map.find(_icon) == _map.end())
            {
                std::string f = std::string(MIMIC_ROOT_DIR "/icons/")  + _icon;
                wxBitmap b;
                b.LoadFile(f);
                rect().SetSize(b.GetSize());
                _map[_icon] = b;
            }
            else
            {
                wxBitmap &b = _map[_icon];
                rect().SetSize(b.GetSize());
            }
        }
    }
    wxBitmap & bitmap() { return  _map[_icon];}
    //
};






}
#endif // MIMICBASETYPES_H
