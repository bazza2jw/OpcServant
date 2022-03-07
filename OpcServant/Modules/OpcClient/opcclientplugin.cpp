#include "opcclientplugin.h"
#include "opcclientobjectmanger.h"
#include "opcclienttypeobject.h"

static MRL::OpcClientPlugin _instance;
MRL::OpcClientPlugin::OpcClientPlugin() : Plugin("OpcClient")
{

}

/*!
 * \brief MRL::MQTT::initialise
 */
void MRL::OpcClientPlugin::initialise()
{
    (void) new MRL::OpcClientObjectManager(); // create the object factory / manager
}

/*!
 * \brief MRL::MQTT::terminate
 */
void MRL::OpcClientPlugin::terminate()
{

}
/*!
 * \brief MRL::OpcClientPlugin::initialiseOpc
 */
void MRL::OpcClientPlugin::initialiseOpc()
{
    //Common::opc().add<OpcClientTypeObject>("OpcClient");
}
