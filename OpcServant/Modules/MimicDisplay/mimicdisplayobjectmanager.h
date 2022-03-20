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
    virtual bool canBeChildOf(const std::string &type) {
        if (type == SITE_TYPE) { // can be a child of a folder
            return true;
        }

        if (type == FOLDER_TYPE) { // can be a child of a folder
            return true;
        }

        return false;
    }
};
}
#endif // MIMICDISPLAYOBJECTMANAGER_H
