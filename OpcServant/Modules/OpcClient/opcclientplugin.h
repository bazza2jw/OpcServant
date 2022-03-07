#ifndef OPCCLIENTPLUGIN_H
#define OPCCLIENTPLUGIN_H
#include <Common/plugin.h>
#include <Common/objectmanager.h>

namespace MRL
{
/*!
 * \brief The OpcClientPlugin class
 */
class OpcClientPlugin : public Plugin
{
public:
    OpcClientPlugin();
    virtual void initialise();
    virtual void terminate();
    virtual void initialiseOpc();

};
}
#endif // OPCCLIENTPLUGIN_H
