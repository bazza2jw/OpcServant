#include "mimicobject.h"
#include <Mimic/Mimic/mimicset.h>
#include <Mimic/Mimic/PropertiesEditorDialog.h>
#include <Mimic/Mimic/propertiesloader.h>
/*!
 * \brief MIMIC::MimicObject::fromData
 * \param set
 */
void MIMIC::MimicObject::fromData(MimicSet *set)
{
    MRL::PropertyPath p;
    toPath(p);
    int x = set->data().getValue<int>(p,"X");
    int y = set->data().getValue<int>(p,"Y");
    int w = set->data().getValue<int>(p,"W");
    int h = set->data().getValue<int>(p,"H");
    //
    _rect = wxRect(x,y,w,h);
    _type = set->data().getValue<int>(p,"TYPE");
    _name = set->data().getValue<std::string>(p,"NAME");
    _enabled = set->data().getValue<bool>(p,"ENABLED");
    //
    std::string cs = set->data().getValue<std::string>(p,"Colour"); // configured colour
    wxColour c(cs);
    _colour = c;
    //
    std::string ts = set->data().getValue<std::string>(p,"TextColour"); // configured colour
    wxColour tc(ts);
    _textColour = tc;

}
/*!
 * \brief MIMIC::MimicObject::toData
 */
void MIMIC::MimicObject::toData(MimicSet *set)
{
    MRL::PropertyPath p;
    toPath(p);
    set->data().setValue(p,"X",_rect.GetLeft());
    set->data().setValue(p,"Y",_rect.GetTop());
    set->data().setValue(p,"W",_rect.GetWidth());
    set->data().setValue(p,"H",_rect.GetHeight());
    set->data().setValue(p,"TYPE",_type);
    set->data().setValue(p,"ID",_id);
    // colour is set by property sheets
}
/*!
 * \brief MIMIC::MimicObject::properties
 * \param parent
 * \param ns
 * \param nodeId
 * \return
 */
bool MIMIC::MimicObject::properties(wxWindow * parent, MimicSet &ns)
{
    try
    {
        // property page
        MRL::PropertyPath p;
        toPath(p);
        PropertiesEditorDialog dlg(parent,ns.data(),p);
        //
        load(dlg,ns,p);
        if(dlg.ShowModal() == wxID_OK)
        {
            save(dlg,ns,p);
            return true;
        }
    }
    CATCH_DEF
    return false;
}
/*!
 * \brief MIMIC::MimicObject::load
 * \param dlg
 * \param ns
 * \param p
 */
void MIMIC::MimicObject::load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    dlg.loader().addStringProperty("Name","Name",ns.data().getValue<std::string>(p,"NAME")); // field[0]
    dlg.loader().addBoolProperty("Enable Object","Enable",ns.data().getValue<bool>(p,"ENABLED")); // field[1]
    //
    wxString cs(ns.data().getValue<std::string>(p,"Colour"));
    wxColour c(cs);
    dlg.loader().addColourProperty("Colour","Colour",c); // field[2]
    //
    wxString ts(ns.data().getValue<std::string>(p,"TextColour"));
    wxColour t(ts);
    dlg.loader().addColourProperty("TextColour","TextColour",t); // field[3]
    //
}
/*!
 * \brief MIMIC::MimicObject::save
 * \param dlg
 * \param ns
 * \param p
 */
void MIMIC::MimicObject::save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    wxVariant v = dlg.loader().fields()[0]->GetValue();
    ns.data().setValue(p,"NAME",v.GetString().ToStdString());
    //
    v = dlg.loader().fields()[1]->GetValue();
    ns.data().setValue(p,"ENABLED",v.GetBool());
    //
    wxColourProperty *c = static_cast<wxColourProperty *>(dlg.loader().fields()[2]);
    wxString s = c->GetValueAsString();
    if(s.at(0) == '(') s = "rgb"+s;
    ns.data().setValue(p,"Colour", s.ToStdString());
    //
    c = static_cast<wxColourProperty *>(dlg.loader().fields()[3]);
    wxString t = c->GetValueAsString();
    if(t.at(0) == '(') t = "rgb"+t;
    ns.data().setValue(p,"TextColour", t.ToStdString());
    //
    fromData(&ns);
}

/*!
 * \brief MIMIC::MimicObject::draw
 * \param dc
 */
void MIMIC::MimicObject::draw(wxDC &dc)
{
    if(selected() && canResize())
    {
        // draw anchors
        wxDCBrushChanger br(dc,*wxBLACK_BRUSH);
        wxDCPenChanger pn(dc,*wxBLACK_PEN);
        //
        dc.DrawCircle(rect().GetBottomLeft(),5);
        dc.DrawCircle(rect().GetBottomRight(),5);
        dc.DrawCircle(rect().GetTopLeft(),5);
        dc.DrawCircle(rect().GetTopRight(),5);
    }
}


