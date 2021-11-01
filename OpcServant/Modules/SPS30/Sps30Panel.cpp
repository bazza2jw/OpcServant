#include "Sps30Panel.h"
#include "sps30rt.h"
/*!
 * \brief Sps30Panel::Sps30Panel
 * \param parent
 * \param id
 */
Sps30Panel::Sps30Panel(wxWindow* parent, unsigned id)
    : Sps30PanelBase(parent),_id(id)
{
    //
    join(MRL::Daq::instance()); // attach to the DAQ manager
    MRL::BObject::setEnabled(true); // enable this object
    //
    GetGrid()->AppendCols(1);
    GetGrid()->AppendRows(10);
    //
    for(int i = 0; i < MRL::SPS30RT::_inputs.size(); i++)
    {
        GetGrid()->SetRowLabelValue(i,_(MRL::SPS30RT::_inputs[i]));
        GetGrid()->SetCellValue(i,0,"0.0");
    }
}
/*!
 * \brief Sps30Panel::~Sps30Panel
 */
Sps30Panel::~Sps30Panel()
{
}

/*!
 * \brief Sps30Panel::processQueueItem
 * \param msg
 * \return
 */
bool Sps30Panel::processQueueItem(const MRL::Message &msg)
{
    if (!MRL::BObject::processQueueItem(msg)) {
        // notifications from DAQ thread
        switch (msg.id()) {
        //
        case MESSAGEID::Started: // object has started
        {
            if(msg.source() == dbId())
            {
            }
        }
        break;

        case MESSAGEID::Stopped: // object has started
        {
            if(msg.source() == dbId())
            {
            }
        }
        break;

        case MESSAGEID::Update_Object:
        {
            if(msg.source() == dbId())
            {
                MRL::RtObjectRef &r =  MRL::Common::daq().objects()[_id];
                for(int i = 0; i < MRL::SPS30RT::_inputs.size(); i++)
                {
                    double v = r->getInput<double>(MRL::SPS30RT::_inputs[i]);
                    wxString s = wxString::Format("%8.1f",v);
                    GetGrid()->SetCellValue(i,0,s);
                }
            }
        }
        break;
        case MESSAGEID::IdleTimeout:

            break;
        // catch the events from the RT object that is running the main board
        //
        default:
            return false;
        }
    }
    return true;
}
