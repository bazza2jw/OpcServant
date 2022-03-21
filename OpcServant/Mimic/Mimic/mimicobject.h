#ifndef MIMICOBJECT_H
#define MIMICOBJECT_H
#include <MrlLib/mrllib.h>
#include <wx/gdicmn.h>
#include <MrlLib/variantpropertytree.h>
#include <wx/pen.h>
#include <wx/brush.h>
#include <wx/graphics.h>
class PropertiesEditorDialog;

#ifndef MIMIC_ROOT_DIR
// where to look - could set in make file
#define MIMIC_ROOT_DIR "/usr/local/OpcServant"
#endif

namespace MIMIC

{
/*!
     * \brief The MimicObject class
     * Base for an item in a mimic display
     */
    class MimicSet;
    class MimicObject
    {
        unsigned _id = 0;
        unsigned _type = 0;
        wxRect _rect; // position and size
        std::string _name; // object name
        bool _selected = false;
        wxColour _colour = *wxWHITE; // node colour
        wxColour _textColour = *wxBLACK; // text node colour
        bool _enabled = true;
        //
    public:
        MimicObject(unsigned i = 0, unsigned t = 0) : _id(i), _type(t) {}
        MimicObject(const MimicObject &) = delete;
        virtual ~MimicObject() {}
        //
        unsigned id() const {
            return _id;
        }
        void setId(unsigned id) {
            _id = id;
        }
        //
        unsigned type() const {
            return _type;   // the node type
        }
        void setType(size_t t) {
            _type = t;
        }

        const std::string & name() const { return _name;}

        void setName(const std::string &n)
        {
            _name = n;
        }
        //
        bool selected() const { return _selected;}
        void setSelected(bool f) { _selected = f;}
        //
        virtual bool canResize() const { return true;}
        //
        wxRect  &rect() { return _rect;}
        //
        // Read & write to config
        virtual void fromData(MimicSet *);
        virtual void toData(MimicSet *);
        //
        void toPath(MRL::PropertyPath &p) // path in tree to a node
        {
            p.clear();
            p.push_back("Objects");
            p.push_back(std::to_string(id()));
        }
        //
        virtual void draw(wxDC &);
        //
        virtual bool properties(wxWindow * parent, MimicSet &ns);
        virtual void load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p);
        virtual void save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p);
        //
        virtual void onClick(wxWindow */*parent*/, wxPoint /*pt*/,MimicSet * /*set*/) {}
        virtual void onMove(wxWindow */*parent*/,  wxPoint /*pt*/,MimicSet  * /*set*/) {}
        //
        virtual wxColour & colour() { return _colour;} // node colour
        virtual wxColour & textColour() { return _textColour;} // text node colour
        virtual void process(MimicSet *) {} // periodic processing
    };
    typedef std::unique_ptr<MimicObject> MIMICOBJECTPTR;
}
#endif // MIMICOBJECT_H
