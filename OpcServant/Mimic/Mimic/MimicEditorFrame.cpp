#include "MimicEditorFrame.h"
#include "MimicEditorChild.h"
#include <wx/app.h>

/*!
 * \brief MimicEditorFrame::MimicEditorFrame
 * \param parent
 */
MimicEditorFrame::MimicEditorFrame(wxWindow* parent)
    : MimicEditorFrameBase(parent)
{
}
/*!
 * \brief MimicEditorFrame::~MimicEditorFrame
 */
MimicEditorFrame::~MimicEditorFrame()
{
}
/*!
 * \brief MimicEditorFrame::onExit
 * \param event
 */
void MimicEditorFrame::onExit(wxCommandEvent& event)
{
      wxApp::GetInstance()->ExitMainLoop();
}
/*!
 * \brief MimicEditorFrame::onNew
 * \param event
 */
void MimicEditorFrame::onNew(wxCommandEvent& event)
{
    MimicEditorChild *w = new MimicEditorChild(this);
    w->Show();
}
