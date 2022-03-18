#include "MimicEditorPanel.h"
#include <wx/dnd.h>
#include "mimictype.h"
MimicEditorPanel::MimicEditorPanel(wxWindow* parent)
    : MimicEditorPanelBase(parent)
{

    _canvas = new MimicCanvas(GetSplitterPageCanvas());
    canvasSizer->Add(_canvas, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    // build the item tree
    _root = GetControlList()->AddRoot("Controls") ;
    //
    std::map<std::string, MIMIC::MimicType *> &m = MIMIC::MimicType::map();

    std::set<const char *> classes;
    //
    for(auto i = m.begin(); i != m.end(); i++)
    {
        MIMIC::MimicType *t = i->second;
        if(t)
        {
            const char * c =  t->typeClass();
            classes.insert(c);
        }
    }
    //
    std::map<std::string, wxTreeItemId> chdr;
    for(auto i = classes.begin(); i != classes.end(); i++)
    {
        std::string c(*i);
        chdr[c] = GetControlList()->AppendItem (_root,c); // create the headers
    }
    //
    for(auto i = m.begin(); i != m.end(); i++)
    {
        MIMIC::MimicType *t = i->second;
        if(t)
        {
            std::string c(t->typeClass());
            GetControlList()->AppendItem (chdr[c],t->name());
        }
    }
    Connect(GetControlList()->GetId(),wxEVT_COMMAND_TREE_BEGIN_DRAG,wxTreeEventHandler(MimicEditorPanel::onDragInit),NULL,this);
    GetControlList()->ExpandAll();
}

/*!
 * \brief MimicEditorPanel::~MimicEditorPanel
 */
MimicEditorPanel::~MimicEditorPanel()
{
}

/*!
 * \brief MimicEditorPanel::onDragInit
 * \param event
 */
void MimicEditorPanel::onDragInit(wxTreeEvent& event)
{
    wxString s = GetControlList()->GetItemText(event.GetItem());
    wxTextDataObject tdo(s);
    wxDropSource tds(tdo, GetControlList());
    tds.DoDragDrop(wxDrag_CopyOnly);
}

/*!
 * \brief MimicEditorPanel::onNew
 * \param event
 */
void MimicEditorPanel::onNew(wxCommandEvent& event)
{
    _canvas->newMimic();
}
/*!
 * \brief MimicEditorPanel::onOpen
 * \param event
 */
void MimicEditorPanel::onOpen(wxCommandEvent& event)
{
    _canvas->open();
}
/*!
 * \brief MimicEditorPanel::onSave
 * \param event
 */
void MimicEditorPanel::onSave(wxCommandEvent& event)
{
    _canvas->save();
}
/*!
 * \brief MimicEditorPanel::onSaveAs
 * \param event
 */
void MimicEditorPanel::onSaveAs(wxCommandEvent& event)
{
    _canvas->saveAs();
}
/*!
 * \brief MimicEditorPanel::onAddBackground
 * \param event
 */
void MimicEditorPanel::onAddBackground(wxCommandEvent& event)
{
    wxFileDialog  openFileDialog(this, _("Set Background Image"), "", "", "PNG files (*.png)|*.png", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;     // the user changed idea...
    std::string f = openFileDialog.GetPath().ToStdString();
    _canvas->setBackground(f);

}
