#ifndef VISUALCALC_HPP
#define VISUALCALC_HPP
#include "DrawingControl.hpp"
#include  <VisualCalc/NodeEngine/NE_SingleValues.hpp>
#include <VisualCalc/NodeEngine/NE_Debug.hpp>
#include <VisualCalc/BuiltInNodes/BI_BuiltInNodes.hpp>
#include <VisualCalc/wxWidgetsAppSupport/WXAS_ParameterDialog.hpp>
#include <VisualCalc/wxWidgetsAppSupport/WXAS_NodeEditorControl.hpp>
#include <locale>
#include <codecvt>
#include <wx/wx.h>
#include <wx/splitter.h>
namespace VisualCalc
{
class ApplicationState
{
public:
    ApplicationState () :
        currentFileName ()
    {

    }

    void ClearCurrentFileName ()
    {
        currentFileName.clear ();
    }

    void SetCurrentFileName (const std::wstring& newCurrentFileName)
    {
        currentFileName = newCurrentFileName;
    }

    bool HasCurrentFileName () const
    {
        return !currentFileName.empty ();
    }

    const std::wstring& GetCurrentFileName () const
    {
        return currentFileName;
    }

private:
    std::wstring currentFileName;
};

class CreateNodeCommand : public NUIE::SingleMenuCommand
{
public:
    enum class NodeType
    {
        Boolean,
        Integer,
        Double,
        String,
        Unsigned,
        Timer,
        Event,
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Color,
        Point,
        Line,
        Circle,
        Offset,
        Output,
        And,
        Or,
        Xor,
        BitAnd,
        BitOr,
        BitXOr,
        GreaterThan,
        LessThan,
        Equals,
        NotEquals,
        Not,
    };

    CreateNodeCommand (WXAS::NodeEditorControl* nodeEditorControl, NodeType nodeType, const NE::LocString& name, const NUIE::Point& position) :
        NUIE::SingleMenuCommand (name, false),
        nodeEditorControl (nodeEditorControl),
        nodeType (nodeType),
        position (position)
    {

    }

    virtual bool WillModify () const override
    {
        return true;
    }

    virtual void DoModification () override
    {
        nodeEditorControl->AddNode (CreateNode (nodeEditorControl->ViewToModel (position)));
    }

    NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition);

private:
    WXAS::NodeEditorControl*	nodeEditorControl = nullptr;
    NodeType			nodeType;
    NUIE::Point			position;
};

class NodeEditorEventHandler : public WXAS::NodeEditorEventHandler
{
public:
    NodeEditorEventHandler (WXAS::NodeEditorControl* control) :
        WXAS::NodeEditorEventHandler (control)
    {

    }

    virtual NUIE::MenuCommandPtr OnContextMenu (NUIE::EventHandler::ContextMenuType type, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;

};

class NodeEditorUIEnvironment : public WXAS::NodeEditorUIEnvironment
{
public:
    NodeEditorUIEnvironment (	WXAS::NodeEditorControl* nodeEditorControl,
                                DrawingControl* drawingControl,
                                NE::StringConverterPtr& stringConverter,
                                NUIE::SkinParamsPtr& skinParams,
                                NUIE::EventHandlerPtr& eventHandler,
                                NE::EvaluationEnv& evaluationEnv) :
        WXAS::NodeEditorUIEnvironment (nodeEditorControl, stringConverter, skinParams, eventHandler, evaluationEnv),
        drawingControl (drawingControl)
    {
    }

    virtual void OnValuesRecalculated () override
    {
        drawingControl->RedrawImage ();
    }

private:
    DrawingControl* drawingControl;
};

const NUIE::BasicSkinParams& GetAppSkinParams ();

class NodeEditorControl : public WXAS::NodeEditorControl
{
public:
    NodeEditorControl (wxWindow *parent) :
        WXAS::NodeEditorControl (parent)
    {

    }

    virtual void OnInit () override;

};

class MenuBar : public wxMenuBar
{
public:
    enum CommandId
    {
        File_New			= 1,
        File_Open			= 2,
        File_Save			= 3,
        File_SaveAs			= 4,
        File_Exit			= 5,
        Edit_Undo			= 6,
        Edit_Redo			= 7,
        Mode_Automatic		= 8,
        Mode_Manual			= 9,
        Mode_Update			= 10,
        View_AlignToWindow	= 11,
        View_FitToWindow	= 12
    };

    MenuBar();
    void UpdateStatus (WXAS::NodeEditorControl::UpdateMode updateMode)
    {
        if (updateMode == WXAS::NodeEditorControl::UpdateMode::Automatic) {
            FindItem (CommandId::Mode_Automatic)->Check ();
        } else if (updateMode == WXAS::NodeEditorControl::UpdateMode::Manual) {
            FindItem (CommandId::Mode_Manual)->Check ();
        } else {
            DBGBREAK ();
        }
    }
};

#if 0
class MainFrame : public wxFrame
{
    std::shared_ptr<ResultImage>		resultImage;
    std::shared_ptr<ResultImageEvaluationData>	evaluationData;
    NE::EvaluationEnv				evaluationEnv;

public:
    MainFrame ();
    ~MainFrame ()	{
    }

    void OnCommand (wxCommandEvent& evt);

private:
    void UpdateMenuBar ()
    {
        menuBar->UpdateStatus (nodeEditorControl->GetUpdateMode ());
    }

    void UpdateStatusBar ()
    {
        std::wstring currentFileText = L"No File";
        if (applicationState.HasCurrentFileName ()) {
            currentFileText = applicationState.GetCurrentFileName ();
        }
        SetStatusText (currentFileText);
    }

    void Reset ()
    {
        nodeEditorControl->New ();
        drawingControl->ClearImage ();
        applicationState.ClearCurrentFileName ();
    }

    MenuBar*			menuBar = nullptr;
    wxSplitterWindow*		mainWindow = nullptr;
    DrawingControl*		drawingControl = nullptr;
    WXAS::NodeEditorControl*	nodeEditorControl = nullptr;
    ApplicationState		applicationState;

    DECLARE_EVENT_TABLE ()
};

#endif

}


#endif // VISUALCALC_HPP
