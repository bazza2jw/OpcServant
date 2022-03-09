#include "FlowEditorDialog.h"

/*!
 * \brief FlowEditorDialog::FlowEditorDialog
 * \param parent
 * \param path
 */
FlowEditorDialog::FlowEditorDialog(wxWindow* parent, const wxString &path)
    : FlowEditorDialogBase(parent),_path(path.ToStdString())
{
    GetEditorPanel()->canvas()->readSet(_path); // load the set
}

FlowEditorDialog::~FlowEditorDialog()
{
}

void FlowEditorDialog::onOk(wxCommandEvent& /*event*/)
{
    GetEditorPanel()->canvas()->save(); // save the current set under the current name
    EndModal(wxID_OK);
}
