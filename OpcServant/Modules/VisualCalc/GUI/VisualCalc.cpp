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
    case NodeType::Integer:
        return NUIE::UINodePtr (new BI::IntegerNode (NE::LocString (L"Integer"), modelPosition, 1));
    case NodeType::Double:
        return NUIE::UINodePtr (new BI::IntegerNode (NE::LocString (L"Double"), modelPosition, 1.0));
    case NodeType::Timer:
        return NUIE::UINodePtr (new BI::TimerNode (NE::LocString (L"Timer"), modelPosition, 1000 ));
    case NodeType::Event:
        return NUIE::UINodePtr (new BI::EventNode (NE::LocString (L"Event"), modelPosition, false));
    //
    case NodeType::Output:
        return NUIE::UINodePtr (new BI::OutputNode (NE::LocString (L"Ouput"), modelPosition));
    //
    case NodeType::Addition:
        return NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), modelPosition));
    case NodeType::Subtraction:
        return NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), modelPosition));
    case NodeType::Multiplication:
        return NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), modelPosition));
    case NodeType::Division:
        return NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), modelPosition));
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
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Double, NE::LocString (L"Double"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Event, NE::LocString (L"Event"), position)));
        inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Timer, NE::LocString (L"Timer"), position)));
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


        NUIE::MultiMenuCommandPtr otherCommandGroup (new NUIE::MultiMenuCommand (NE::LocString (L"Output Nodes")));
        otherCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (control, CreateNodeCommand::NodeType::Output, NE::LocString (L"Output"), position)));
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

