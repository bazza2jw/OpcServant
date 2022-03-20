#include "mimicbasetypes.h"
#include "mimictype.h"
#include "propertiesloader.h"
#include "PropertiesEditorDialog.h"

std::map<std::string,wxBitmap> MIMIC::IconObject::_map;


void initialiseBaseTypes()
{
    (void) new MIMIC::MimicObjectType<MIMIC::CircleObject>("Circle");
    (void) new MIMIC::MimicObjectType<MIMIC::RectObject>("Rectangle");
    (void) new MIMIC::MimicObjectType<MIMIC::TextObject>("Text");
    (void) new MIMIC::MimicObjectType<MIMIC::IconObject>("Icon");
    (void) new MIMIC::MimicObjectType<MIMIC::GotoObject>("Goto");

}



/*!
 * \brief MIMIC::TextObject::load
 * \param dlg
 * \param ns
 * \param p
 */
void MIMIC::TextObject::load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    MimicObject::load(dlg,ns,p);
    dlg.loader().addStringProperty("Text","Text",ns.data().getValue<std::string>(p,"TEXT")); // field[4]
}

/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void MIMIC::TextObject::save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    MimicObject::save(dlg,ns,p);
    wxVariant v = dlg.loader().fields()[4]->GetValue();
    _text = v.GetString().ToStdString();
    ns.data().setValue(p,"TEXT",_text);
}

/*!
 * \brief MIMIC::IconObject::load
 * \param dlg
 * \param ns
 * \param p
 */
static  wxArrayString staticList;

void MIMIC::IconObject::load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    MimicObject::load(dlg,ns,p);
    // list the icons directory
    wxArrayString l;
    MRL::getFileList(staticList,MIMIC_ROOT_DIR "/icons","*.png");
    wxString c(ns.data().getValue<std::string>(p,"ICON"));
    int i = staticList.Index(c);
    if(i < 0) i = 0;
    //
    wxEnumProperty * e = dlg.loader().addChoiceProperty("Icon", "Icon", i, staticList);
    if(e)
    {
        e->SetChoiceSelection(i);
    }


}

/*!
 * \brief MIMIC::IconObject::save
 * \param dlg
 * \param ns
 * \param p
 */
void MIMIC::IconObject::save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    MimicObject::save(dlg,ns,p);
    wxEnumProperty *e = dynamic_cast<wxEnumProperty *>(dlg.loader().fields()[4]);
    if(e)
    {
        std::string s = staticList[e->GetChoiceSelection()].ToStdString();
        ns.data().setValue(p,"ICON",s);
        fromData(&ns);
    }
}


static  wxArrayString staticGotoList;

void MIMIC::GotoObject::load(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    IconObject::load(dlg,ns,p);
    // list the icons directory
    wxArrayString l;
    MRL::getFileList(staticGotoList,MIMIC_ROOT_DIR "/mimic","*.mim");
    wxString c(ns.data().getValue<std::string>(p,"LINK"));
    int i = staticGotoList.Index(c);
    if(i < 0) i = 0;
    //
    wxEnumProperty * e = dlg.loader().addChoiceProperty("Link", "Link", i, staticGotoList);
    if(e)
    {
        e->SetChoiceSelection(i);
    }
}

/*!
 * \brief MIMIC::IconObject::save
 * \param dlg
 * \param ns
 * \param p
 */
void MIMIC::GotoObject::save(PropertiesEditorDialog &dlg,MimicSet &ns,MRL::PropertyPath p)
{
    IconObject::save(dlg,ns,p);
    wxEnumProperty *e = dynamic_cast<wxEnumProperty *>(dlg.loader().fields()[5]);
    if(e)
    {
        std::string s = staticGotoList[e->GetChoiceSelection()].ToStdString();
        ns.data().setValue(p,"LINK",s);
        fromData(&ns);
    }
}




