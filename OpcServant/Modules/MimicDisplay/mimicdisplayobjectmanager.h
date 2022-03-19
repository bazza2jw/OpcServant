#ifndef MIMICDISPLAYOBJECTMANAGER_H
#define MIMICDISPLAYOBJECTMANAGER_H
#include <Common/objectmanager.h>
#include "mimicdisplayrt.h"
#include "MimicDisplayConfigurationDialog.h"

namespace MRL
{
typedef ObjectManagerRT < MimicDisplayRT, MimicDisplayConfigurationDialog> MimicDisplayObjectManagerBase;
class MimicDisplayObjectManager : public MimicDisplayObjectManagerBase
{
public:
    MimicDisplayObjectManager() :  MimicDisplayObjectManagerBase("MimicDisplay") {}
    wxWindow * createTabWindow(wxWindow *parent, unsigned id);
};
}
#endif // MIMICDISPLAYOBJECTMANAGER_H
