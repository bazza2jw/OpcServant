#ifndef OPCSERVANTOBJECTS_H
#define OPCSERVANTOBJECTS_H
#include <Mimic/Mimic/mimictype.h>
#include <Mimic/Mimic/mimiccontrols.h>
#include <Mimic/Mimic/mimicbasetypes.h>
namespace MIMIC {

    class OpcServantInputObject : public MIMIC::TrafficLightObject
    {
        std::string _alias;
        unsigned _id; // the object id
        std::string _tag;

    public:
        OpcServantInputObject(unsigned id = 0, unsigned t = 0) : TrafficLightObject(id,t) {}
        virtual void load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p);
        virtual void save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p);
        virtual void fromData(MimicSet *set);
        virtual void process(MimicSet *set);
        virtual void onClick(wxWindow */*parent*/, wxPoint /*pt*/,MimicSet * /*set*/);
        virtual void onMove(wxWindow */*parent*/,  wxPoint /*pt*/,MimicSet  * /*set*/) {}

    };

}

#endif // OPCSERVANTOBJECTS_H
