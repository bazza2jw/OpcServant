#include "mimicdisplayobjectmanager.h"
#include "MimicTabDisplay.h"
/*!
 * \brief MRL::MimicDisplayObjectManager::createTabWindow
 * \param parent
 * \param id
 * \return
 */
wxWindow * MRL::MimicDisplayObjectManager::createTabWindow(wxWindow *parent, unsigned id)
{
    return new MimicTabDisplay(parent,id);
}

