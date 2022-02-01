#include "VisualCalc.hpp"

/*!
 * \brief GetAppSkinParams
 * \return
 */
static const NUIE::BasicSkinParams& GetAppSkinParams ()
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
    case NodeType::Integer:
        return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), modelPosition, 0, 5));
    case NodeType::Number:
        return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), modelPosition, 0.0, 5.0));
    case NodeType::IntegerIncrement:
        return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), modelPosition));
    case NodeType::NumberIncrement:
        return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), modelPosition));
    case NodeType::NumberDistribution:
        return NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), modelPosition));
    case NodeType::ListBuilder:
        return NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), modelPosition));
    case NodeType::Addition:
        return NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), modelPosition));
    case NodeType::Subtraction:
        return NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), modelPosition));
    case NodeType::Multiplication:
        return NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), modelPosition));
    case NodeType::Division:
        return NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), modelPosition));
    case NodeType::Color:
        return NUIE::UINodePtr (new ColorNode (NE::LocString (L"Color"), modelPosition));
    case NodeType::Point:
        return NUIE::UINodePtr (new PointNode (NE::LocString (L"Point"), modelPosition));
    case NodeType::Line:
        return NUIE::UINodePtr (new LineNode (NE::LocString (L"Line"), modelPosition));
    case NodeType::Circle:
        return NUIE::UINodePtr (new CircleNode (NE::LocString (L"Circle"), modelPosition));
    case NodeType::Offset:
        return NUIE::UINodePtr (new OffsetNode (NE::LocString (L"Offset"), modelPosition));
    case NodeType::Viewer:
        return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), modelPosition, 5));
    case NodeType::And:
        return NUIE::UINodePtr (new BI::AndNode (NE::LocString (L"And"), modelPosition));
    case NodeType::Or:
        return NUIE::UINodePtr (new BI::OrNode (NE::LocString (L"Or"), modelPosition));
    case NodeType::Xor:
        return NUIE::UINodePtr (new BI::XorNode (NE::LocString (L"Xor"), modelPosition));
    case NodeType::BitAnd:
        return NUIE::UINodePtr (new BI::BitAndNode (NE::LocString (L"Bit And"), modelPosition));
    case NodeType::BitOr:
        return NUIE::UINodePtr (new BI::BitOrNode (NE::LocString (L"Bit Or"), modelPosition));
    case NodeType::BitXOr:
        return NUIE::UINodePtr (new BI::BitXorNode (NE::LocString (L"Bit Xor"), modelPosition));
    case NodeType::GreaterThan:
        return NUIE::UINodePtr (new BI::GreaterThanNode (NE::LocString (L"Greater Than"), modelPosition));
    case NodeType::LessThan:
        return NUIE::UINodePtr (new BI::LessThanNode (NE::LocString (L"Less Than"), modelPosition));
    case NodeType::Equals:
        return NUIE::UINodePtr (new BI::EqualsNode (NE::LocString (L"Equals"), modelPosition));
    case NodeType::NotEquals:
        return NUIE::UINodePtr (new BI::NotEqualsNode (NE::LocString (L"Not Equals"), modelPosition));
    case NodeType::Not:
        return NUIE::UINodePtr (new BI::NotNode (NE::LocString (L"Not"), modelPosition));

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
    static const bool isStressTest = false;
    if (isStressTest) {
        static int count = 10;
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < count; j++) {
                nodeEditor->AddNode (NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Range"), NUIE::Point (i * 150, j * 150))));
            }
        }
        nodeEditor->Update ();
    } else {
        NUIE::UINodePtr startInputNode (new BI::DoubleUpDownNode (NE::LocString (L"Number"), NUIE::Point (70, 70), 20, 5));
        NUIE::UINodePtr stepInputNode (new BI::DoubleUpDownNode (NE::LocString (L"Number"), NUIE::Point (70, 180), 20, 5));
        NUIE::UINodePtr intRangeNodeX (new BI::DoubleIncrementedNode (NE::LocString (L"Increment"), NUIE::Point (220, 100)));
        NUIE::UINodePtr inputNodeY (new BI::DoubleUpDownNode (NE::LocString (L"Number"), NUIE::Point (220, 220), 20, 5));
        std::shared_ptr<PointNode> pointNode (new PointNode (NE::LocString (L"Point"), NUIE::Point (400, 150)));
        NUIE::UINodePtr viewerNode (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), NUIE::Point (600, 150), 5));

        nodeEditor->AddNode (startInputNode);
        nodeEditor->AddNode (stepInputNode);
        nodeEditor->AddNode (intRangeNodeX);
        nodeEditor->AddNode (inputNodeY);
        nodeEditor->AddNode (pointNode);
        nodeEditor->AddNode (viewerNode);

        nodeEditor->ConnectOutputSlotToInputSlot (startInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("start")));
        nodeEditor->ConnectOutputSlotToInputSlot (stepInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("step")));
        nodeEditor->ConnectOutputSlotToInputSlot (intRangeNodeX->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("x")));
        nodeEditor->ConnectOutputSlotToInputSlot (inputNodeY->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("y")));
        nodeEditor->ConnectOutputSlotToInputSlot (pointNode->GetUIOutputSlot (NE::SlotId ("point")), viewerNode->GetUIInputSlot (NE::SlotId ("in")));
        nodeEditor->Update ();
    }
}

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


