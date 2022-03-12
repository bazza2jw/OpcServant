#ifndef NODEFLOWWEBCONFIGURATION_H
#define NODEFLOWWEBCONFIGURATION_H
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>
#include <Wt/WDoubleSpinBox.h>
#include "nodeflowrt.h"
#include <MrlLib/stats.hpp>

namespace MRL
{
    class NodeFlowWebConfiguration : public WebDialogBase
    {
        PropertyPath _path;
        VariantPropertyTree _configuration;
        //
        Wt::WComboBox * _flowName = nullptr;
        Wt::WCheckBox * _enabled = nullptr;
        //
    public:
        NodeFlowWebConfiguration( const PropertyPath &path) : _path(path){}
        void setup();
        void onOk();
    };
}
#endif // NODEFLOWWEBCONFIGURATION_H
