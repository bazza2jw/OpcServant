#include "VisualCalc.hpp"
namespace VisualCalc
{
/*!
 * \brief GetAppSkinParams
 * \return
 */
const NUIE::BasicSkinParams& GetAppSkinParams ()
{
    static const NUIE::BasicSkinParams skinParams (
        /*backgroundColor*/ NUIE::Color (255, 255, 255),
        /*connectionLinePen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
        /*connectionMarker */ NUIE::SkinParams::ConnectionMarker::Circle,
        /*connectionMarkerSize*/ NUIE::Size (8.0, 8.0),
        /*nodePadding*/ 5.0,
        /*nodeBorderPen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
        /*nodeHeaderTextFont*/ NUIE::Font (L"Arial", 12.0),
        /*nodeHeaderTextColor*/ NUIE::Color (250, 250, 250),
        /*nodeHeaderErrorTextColor*/ NUIE::Color (250, 250, 250),
        /*nodeHeaderBackgroundColor*/ NUIE::Color (41, 127, 255),
        /*nodeHeaderErrorBackgroundColor*/ NUIE::Color (199, 80, 80),
        /*nodeContentTextFont*/ NUIE::Font (L"Arial", 10.0),
        /*nodeContentTextColor*/ NUIE::Color (0, 0, 0),
        /*nodeContentBackgroundColor*/ NUIE::Color (236, 236, 236),
        /*slotTextColor*/ NUIE::Color (0, 0, 0),
        /*slotTextBackgroundColor*/ NUIE::Color (246, 246, 246),
        /*slotMarker*/ NUIE::SkinParams::SlotMarker::Circle,
        /*hiddenSlotMarker*/ NUIE::SkinParams::HiddenSlotMarker::Arrow,
        /*slotMarkerSize*/ NUIE::Size (8.0, 8.0),
        /*selectionBlendColor*/ NUIE::BlendColor (NUIE::Color (41, 127, 255), 0.25),
        /*disabledBlendColor*/ NUIE::BlendColor (NUIE::Color (0, 138, 184), 0.2),
        /*selectionRectPen*/ NUIE::Pen (NUIE::Color (41, 127, 255), 1.0),
        /*nodeSelectionRectPen*/ NUIE::Pen (NUIE::Color (41, 127, 255), 3.0),
        /*buttonBorderPen*/ NUIE::Pen (NUIE::Color (146, 152, 155), 1.0),
        /*buttonBackgroundColor*/ NUIE::Color (217, 217, 217),
        /*textPanelTextColor*/ NUIE::Color (0, 0, 0),
        /*textPanelBackgroundColor*/ NUIE::Color (236, 236, 236),
        /*groupNameFont*/ NUIE::Font (L"Arial", 12.0),
        /*groupNameColor*/ NUIE::Color (0, 0, 0),
    /*groupBackgroundColors*/ NUIE::NamedColorSet ({
        { NE::LocalizeString (L"Blue"), NUIE::Color (160, 200, 240) },
        { NE::LocalizeString (L"Green"), NUIE::Color (160, 239, 160) },
        { NE::LocalizeString (L"Red"), NUIE::Color (239, 189, 160) }
    }),
    /*groupPadding*/ 12.0
    );
    return skinParams;
}

}

/*!
     * \brief VisualCalc::CreateNodeCommand::CreateNode
     * \param modelPosition
     * \return
     */

NUIE::UINodePtr VisualCalc::CreateNodeCommand::CreateNode (const NUIE::Point& modelPosition)
{
    switch (nodeType) {
    case NodeType::Boolean:
        return NUIE::UINodePtr (new BI::BooleanNode (NE::LocString (L"Boolean"), modelPosition, true));
    case NodeType::Viewer:
        return NUIE::UINodePtr (new BI::OutputNode(NE::LocString (L"Viewer"), modelPosition));
    case NodeType::Addition:
        return NUIE::UINodePtr (new BI::AdditionNode(NE::LocString (L"Addition"), modelPosition));

    default:
        break;
    }
    return nullptr;
}

/*!
 * \brief VisualCalc::NodeEditorEventHandler::OnContextMenu
 * \param type
 * \param position
 * \param commands
 * \return
 */

