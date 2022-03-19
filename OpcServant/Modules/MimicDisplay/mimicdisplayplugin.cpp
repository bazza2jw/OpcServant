#include "mimicdisplayplugin.h"
#include "mimicdisplayobjectmanager.h"
#include "opcservantobjects.h"

void initialiseBaseTypes(); // mimic object loaders
void loadControlObjects();

/*!
 * \brief _instance
 */
static MRL::MimicDisplayPlugin _instance; // self register
/*!
 * \brief MRL::MimicDisplayPlugin::MimicDisplayPlugin
 */
MRL::MimicDisplayPlugin::MimicDisplayPlugin() : Plugin("MimicDisplay")
{

}
/*!
 * \brief MRL::MimicDisplayPlugin::initialise
 */
void MRL::MimicDisplayPlugin::initialise()
{
    initialiseBaseTypes();
    loadControlObjects();
    (void) new MIMIC::MimicObjectType<MIMIC::OpcServantInputObject>("OpcServantInput");
    // load the objects for the editor
    (void) new MimicDisplayObjectManager(); // MimicDisplayPlugin publisher
}


