#ifndef MIMICDISPLAYPLUGIN_H
#define MIMICDISPLAYPLUGIN_H
#include <Common/plugin.h>
#include <Common/objectmanager.h>

namespace MRL
{
    class MimicDisplayPlugin : MRL::Plugin
    {
    public:
        MimicDisplayPlugin();
        void initialise();
    };
}
#endif // MIMICDISPLAYPLUGIN_H