NUIE::MenuCommandPtr VisualCalc::NodeEditorEventHandler::OnContextMenu (NUIE::EventHandler::ContextMenuType type, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
    if (type == NUIE::EventHandler::ContextMenuType::EmptyArea) {
        NUIE::MenuCommandStructure actualCommands = commands;
        NUIE::MultiMenuCommandPtr createCommandGroup (new NUIE::MultiMenuCommand (NE::LocString (L"Add Node")));

        NUIE::MultiMenuCommandPtr inputCommandGroup (new NUIE::MultiMenuCommand (NE::LocString (L"Input Nodes")));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Boolean, NE::LocString (L"Boolean"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Integer, NE::LocString (L"Integer"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Number, NE::LocString (L"Number"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::String, NE::LocString (L"String"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::IntegerIncrement, NE::LocString (L"Integer Increment"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::NumberIncrement, NE::LocString (L"Number Increment"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::NumberDistribution, NE::LocString (L"Number Distribution"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::ListBuilder, NE::LocString (L"List Builder"), position)));
        createCommandGroup->AddChildCommand (inputCommandGroup);

        NUIE::MultiMenuCommandPtr arithmeticCommandGroup (new NUIE::MultiMenuCommand (NE::LocString (L"Arithmetic Nodes")));
        arithmeticCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Addition, NE::LocString (L"Addition"), position)));
        arithmeticCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Subtraction, NE::LocString (L"Subtraction"), position)));
        arithmeticCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Multiplication, NE::LocString (L"Multiplication"), position)));
        arithmeticCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Division, NE::LocString (L"Division"), position)));
        createCommandGroup->AddChildCommand (arithmeticCommandGroup);

        NUIE::MultiMenuCommandPtr logicCommandGroup(new NUIE::MultiMenuCommand (NE::LocString (L"Logic")));
        logicCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::And, NE::LocString (L"And"), position)));
        logicCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Or, NE::LocString (L"Or"), position)));
        logicCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Xor, NE::LocString (L"Xor"), position)));
        logicCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Not, NE::LocString (L"Not"), position)));
        createCommandGroup->AddChildCommand (logicCommandGroup);

        NUIE::MultiMenuCommandPtr bitCommandGroup(new NUIE::MultiMenuCommand (NE::LocString (L"Bitwise")));
        bitCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::BitAnd, NE::LocString (L"Bit And"), position)));
        bitCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::BitOr, NE::LocString (L"Bit Or"), position)));
        bitCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::BitXOr, NE::LocString (L"Bit XOr"), position)));
        createCommandGroup->AddChildCommand (bitCommandGroup);


        NUIE::MultiMenuCommandPtr compareCommandGroup(new NUIE::MultiMenuCommand (NE::LocString (L"Compare")));
        compareCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::GreaterThan, NE::LocString (L"Greater Than"), position)));
        compareCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::LessThan, NE::LocString (L"Less Than"), position)));
        compareCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Equals, NE::LocString (L"Equals"), position)));
        compareCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::NotEquals, NE::LocString (L"Not Equals"), position)));
        createCommandGroup->AddChildCommand (compareCommandGroup);



        NUIE::MultiMenuCommandPtr drawingCommandGroup (new NUIE::MultiMenuCommand (NE::LocString (L"Drawing Nodes")));
        drawingCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Color, NE::LocString (L"Color"), position)));
        drawingCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Point, NE::LocString (L"Point"), position)));
        drawingCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Line, NE::LocString (L"Line"), position)));
        drawingCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Circle, NE::LocString (L"Circle"), position)));
        createCommandGroup->AddChildCommand (drawingCommandGroup);

        NUIE::MultiMenuCommandPtr transformationCommandGroup (new NUIE::MultiMenuCommand (NE::LocString (L"Transformation Nodes")));
        transformationCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Offset, NE::LocString (L"Offset"), position)));
        createCommandGroup->AddChildCommand (transformationCommandGroup);

        NUIE::MultiMenuCommandPtr runtimeCommandGroup (new NUIE::MultiMenuCommand (NE::LocString (L"Runtime Nodes")));
        runtimeCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::RuntimeInput, NE::LocString (L"Runtime Input"), position)));
        runtimeCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::RuntimeOutput, NE::LocString (L"Runtime Output"), position)));
        createCommandGroup->AddChildCommand (runtimeCommandGroup);


        NUIE::MultiMenuCommandPtr otherCommandGroup (new NUIE::MultiMenuCommand (NE::LocString (L"Other Nodes")));
        otherCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Viewer, NE::LocString (L"Viewer"), position)));
        createCommandGroup->AddChildCommand (otherCommandGroup);

        actualCommands.AddCommand (createCommandGroup);
        return WXAS::SelectCommandFromContextMenu (control, position, actualCommands);
    } else {
        return WXAS::SelectCommandFromContextMenu (control, position, commands);
    }
}

/*!
  * \brief VisualCalc::NodeEditorControl::OnInit
  */

void VisualCalc::NodeEditorControl::OnInit ()
{
    NUIE::UINodePtr boolNode (new BI::BooleanNode (NE::LocString (L"Boolean"), NUIE::Point (600, 150),false));
    NUIE::UINodePtr outNode (new BI::OutputNode (NE::LocString (L"Output"), NUIE::Point (500, 150)));
    nodeEditor->AddNode(boolNode);
    nodeEditor->AddNode(outNode);
    nodeEditor->Update ();
}

