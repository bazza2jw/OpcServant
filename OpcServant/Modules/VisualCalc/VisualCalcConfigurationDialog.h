#ifndef VISUALCALCCONFIGURATIONDIALOG_H
#define VISUALCALCCONFIGURATIONDIALOG_H
#include "VisualCalcUI.h"
#include <Common/common.h>
#include "GUI/VisualCalc.hpp"


class VisualCalcConfigurationDialog : public VisualCalcConfigurationDialogBase
{
    const MRL::PropertyPath & _path;
    //
    std::shared_ptr<ResultImage>		resultImage;
    std::shared_ptr<ResultImageEvaluationData>	evaluationData;
    NE::EvaluationEnv	evaluationEnv;
    DrawingControl*		drawingControl = nullptr;
    WXAS::NodeEditorControl*	nodeEditorControl = nullptr;
    VisualCalc::ApplicationState		applicationState;
    std::shared_ptr<WXAS::NodeEditorUIEnvironment> uiEnvironment;
    //

public:
    VisualCalcConfigurationDialog(wxWindow* parent,const MRL::PropertyPath &path);
    virtual ~VisualCalcConfigurationDialog();
    void Reset ()
    {
        nodeEditorControl->New ();
        drawingControl->ClearImage ();
        applicationState.ClearCurrentFileName ();
        nodeEditorControl->Update();
    }

protected:
    virtual void onNew(wxCommandEvent& event);
    virtual void onRedo(wxCommandEvent& event);
    virtual void onUndo(wxCommandEvent& event);
    virtual void onAutomatic(wxCommandEvent& event);
    virtual void onManual(wxCommandEvent& event);
    virtual void onSave(wxCommandEvent& event);
    virtual void onSaveAs(wxCommandEvent& event);
    virtual void onUpdate(wxCommandEvent& event);
    virtual void onStartTimer(wxTimerEvent& event);
    virtual void onOpen(wxCommandEvent& event);
};
#endif // VISUALCALCCONFIGURATIONDIALOG_H
