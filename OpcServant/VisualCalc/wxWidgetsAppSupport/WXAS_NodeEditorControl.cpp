#include "WXAS_NodeEditorControl.hpp"
#include "WXAS_ContextFactory.hpp"
#include "WXAS_ParameterDialog.hpp"

#include <wx/menu.h>

namespace WXAS
{

NodeEditorEventHandler::NodeEditorEventHandler (NodeEditorControl* control) :
    control (control)
{

}

NodeEditorEventHandler::~NodeEditorEventHandler ()
{

}

NUIE::MenuCommandPtr NodeEditorEventHandler::OnContextMenu (NUIE::EventHandler::ContextMenuType, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
    return SelectCommandFromContextMenu (control, position, commands);
}

bool NodeEditorEventHandler::OnParameterSettings (NUIE::EventHandler::ParameterSettingsType, NUIE::ParameterInterfacePtr paramInterface)
{
    ParameterDialog paramDialog (control, paramInterface);
    if (paramDialog.ShowModal () == wxID_OK) {
        return true;
    }
    return false;
}

void NodeEditorEventHandler::OnDoubleClick (const NUIE::Point&, NUIE::MouseButton)
{

}

NodeEditorUIEnvironment::NodeEditorUIEnvironment (	NodeEditorControl* nodeEditorControl,
        NE::StringConverterPtr& stringConverter,
        NUIE::SkinParamsPtr& skinParams,
        NUIE::EventHandlerPtr& eventHandler,
        NE::EvaluationEnv& evaluationEnv) :
    nodeEditorControl (nodeEditorControl),
    evaluationEnv (evaluationEnv),
    stringConverter (stringConverter),
    skinParams (skinParams),
    eventHandler (eventHandler),
    clipboardHandler (new NUIE::MemoryClipboardHandler ()),
    drawingContext (CreateNativeDrawingContext ())
{
    try {
        drawingContext->Init (GetNativeHandle (nodeEditorControl));
    }
    catch(...)
    {
        DbgBreak();
    }

}

NodeEditorUIEnvironment::~NodeEditorUIEnvironment ()
{

}

void NodeEditorUIEnvironment::OnPaint (wxPanel*, wxPaintEvent&)
{
    try
    {
        drawingContext->BlitToWindow (GetNativeHandle (nodeEditorControl));
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorUIEnvironment::OnResize (int width, int height)
{
    try
    {
        drawingContext->Resize (width, height);
    }
    catch(...)
    {
        DbgBreak();
    }

}

const NE::StringConverter& NodeEditorUIEnvironment::GetStringConverter ()
{
    return *stringConverter;
}

const NUIE::SkinParams& NodeEditorUIEnvironment::GetSkinParams ()
{
    return *skinParams;
}

NUIE::DrawingContext& NodeEditorUIEnvironment::GetDrawingContext ()
{
    return *drawingContext;
}

double NodeEditorUIEnvironment::GetWindowScale ()
{
    return 1.0;
}

NE::EvaluationEnv& NodeEditorUIEnvironment::GetEvaluationEnv ()
{
    return evaluationEnv;
}

void NodeEditorUIEnvironment::OnEvaluationBegin ()
{
    wxBeginBusyCursor ();
}

void NodeEditorUIEnvironment::OnEvaluationEnd ()
{
    wxEndBusyCursor ();
}

void NodeEditorUIEnvironment::OnValuesRecalculated ()
{

}

void NodeEditorUIEnvironment::OnRedrawRequested ()
{
    try
    {
        nodeEditorControl->Refresh (false);
    }
    catch(...)
    {
        DbgBreak();
    }

}

NUIE::EventHandler& NodeEditorUIEnvironment::GetEventHandler ()
{
    return *eventHandler;
}

NUIE::ClipboardHandler& NodeEditorUIEnvironment::GetClipboardHandler ()
{
    return *clipboardHandler;
}

void NodeEditorUIEnvironment::OnSelectionChanged (const NUIE::Selection&)
{

}

void NodeEditorUIEnvironment::OnUndoStateChanged (const NUIE::UndoState&)
{

}

void NodeEditorUIEnvironment::OnClipboardStateChanged (const NUIE::ClipboardState&)
{

}

void NodeEditorUIEnvironment::OnIncompatibleVersionPasted (const NUIE::Version&)
{

}

NodeEditorControl::NodeEditorControl (wxWindow *parent) :
    wxPanel (parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS),
    captureHandler (this)
{

}

NodeEditorControl::~NodeEditorControl ()
{

}

void NodeEditorControl::Init (std::shared_ptr<NodeEditorUIEnvironment>& editorUIEnvironment)
{
    try {
        uiEnvironment = editorUIEnvironment;
        nodeEditor = std::shared_ptr<NUIE::NodeEditor> (new NUIE::NodeEditor (*uiEnvironment));
        OnInit ();
        nodeEditor->Update ();
    }
    catch(...)
    {
        DbgBreak();
    }

}


void  NodeEditorControl::Step()
{
    try
    {
        nodeEditor->UiManager().Step(uiEnvironment->GetEvaluationEnv()); // one iteration of the node flows
    }
    catch(...)
    {
        DbgBreak();
    }

}


void NodeEditorControl::OnInit ()
{

}

void NodeEditorControl::OnPaint (wxPaintEvent& evt)
{
    try {
        nodeEditor->Draw ();
        uiEnvironment->OnPaint (this, evt);
    } catch (...) {
        wxTrap();
    }
}

void NodeEditorControl::OnResize (wxSizeEvent& evt)
{
    try {
        SetFocus ();
        wxSize size = evt.GetSize ();
        nodeEditor->OnResize (size.GetWidth (), size.GetHeight ());
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnMouseCaptureLost (wxMouseCaptureLostEvent&)
{
    try {
        captureHandler.OnCaptureLost ();
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnLeftButtonDown (wxMouseEvent& evt)
{
    try {
        captureHandler.OnMouseDown ();
        nodeEditor->OnMouseDown (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Left, evt.GetX (), evt.GetY ());
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnLeftButtonUp (wxMouseEvent& evt)
{
    try
    {
        nodeEditor->OnMouseUp (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Left, evt.GetX (), evt.GetY ());
        captureHandler.OnMouseUp ();
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnLeftButtonDoubleClick (wxMouseEvent& evt)
{
    try
    {
        nodeEditor->OnMouseDoubleClick (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Left, evt.GetX (), evt.GetY ());
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnRightButtonDown (wxMouseEvent& evt)
{
    try
    {
        captureHandler.OnMouseDown ();
        nodeEditor->OnMouseDown (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Right, evt.GetX (), evt.GetY ());
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnRightButtonUp (wxMouseEvent& evt)
{
    try
    {
        nodeEditor->OnMouseUp (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Right, evt.GetX (), evt.GetY ());
        captureHandler.OnMouseUp ();
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnRightButtonDoubleClick (wxMouseEvent& evt)
{
    try {
        nodeEditor->OnMouseDoubleClick (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Right, evt.GetX (), evt.GetY ());
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnMiddleButtonDown (wxMouseEvent& evt)
{
    try {
        captureHandler.OnMouseDown ();
        nodeEditor->OnMouseDown (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Middle, evt.GetX (), evt.GetY ());
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnMiddleButtonUp (wxMouseEvent& evt)
{
    try {
        nodeEditor->OnMouseUp (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Middle, evt.GetX (), evt.GetY ());
        captureHandler.OnMouseUp ();
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnMiddleButtonDoubleClick (wxMouseEvent& evt)
{
    try {
        nodeEditor->OnMouseDoubleClick (GetModiferKeysFromEvent (evt), NUIE::MouseButton::Middle, evt.GetX (), evt.GetY ());
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnMouseMove (wxMouseEvent& evt)
{
    try {
        SetFocus ();
        nodeEditor->OnMouseMove (GetModiferKeysFromEvent (evt), evt.GetX (), evt.GetY ());

    } catch (...) {
        wxTrap();
    }
}

void NodeEditorControl::OnMouseWheel (wxMouseEvent& evt)
{
    try {
        NUIE::MouseWheelRotation rotation = evt.GetWheelRotation () > 0 ? NUIE::MouseWheelRotation::Forward : NUIE::MouseWheelRotation::Backward;
        nodeEditor->OnMouseWheel (GetModiferKeysFromEvent (evt), rotation, evt.GetX (), evt.GetY ());
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::OnKeyDown (wxKeyEvent& evt)
{
    try {
        NUIE::CommandCode commandCode = GetCommandFromEvent (evt);
        if (commandCode == NUIE::CommandCode::Undefined) {
            return;
        }
        nodeEditor->ExecuteCommand (commandCode);
    }
    catch(...)
    {
        DbgBreak();
    }

}

NodeEditorControl::UpdateMode NodeEditorControl::GetUpdateMode () const
{
    try {
        if (nodeEditor->GetUpdateMode () == NUIE::NodeEditor::UpdateMode::Automatic) {
            return UpdateMode::Automatic;
        } else if (nodeEditor->GetUpdateMode () == NUIE::NodeEditor::UpdateMode::Manual) {
            return UpdateMode::Manual;
        } else {
            DBGBREAK ();
            return UpdateMode::Automatic;
        }
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::SetUpdateMode (UpdateMode mode) const
{
    try {
        if (mode == UpdateMode::Automatic) {
            nodeEditor->SetUpdateMode (NUIE::NodeEditor::UpdateMode::Automatic);
        } else if (mode == UpdateMode::Manual) {
            nodeEditor->SetUpdateMode (NUIE::NodeEditor::UpdateMode::Manual);
        } else {
            DBGBREAK ();
        }
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::ManualUpdate ()
{
    nodeEditor->ManualUpdate ();
}

void NodeEditorControl::AddNode (const NUIE::UINodePtr& uiNode)
{
    nodeEditor->AddNode (uiNode);
}

NUIE::Point NodeEditorControl::ViewToModel (const NUIE::Point& viewPoint) const
{
    try {
        return nodeEditor->ViewToModel (viewPoint);
    }
    catch(...)
    {
        DbgBreak();
    }

}

void NodeEditorControl::AlignToWindow ()
{
    nodeEditor->AlignToWindow ();
}

void NodeEditorControl::FitToWindow ()
{
    nodeEditor->FitToWindow ();
}

void NodeEditorControl::New ()
{
    nodeEditor->New ();
}

bool NodeEditorControl::Open (const std::wstring& fileName)
{
    return nodeEditor->Open (fileName);
}

bool NodeEditorControl::Open (NE::InputStream& inputStream)
{
    return nodeEditor->Open (inputStream);
}

bool NodeEditorControl::Save (const std::wstring& fileName)
{
    return nodeEditor->Save (fileName);
}

bool NodeEditorControl::Save (NE::OutputStream& outputStream) const
{
    return nodeEditor->Save (outputStream);
}

bool NodeEditorControl::NeedToSave () const
{
    return nodeEditor->NeedToSave ();
}

void NodeEditorControl::Undo ()
{
    nodeEditor->Undo ();
}

void NodeEditorControl::Redo ()
{
    nodeEditor->Redo ();
}

BEGIN_EVENT_TABLE(NodeEditorControl, wxPanel)

    EVT_PAINT (NodeEditorControl::OnPaint)
    EVT_SIZE (NodeEditorControl::OnResize)
    EVT_MOUSE_CAPTURE_LOST (NodeEditorControl::OnMouseCaptureLost)

    EVT_LEFT_DOWN (NodeEditorControl::OnLeftButtonDown)
    EVT_LEFT_UP (NodeEditorControl::OnLeftButtonUp)
    EVT_LEFT_DCLICK (NodeEditorControl::OnLeftButtonDoubleClick)

    EVT_RIGHT_DOWN (NodeEditorControl::OnRightButtonDown)
    EVT_RIGHT_UP (NodeEditorControl::OnRightButtonUp)
    EVT_RIGHT_DCLICK (NodeEditorControl::OnRightButtonDoubleClick)

    EVT_MIDDLE_DOWN (NodeEditorControl::OnMiddleButtonDown)
    EVT_MIDDLE_UP (NodeEditorControl::OnMiddleButtonUp)
    EVT_MIDDLE_DCLICK (NodeEditorControl::OnMiddleButtonDoubleClick)

    EVT_MOUSEWHEEL (NodeEditorControl::OnMouseWheel)
    EVT_MOTION (NodeEditorControl::OnMouseMove)
    EVT_KEY_DOWN (NodeEditorControl::OnKeyDown)

END_EVENT_TABLE ()

}
