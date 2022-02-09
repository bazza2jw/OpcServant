#include "VisualCalcConfigurationDialog.h"

VisualCalcConfigurationDialog::VisualCalcConfigurationDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : VisualCalcConfigurationDialogBase(parent),_path(path),
      resultImage (new ResultImage ()),
      evaluationData (new ResultImageEvaluationData (resultImage)),
      evaluationEnv (evaluationData)
{


}

VisualCalcConfigurationDialog::~VisualCalcConfigurationDialog()
{
}

void VisualCalcConfigurationDialog::onStartTimer(wxTimerEvent& /*event*/)
{
    drawingControl = new DrawingControl (GetNotebook(), resultImage);
    nodeEditorControl = new VisualCalc::NodeEditorControl (GetNotebook());
    GetNotebook()->AddPage(drawingControl,"Drawing");
    GetNotebook()->AddPage(nodeEditorControl,"Control");
//
    NE::StringConverterPtr stringConverter (new NE::BasicStringConverter (NE::GetDefaultStringSettings ()));
    NUIE::SkinParamsPtr skinParams (new NUIE::BasicSkinParams (VisualCalc::GetAppSkinParams ()));
    NUIE::EventHandlerPtr eventHandler (new VisualCalc::NodeEditorEventHandler (nodeEditorControl));
    //
    uiEnvironment = std::shared_ptr<WXAS::NodeEditorUIEnvironment> (
                        new VisualCalc::NodeEditorUIEnvironment (
                            nodeEditorControl,
                            drawingControl,
                            stringConverter,
                            skinParams,
                            eventHandler,
                            evaluationEnv
                        )
                    );

    nodeEditorControl->Init (uiEnvironment);
//
}
void VisualCalcConfigurationDialog::onAutomatic(wxCommandEvent& event)
{


}
void VisualCalcConfigurationDialog::onManual(wxCommandEvent& event)
{
    if(nodeEditorControl)
    {
        nodeEditorControl->Step();
    }
}
void VisualCalcConfigurationDialog::onOpen(wxCommandEvent& event)
{
    if(nodeEditorControl)
    {
        wxFileDialog fileDialog (this, L"Open", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        if (fileDialog.ShowModal () == wxID_OK) {
            drawingControl->ClearImage ();
            std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
            if (nodeEditorControl->Open (fileName)) {
                applicationState.SetCurrentFileName (fileName);
            } else {
                Reset ();
            }
        }
    }
}

void VisualCalcConfigurationDialog::onSave(wxCommandEvent& event)
{
    if(nodeEditorControl)
    {
        wxFileDialog fileDialog (this, L"Save", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_SAVE);
        if (applicationState.HasCurrentFileName ()) {
            nodeEditorControl->Save (applicationState.GetCurrentFileName ());
        } else if (fileDialog.ShowModal () == wxID_OK) {
            std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
            nodeEditorControl->Save (fileName);
            applicationState.SetCurrentFileName (fileName);
        }
    }
}
void VisualCalcConfigurationDialog::onSaveAs(wxCommandEvent& event)
{
    if(nodeEditorControl)
    {
        wxFileDialog fileDialog (this, L"Save As", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_SAVE);
        if (fileDialog.ShowModal () == wxID_OK) {
            std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
            nodeEditorControl->Save (fileName);
            applicationState.SetCurrentFileName (fileName);
        }
    }
}
void VisualCalcConfigurationDialog::onUpdate(wxCommandEvent& event)
{

}
void VisualCalcConfigurationDialog::onRedo(wxCommandEvent& event)
{
    if(nodeEditorControl)
        nodeEditorControl->Redo ();
}
void VisualCalcConfigurationDialog::onUndo(wxCommandEvent& event)
{
    if(nodeEditorControl)
        nodeEditorControl->Undo ();

}
void VisualCalcConfigurationDialog::onNew(wxCommandEvent& event)
{
    if(nodeEditorControl) Reset();
}
