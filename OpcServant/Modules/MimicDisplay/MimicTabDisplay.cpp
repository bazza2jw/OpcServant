#include "MimicTabDisplay.h"

/*!
 * \brief MimicTabDisplay::MimicTabDisplay
 * \param parent
 * \param id
 */
MimicTabDisplay::MimicTabDisplay(wxWindow* parent, unsigned id)
    : MimicTabDisplayBase(parent),_id(id)
{
    // load the mimic
    _canvas = new MimicCanvas(this);
    canvasSizer->Add(_canvas, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    _canvas->setEditMode(false);
}
/*!
 * \brief MimicTabDisplay::~MimicTabDisplay
 */
MimicTabDisplay::~MimicTabDisplay()
{
}

/*!
 * \brief MimicTabDisplay::load
 * \param s
 */
void MimicTabDisplay::load()
{
    MRL::RtObjectRef &r = MRL::Common::daq().objects()[_id];
    if(r && _canvas)
    {
        MRL::VariantPropertyTree &t = r->configuration();
        GetTimer()->Stop();
        GetTimer()->Start(t.getInt("/UpdateInterval") * 1000);
        _fileName = MRL::Common::baseDir() + "/mimic/"  + t.getString("/Filename");
        _canvas->readSet(_fileName);
        _canvas->setEditMode(false);
        _loaded = true;
    }
}


/*!
 * \brief MimicTabDisplay::onTimer
 */
void MimicTabDisplay::onTimer(wxTimerEvent& /*event*/)
{
    // drive the mimic updates
    _canvas->objects().process();
    if(!_loaded) load();
    _canvas->Refresh();
}

/*!
 * \brief MimicTabDisplay::onHome
 * \param event
 */
void MimicTabDisplay::onHome(wxCommandEvent& event)
{
    _loaded = false; // force a reload of the root mimic - in principle we could page in various mimics
}
/*!
 * \brief MimicTabDisplay::onBack
 * \param event
 */
void MimicTabDisplay::onBack(wxCommandEvent& event)
{
    // TBD - pop previous mimic
}
