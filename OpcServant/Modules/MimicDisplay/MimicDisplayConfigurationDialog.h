#ifndef MIMICDISPLAYCONFIGURATIONDIALOG_H
#define MIMICDISPLAYCONFIGURATIONDIALOG_H
#include "MimicDisplayUI.h"
#include <Common/common.h>
/*!
 * \brief The MimicDisplayConfigurationDialog class
 */
class MimicDisplayConfigurationDialog : public MimicDisplayConfigurationDialogBase
{
    const MRL::PropertyPath & _path; // path to object in configuration and elsewhere
    MRL::VariantPropertyTree _configuration; // tree of configuration values

public:
    MimicDisplayConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~MimicDisplayConfigurationDialog();
protected:
    virtual void onEdit(wxCommandEvent& event);
    virtual void onOk(wxCommandEvent& event);
};
#endif // MIMICDISPLAYCONFIGURATIONDIALOG_H
