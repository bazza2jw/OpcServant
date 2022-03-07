#ifndef OPCCLIENTOBJECTMANGER_H
#define OPCCLIENTOBJECTMANGER_H
//
#include <Common/objectmanager.h>
#include <Common/Gui/SetupDialog.h>
#include <Common/Web/webpanel.h>
#include "OpcClientConfigurationDialog.h"
#include "opcclientrt.h"
//
namespace MRL
{
/*!
 * \brief The OpcClientObjectManger class
 */
    typedef ObjectManagerRT<OpcClientRT, OpcClientConfigurationDialog> OpcClientObjectManagerBase;
class OpcClientObjectManager : public OpcClientObjectManagerBase
{
public:
    OpcClientObjectManager() : OpcClientObjectManagerBase("OpcClient") {}
};
}
#endif // OPCCLIENTOBJECTMANGER_H
