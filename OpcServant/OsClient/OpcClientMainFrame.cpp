#include "OpcClientMainFrame.h"
#include "osclientapp.h"
/*!
 * \brief OpcClientMainFrame::OpcClientMainFrame
 * \param parent
 */
OpcClientMainFrame::OpcClientMainFrame(wxWindow* parent)
    : OpcClientMainFrameBase(parent)
{
    Maximize(true);
    GetStatusGrid()->SetDefaultColSize(100,true);
    GetStatusGrid()->SetDefaultRowSize(100,true);
    GetStatusGrid()->AppendCols(5);
    GetStatusGrid()->AppendRows(5);
}

/*!
 * \brief OpcClientMainFrame::~OpcClientMainFrame
 */
OpcClientMainFrame::~OpcClientMainFrame()
{

}
/*!
 * \brief OpcClientMainFrame::OnExit
 */
void OpcClientMainFrame::OnExit(wxCommandEvent& /*event*/)
{
     MRL::OsClientApp::GetInstance()->Exit();
}
/*!
 * \brief OpcClientMainFrame::OnOpen
 * \param event
 */
void OpcClientMainFrame::OnOpen(wxCommandEvent& /*event*/)
{
    // open a configuration
}
/*!
 * \brief OpcClientMainFrame::OnRestore
 * \param event
 */
void OpcClientMainFrame::OnRestore(wxCommandEvent& /*event*/)
{
    //
}
/*!
 * \brief OpcClientMainFrame::OnSave
 * \param event
 */
void OpcClientMainFrame::OnSave(wxCommandEvent& /*event*/)
{

}
/*!
 * \brief OpcClientMainFrame::OnSaveAs
 * \param event
 */
void OpcClientMainFrame::OnSaveAs(wxCommandEvent& /*event*/)
{
    // ask for save name
}
/*!
 * \brief OpcClientMainFrame::OnSetup
 * \param event
 */
void OpcClientMainFrame::OnSetup(wxCommandEvent& /*event*/)
{
}
/*!
 * \brief OpcClientMainFrame::onAddReportDataSource
 * \param event
 */
void OpcClientMainFrame::onAddReportDataSource(wxCommandEvent& /*event*/)
{

}
/*!
 * \brief OpcClientMainFrame::onMakeReport
 * \param event
 */
void OpcClientMainFrame::onMakeReport(wxCommandEvent& /*event*/)
{

}
/*!
 * \brief OpcClientMainFrame::onNewReport
 * \param event
 */
void OpcClientMainFrame::onNewReport(wxCommandEvent& /*event*/)
{
}
/*!
 * \brief OpcClientMainFrame::onRemoveReport
 * \param event
 */
void OpcClientMainFrame::onRemoveReport(wxCommandEvent& /*event*/)
{

}
/*!
 * \brief OpcClientMainFrame::onRemoveReportDataSource
 * \param event
 */
void OpcClientMainFrame::onRemoveReportDataSource(wxCommandEvent& /*event*/)
{
}
/*!
 * \brief OpcClientMainFrame::onReportSelect
 * \param event
 */
void OpcClientMainFrame::onReportSelect(wxCommandEvent& /*event*/)
{

}
/*!
 * \brief OpcClientMainFrame::onViewReport
 * \param event
 */
void OpcClientMainFrame::onViewReport(wxCommandEvent& /*event*/)
{
}
/*!
 * \brief OpcClientMainFrame::onClearMessages
 * \param event
 */
void OpcClientMainFrame::onClearMessages(wxCommandEvent& /*event*/)
{
    GetListMessages()->Clear();
}
