#include "InputViewer.h"
#include <Common/Daq/daqcommon.h>
#include <MrlLib/states.h>
#include <Common/messageids.h>

/*!
 * \brief InputViewer::InputViewer
 * \param parent
 */
InputViewer::InputViewer(wxWindow* parent, unsigned id)
    : InputViewerBase(parent), MRL::BObject(id)
{
    join(MRL::Daq::instance()); // attach to the DAQ manager
    MRL::BObject::setEnabled(true); // enable this object
    GetCurrentStatus()->Clear();

    // set up the graph
    // read the database
    // connect for notifications for updates
    MRL::RtObjectRef &r  =  MRL::Common::daq().objects()[id];
    if(r)
    {
        MRL::VariantPropertyTree &t = r->runtime();
        MRL::StringVector &l = r->inputs();
        for(auto i = l.begin(); i != l.end(); i++)
        {
            InputViewerHistory *w = new InputViewerHistory(GetTabs());
            _history[*i] = w;
            GetTabs()->AddPage(w, *i);
        }
    }
}
/*!
 * \brief InputViewer::~InputViewer
 */
InputViewer::~InputViewer()
{

}
/*!
 * \brief InputViewer::processQueueItem
 * \param msg
 * \return
 */
bool InputViewer::processQueueItem(const MRL::Message &msg)
{
    if(msg.source() == dbId())
    {
        if (!MRL::BObject::processQueueItem(msg)) {
            // notifications from DAQ thread
            switch (msg.id()) {
            //
            case MESSAGEID::Started: // object has started
            {
            }
            break;

            case MESSAGEID::Stopped: // object has started
            {
            }
            break;

            case MESSAGEID::IdleTimeout:

                break;

            case MESSAGEID::Update_Value:
            {
                MRL::RtObjectRef &r  =  MRL::Common::daq().objects()[dbId()];
                if(r)
                {
                    // look up the runtime
                    // get input value
                    // add to graph
                    // update history
                    // update current
                    double v;
                    std::string tag;
                    unsigned state;
                    time_t timeStamp;
                    //
                    MRL::Message &mm = const_cast<MRL::Message &>(msg);
                    MRL::MessageVariantMap &m = mm.data();
                    m.get(PARAMETERID::Value, v);
                    m.get(PARAMETERID::Tag,tag);
                    m.get(PARAMETERID::Status, state);
                    m.get(PARAMETERID::Timestamp,timeStamp);
                    //
                    InputViewerHistory *w = _history[tag];
                    //
                    if(w)
                    {
                        wxDateTime tm(timeStamp);
                        wxString sn(STATES::States::toName(state)); // get the state name
                        wxString item = wxString::Format("%s %8.8s %8.1f",tm.FormatISOCombined(' '),sn,v);
                        w->GetHistory()->Append(item);
                        while(w->GetHistory()->GetCount() > MaxHistorySize) {
                            w->GetHistory()->Delete(0);
                        }
                        w->GetGraph()->graph().update(tag,timeStamp,v); // add the graph lines
                        //
                        GetCurrentStatus()->Clear();
                        GetCurrentStatus()->BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
                        GetCurrentStatus()->BeginBold();
                        GetCurrentStatus()->BeginFontSize(20);
                        //
                        GetCurrentStatus()->WriteText(tm.FormatISOCombined(' '));
                        GetCurrentStatus()->WriteText(" ");
                        GetCurrentStatus()->WriteText(sn);
                        GetCurrentStatus()->Newline();
                        GetCurrentStatus()->BeginFontSize(30);
                        GetCurrentStatus()->WriteText(wxString::Format("%8.1f",v));
                        GetCurrentStatus()->EndFontSize();
                        GetCurrentStatus()->Newline();
                        GetCurrentStatus()->EndFontSize();
                        GetCurrentStatus()->EndBold();
                        GetCurrentStatus()->EndAlignment();
                    }
                }
            }
            break;

            default:
                return false;
            }
        }
        return true;
    }
    return false;
}

void InputViewer::DriveProcess(wxTimerEvent&)
{
    MRL::BObject::process();
}
