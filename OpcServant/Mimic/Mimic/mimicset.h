#ifndef MIMICSET_H
#define MIMICSET_H
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
#include <Mimic/Mimic/mimicobject.h>

namespace MIMIC
{
    /*!
     * \brief The MimicSet class
     * Collection of objects in a display
     */
   typedef std::map<unsigned,MIMICOBJECTPTR> MIMICOBJECTMAP;
/*!
     * \brief The MimicSet class
     */
    class MimicSet
    {
        unsigned _nextId = 100;
        MRL::VariantPropertyTree _data; // data tree
        MIMICOBJECTMAP _objects;
        std::string _backgroundImage; // path to the background

    public:
        typedef enum
        {
            HIT_NONE = 0,
            HIT_OBJECT,
            HIT_ANCHOR1,
            HIT_ANCHOR2,
            HIT_ANCHOR3,
            HIT_ANCHOR4
        } HitPoint;

        MimicSet() {}
        MRL::VariantPropertyTree &data() { return  _data;}
        MIMICOBJECTMAP &objects() { return _objects;}
        void fromData(); // read from tree
        void toData(); // write to tree
        MIMICOBJECTPTR & find(unsigned id);
        MIMICOBJECTPTR & hitTest(const wxPoint &pt);
        HitPoint hitPoint(const wxPoint &pt, wxRect &rect);
        void addObject(const wxPoint &pt, const std::string &type);
        void removeObject(unsigned id);
        void draw(wxDC &dc, wxRect &r);
        void clear()
        {
            _objects.clear();
            _nextId = 100;
            _data.clear();
        }

    };
}
#endif // MIMICSET_H
