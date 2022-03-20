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
        //
        std::stack<std::string> _stack;
        std::string _fileName;

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
            _fileName.clear();
            _objects.clear();
            _nextId = 100;
            _data.clear();
            _backgroundImage.clear();
            toData();
        }
        /*!
         * \brief process
         */
        void process()
        {
            for(auto i = _objects.begin(); i != _objects.end(); i++)
            {
                MIMICOBJECTPTR &o = i->second;
                if(o)
                {
                    o->process(this);
                }
            }
        }

        const std::string & fileName() { return _fileName;}
        void setFileName(const std::string &f) { _fileName = f;}

        const std::string &backgroundImage() { return   _backgroundImage;}
        void setBackgroundImage(const std::string &s){ _backgroundImage = s;}

         void onClick(wxWindow *parent, wxPoint pt)
         {
             MIMIC::MIMICOBJECTPTR &o = hitTest(pt);
             if(o)
             {
                 o->onClick(parent,pt,this);
             }
         }

         bool readSet(const std::string &f);

         void gotoSet(const std::string &s)
         {
            _stack.push(_fileName);
            readSet(s);
         }

         void popSet()
         {
             if(!_stack.empty())
             {
                 clear();
                 readSet(_stack.top());
                 _stack.pop();
             }
         }


    };
}
#endif // MIMICSET_H
