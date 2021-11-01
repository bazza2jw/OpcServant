#include "EditObjectDialog.h"
#include <BourbonCommon/bourboncommon.h>
#include <BourbonCommon/objectmanager.h>

/*!
 * \brief EditObjectDialog::EditObjectDialog
 * \param parent
 * \param path
 */
EditObjectDialog::EditObjectDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : EditObjectDialogBase(parent),_path(path),_common(m_panelCommon),_recipe(m_panelRecipe)
{
    _common.createSheet();
    CommonSizer->Add(_common.root(), 1, wxALL|wxEXPAND, WXC_FROM_DIP(20));
    _recipe.createSheet();
    RecipeSizer->Add(_recipe.root(), 1, wxALL|wxEXPAND, WXC_FROM_DIP(20));
}

/*!
 * \brief EditObjectDialog::~EditObjectDialog
 */
EditObjectDialog::~EditObjectDialog()
{

}

/*!
 * \brief EditObjectDialog::OnOk
 * \param event
 */
void EditObjectDialog::OnOk(wxCommandEvent& event)
{
    unsigned t = MRL::BourbonCommon::configuration().typeId(_path);
    auto o = MRL::ObjectManager::find(t);
    if(o)
    {
        o->saveSheet(_path,MRL::CommonSheet,_common);
    }
}

/*!
 * \brief EditObjectDialog::setCommonPropertySheet
 */
void EditObjectDialog::setCommonPropertySheet()
{
    unsigned t = MRL::BourbonCommon::configuration().typeId(_path);
    auto o = MRL::ObjectManager::find(t);
    if(o)
    {
        o->setSheet(_path,MRL::CommonSheet,_common);
        o->loadSheet(_path,MRL::CommonSheet,_common);
    }
}

/*!
 * \brief EditObjectDialog::setRecipePropertySheet
 */
void EditObjectDialog::setRecipePropertySheet()
{
    unsigned t = MRL::BourbonCommon::configuration().typeId(_path);
    auto o = MRL::ObjectManager::find(t);
    if(o)
    {
        o->setSheet(_path,MRL::RecipeSheet,_common);
    }
}

/*!
 * \brief EditObjectDialog::OnRecipeSelectionChanged
 * \param event
 */
void EditObjectDialog::OnRecipeSelectionChanged(wxCommandEvent& event)
{
    unsigned t = MRL::BourbonCommon::configuration().typeId(_path);
    auto o = MRL::ObjectManager::find(t);
    if(!_currentRecipe.empty())
    {
        o->loadSheet(_path,MRL::RecipeSheet,_common,_currentRecipe);
    }
    _currentRecipe = GetRecipe()->GetValue().ToStdString();
    o->loadSheet(_path,MRL::RecipeSheet,_common,_currentRecipe);
}