#if 0
/*!
* \brief VisualCalc::MenuBar::MenuBar
*/
VisualCalc::MenuBar::MenuBar () :
    wxMenuBar ()
{
    wxMenu* fileMenu = new wxMenu ();
    fileMenu->Append (CommandId::File_New, "New");
    fileMenu->Append (CommandId::File_Open, "Open...");
    fileMenu->Append (CommandId::File_Save, "Save...");
    fileMenu->Append (CommandId::File_SaveAs, "Save As...");
    fileMenu->AppendSeparator ();
    fileMenu->Append (CommandId::File_Exit, L"Exit");
    Append (fileMenu, L"&File");

    wxMenu* editMenu = new wxMenu ();
    editMenu->Append (CommandId::Edit_Undo, "Undo");
    editMenu->Append (CommandId::Edit_Redo, "Redo");
    Append (editMenu, L"&Edit");

    wxMenu* modeMenu = new wxMenu ();
    modeMenu->AppendRadioItem (CommandId::Mode_Automatic, "Automatic");
    modeMenu->AppendRadioItem (CommandId::Mode_Manual, "Manual");
    modeMenu->AppendSeparator ();
    modeMenu->Append (CommandId::Mode_Update, L"Update");
    Append (modeMenu, L"&Mode");

    wxMenu* viewMenu = new wxMenu ();
    viewMenu->Append (CommandId::View_AlignToWindow, "Align To Window");
    viewMenu->Append (CommandId::View_FitToWindow, "Fit To Window");
    Append (viewMenu, L"&View");
}


VisualCalc::MainFrame::MainFrame () :
    wxFrame (NULL, wxID_ANY, L"Node Engine Test App", wxDefaultPosition, wxSize (1000, 600)),
    resultImage (new ResultImage ()),
    evaluationData (new ResultImageEvaluationData (resultImage)),
    evaluationEnv (evaluationData),
    menuBar (new MenuBar ()),
    mainWindow (new wxSplitterWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH | wxSP_LIVE_UPDATE)),
    drawingControl (new DrawingControl (mainWindow, resultImage)),
    nodeEditorControl (new NodeEditorControl (mainWindow)),
    applicationState ()
{
    NE::StringConverterPtr stringConverter (new NE::BasicStringConverter (NE::GetDefaultStringSettings ()));
    NUIE::SkinParamsPtr skinParams (new NUIE::BasicSkinParams (GetAppSkinParams ()));
    NUIE::EventHandlerPtr eventHandler (new NodeEditorEventHandler (nodeEditorControl));
    std::shared_ptr<WXAS::NodeEditorUIEnvironment> uiEnvironment = std::shared_ptr<WXAS::NodeEditorUIEnvironment> (
                new NodeEditorUIEnvironment (
                    nodeEditorControl,
                    drawingControl,
                    stringConverter,
                    skinParams,
                    eventHandler,
                    evaluationEnv
                )
            );

    nodeEditorControl->Init (uiEnvironment);

    SetMenuBar (menuBar);
    UpdateMenuBar ();

    CreateStatusBar ();
    UpdateStatusBar ();

    mainWindow->SetSashGravity (0.5);
    mainWindow->SetMinimumPaneSize (20);
    mainWindow->SplitVertically (nodeEditorControl, drawingControl, 700);
}


void VisualCalc::MainFrame::OnCommand (wxCommandEvent& evt)
{
    MenuBar::CommandId commandId = (MenuBar::CommandId) evt.GetId ();
    switch (commandId) {
    case MenuBar::CommandId::File_New:
    {
        Reset ();
    }
    break;
    case MenuBar::CommandId::File_Open:
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
    break;
    case MenuBar::CommandId::File_Save:
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
    break;
    case MenuBar::CommandId::File_SaveAs:
    {
        wxFileDialog fileDialog (this, L"Save As", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_SAVE);
        if (fileDialog.ShowModal () == wxID_OK) {
            std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
            nodeEditorControl->Save (fileName);
            applicationState.SetCurrentFileName (fileName);
        }
    }
    break;
    case MenuBar::CommandId::File_Exit:
    {
        Close (true);
    }
    break;
    case MenuBar::CommandId::Edit_Undo:
    {
        nodeEditorControl->Undo ();
    }
    break;
    case MenuBar::CommandId::Edit_Redo:
    {
        nodeEditorControl->Redo ();
    }
    break;
    case MenuBar::CommandId::Mode_Automatic:
    {
        nodeEditorControl->SetUpdateMode (WXAS::NodeEditorControl::UpdateMode::Automatic);
    }
    break;
    case MenuBar::CommandId::Mode_Manual:
    {
        nodeEditorControl->SetUpdateMode (WXAS::NodeEditorControl::UpdateMode::Manual);
    }
    break;
    case MenuBar::CommandId::Mode_Update:
    {
        nodeEditorControl->ManualUpdate ();
    }
    break;
    case MenuBar::CommandId::View_AlignToWindow:
    {
        nodeEditorControl->AlignToWindow ();
    }
    break;
    case MenuBar::CommandId::View_FitToWindow:
    {
        nodeEditorControl->FitToWindow ();
    }
    break;
    }
    UpdateMenuBar ();
    UpdateStatusBar ();
}


BEGIN_EVENT_TABLE (VisualCalc::MainFrame, wxFrame)
    EVT_MENU (wxID_ANY, VisualCalc::MainFrame::OnCommand)
END_EVENT_TABLE ()
#endif

