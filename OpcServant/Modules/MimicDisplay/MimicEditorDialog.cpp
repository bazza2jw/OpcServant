#include "MimicEditorDialog.h"
#include <Common/common.h>
/*!
 * \brief MimicEditorDialog::MimicEditorDialog
 * \param parent
 * \param f
 */
MimicEditorDialog::MimicEditorDialog(wxWindow* parent, const std::string &f)
    : MimicEditorDialogBase(parent)
{
    if(!f.empty())
    {
        std::string file = MRL::Common::baseDir() + "/mimic/" + f;
        GetPanelEditor()->canvas()->readSet(file); // load it
    }
}
/*!
 * \brief MimicEditorDialog::~MimicEditorDialog
 */
MimicEditorDialog::~MimicEditorDialog()
{

}
/*!
 * \brief MimicEditorDialog::onOk
 * \param event
 */
void MimicEditorDialog::onOk(wxCommandEvent& event)
{
    GetPanelEditor()->onSave(event);
    EndModal(wxID_OK);
}
/*!
 * \brief MimicEditorDialog::onTimer
 * \param event
 */
void MimicEditorDialog::onTimer(wxTimerEvent& event)
{

}
