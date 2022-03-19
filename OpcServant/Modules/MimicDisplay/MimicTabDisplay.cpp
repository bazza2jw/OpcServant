#include "MimicTabDisplay.h"

/*!
 * \brief MimicTabDisplay::MimicTabDisplay
 * \param parent
 * \param id
 */
MimicTabDisplay::MimicTabDisplay(wxWindow* parent, unsigned id)
    : MimicTabDisplayBase(parent), BObject(id)
{
    // load the mimic
    _canvas = new MimicCanvas(this);
    canvasSizer->Add(_canvas, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    join(MRL::Daq::instance()); // attach to the DAQ manager
    MRL::BObject::setEnabled(true); // enable this object
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
    MRL::RtObjectRef &r = MRL::Common::daq().objects()[dbId()];
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
    BObject::process();
    _canvas->Refresh();
}


bool MimicTabDisplay::processQueueItem(const MRL::Message &msg)
{
    if (!MRL::BObject::processQueueItem(msg)) {
        // notifications from DAQ thread
        switch (msg.id()) {
        //
        case MESSAGEID::Started: // object has started
        {
            if(msg.source() == dbId())
            {
                _loaded = false;
            }
            break;

            case MESSAGEID::Stopped: // object has started
            {
                if(msg.source() == dbId())
                {
                }
            }
            break;

            case MESSAGEID::IdleTimeout:

                break;

            case MESSAGEID::Update_Object:
            {
                if(msg.source() == dbId())
                {
                }
            }
            break;

            default:
                return false;
            }
        }
    }
    return true;
}
