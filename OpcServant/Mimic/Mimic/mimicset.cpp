#include "mimicset.h"
#include "mimictype.h"

static MIMIC::MIMICOBJECTPTR nullObj;

/*!
 * \brief MIMIC::MimicSet::addObject
 * \param pt
 * \param type
 */
void MIMIC::MimicSet::addObject(const wxPoint &pt, const std::string &type)
{
    MimicType * t = MimicType::find(type);
    if(t)
    {
        MimicObject * o = t->create(_nextId++);
        o->rect() = wxRect(pt,wxSize(50,50));
        o->toData(this); // store it
        _objects[o->id()] = std::move(std::unique_ptr<MimicObject>(o));
    }
}
/*!
 * \brief MIMIC::MimicSet::removeObject
 * \param id
 */
void MIMIC::MimicSet::removeObject(unsigned id)
{
    MIMIC::MIMICOBJECTPTR &n = find(id);
    if(n)
    {
        MRL::PropertyPath p;
        n->toPath(p);
        data().remove(p);
        objects().erase(id);
    }
}

/*!
 * \brief MIMIC::MimicSet::find
 * \param id
 * \return
 */
MIMIC::MIMICOBJECTPTR & MIMIC::MimicSet::find(unsigned id)
{
    auto i = _objects.find(id);
    if(i != _objects.end()) return i->second;
    return nullObj;
}

/*!
 * \brief MIMIC::MimicSet::hitTest
 * \param pt
 * \param nodeId
 * \return
 */
MIMIC::MIMICOBJECTPTR &  MIMIC::MimicSet::hitTest(const wxPoint &pt)
{
    for(auto i = _objects.begin(); i != _objects.end(); i++)
    {
        MIMICOBJECTPTR &o = i->second;
        if(o)
        {
            if(o->rect().Contains(pt))
            {
                return o;
            }
        }
    }
    return nullObj;
}

/*!
 * \brief MIMIC::MimicSet::hitPoint
 * \param pt
 * \param o
 * \return
 */
MIMIC::MimicSet::HitPoint MIMIC::MimicSet::hitPoint(const wxPoint &pt, wxRect &rect)
{
    wxPoint p = rect.GetBottomLeft() - pt;
    if((abs(p.x) + abs(p.y)) < 10) return MIMIC::MimicSet::HIT_ANCHOR4;
    p = rect.GetBottomRight() - pt;
    if((abs(p.x) + abs(p.y)) < 10) return MIMIC::MimicSet::HIT_ANCHOR3;
    p = rect.GetRightTop() - pt;
    if((abs(p.x) + abs(p.y)) < 10) return MIMIC::MimicSet::HIT_ANCHOR2;
    p = rect.GetLeftTop() - pt;
    if((abs(p.x) + abs(p.y)) < 10) return MIMIC::MimicSet::HIT_ANCHOR1;
    if(rect.Contains(pt)) MIMIC::MimicSet::HIT_OBJECT;
    return MIMIC::MimicSet::HIT_NONE;
}

/*!
 * \brief MIMIC::MimicSet::draw
 * \param dc
 * \param r
 */
void MIMIC::MimicSet::draw(wxDC &dc, wxRect &r)
{
    for(auto i = _objects.begin(); i != _objects.end(); i++)
    {
        MIMICOBJECTPTR &o = i->second;
        if(o)
        {
            if(o->rect().Intersects(r))
            {
                o->draw(dc);
            }
        }
    }
}

/*!
 * \brief MIMIC::MimicSet::fromData
 */
void MIMIC::MimicSet::fromData() // read from tree
{
    _nextId = data().getInt("/NextId");
    _backgroundImage = data().getString("/Background");
    MRL::PropertyPath p;
    p.push_back("Objects");
    MRL::StringList l;
    data().listChildren(p,l);
    for(auto i = l.begin(); i != l.end(); i++)
    {
        p.push_back(*i);
        unsigned id = data().getValue<unsigned>(p,"ID");
        unsigned tid = data().getValue<unsigned>(p,"TYPE");
        MimicType * t = MimicType::find(tid);
        if(t)
        {
            MimicObject * o = t->create(id);
            _objects[o->id()] = std::move(std::unique_ptr<MimicObject>(o));
            o->fromData(this); // load from tree
        }
        p.pop_back();
    }
}
/*!
 * \brief MIMIC::MimicSet::toData
 */
void MIMIC::MimicSet::toData() // write to tree
{
    //
    data().setValue("/NextId",_nextId);
    data().setValue("/Background",_backgroundImage);
    //
    for(auto i = _objects.begin(); i != _objects.end(); i++)
    {
        MIMICOBJECTPTR &o = i->second;
        if(o)
        {
            o->toData(this);
        }
    }
}

