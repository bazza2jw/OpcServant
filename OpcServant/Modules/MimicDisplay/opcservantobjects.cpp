#include "opcservantobjects.h"
#include <Common/Daq/rtobject.h>
#include <Common/Daq/daq.h>
#include <Common/Daq/daqcommon.h>
#include <Common/Gui/InputViewer.h>
static  wxArrayString staticList;

template <typename T>
void buildAliasList(T &l)
{
    l.clear();
    MRL::StringMap &a = MRL::Common::aliasMap();
    for(auto i = a.begin(); i != a.end(); i++)
    {
        l.push_back(i->first);
    }
    l.push_back("(null)");
}

/*!
 * \brief MIMIC::OpcServantInputObject::load
 * \param dlg
 * \param ns
 * \param p
 */
void MIMIC::OpcServantInputObject::load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    TrafficLightObject::load(dlg,ns,p);
    buildAliasList(staticList);
    wxString c(ns.data().getValue<std::string>(p,"INPUT"));
    int i = staticList.Index(c);
    if(i < 0) i = 0;
    //
    wxEnumProperty * e = dlg.loader().addChoiceProperty("Input", "Input", i, staticList);// [8]
    if(e)
    {
        e->SetChoiceSelection(i);
    }

}
/*!
 * \brief MIMIC::OpcServantInputObject::save
 * \param dlg
 * \param ns
 * \param p
 */
void MIMIC::OpcServantInputObject::save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    TrafficLightObject::save(dlg,ns,p);
    wxEnumProperty *e = dynamic_cast<wxEnumProperty *>(dlg.loader().fields()[8]);
    if(e)
    {
        std::string s = staticList[e->GetChoiceSelection()].ToStdString();
        ns.data().setValue(p,"INPUT",s);
    }
}
/*!
 * \brief MIMIC::OpcServantInputObject::fromData
 * \param set
 */
void MIMIC::OpcServantInputObject::fromData(MimicSet *set)
{
    TrafficLightObject::fromData(set);
    MRL::PropertyPath p;
    toPath(p);
    _alias = set->data().getValue<std::string>(p,"INPUT");
    std::string o = MRL::Common::aliasMap()[_alias];
    // split on the tag separator
    MRL::PathTag pt;
    if(pt.parse(o))
    {
        MRL::PropertyPath p;
        p.toList(pt._path);
        _id  = MRL::Common::configuration().find(p); // the object id
        _tag = pt._tag;
    }
}

/*!
 * \brief MIMIC::OpcServantInputObject::process
 */
void MIMIC::OpcServantInputObject::process(MimicSet *set)
{
    MRL::RtObjectRef &r  =  MRL::Common::daq().objects()[_id];
    if(r)
    {
        if(r->enabled())
        {
            setText(r->getInputAsString(_tag));
            setState(r->getStatus(_tag));
        }
        else
        {
            setState(0);
            setText("Disabled");
        }
    }
}

/*!
 * \brief MIMIC::OpcServantInputObject::onClick
 */
void MIMIC::OpcServantInputObject::onClick(wxWindow *parent, wxPoint pt, MimicSet *set)
{
    InputViewer * dlg = new InputViewer(parent,_id);
    wxSize sz(300,200);
    dlg->SetSize(sz);
    dlg->Show();
}

