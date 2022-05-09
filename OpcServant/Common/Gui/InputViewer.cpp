#include "InputViewer.h"
#include <Common/Daq/daqcommon.h>
#include <MrlLib/states.h>
#include <Common/messageids.h>
#include <Common/reporter.h>
#include <Common/reportconfig.h>


int InputViewer::_rId = 0;

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
        const std::string &p = r->pathAsString();
        //
        for(auto i = l.begin(); i != l.end(); i++)
        {
            InputViewerHistory *w = new InputViewerHistory(GetTabs());
             std::string k = p + ":" + *i; // source name in database
            _history[k] = w;
            GetTabs()->AddPage(w, *i);
        }
        fetch(r);
    }
}
/*!
 * \brief InputViewer::~InputViewer
 */
InputViewer::~InputViewer()
{

}

/*!
 * \brief InputViewer::fetch
 */
void InputViewer::fetch(MRL::RtObjectRef &r)
{
    //
    // fetch data from the results database
    // construct the query
    // do the fetch
    // add results to graph
    // add results to table
    //
    MRL::ReportGroup rg;
    rg._end = time(nullptr) - 10;
    rg._start = rg._end - 3600;
    rg._items.resize(r->inputs().size());
    //
    int j = 0;
    //
    for(auto i = _history.begin(); i != _history.end(); i++, j++)
    {
        rg._items[j] = i->first;
    }
    //
    std::string rn = "_T" + std::to_string(_rId++);
    std::string rd = MRL::Common::baseDir() + MRL::ReportConfig::reportDir + "/" + rn;
    if(!wxDir::Exists(rd)) wxDir::Make(rd); // create the directory if necessary
    //
    MRL::Reporter rr(rn, MRL::LocalDb::LOCAL_DB_DIR, MRL::LOCAL_DB_NAME);
    rr.setOutputDir(rd);
    //
    if(rr.lock()) // it is possible for multiple accesses to the same report - eg desktop and web or multiple web accesses
    {
        if(rr.fetch(rg))// generate the tables and the stats to a SQLITE database
        {
           // we have a result set

           // walk it
            for(int i = 0; i < int(rg._items.size()); i++)
            {
                InputViewerHistory *w = _history[rg._items[i]];
                if(w)
                {
                    MRL::ReportItemList l;
                    rr.resultsdb().getPage(rg._items[i],0,l,250);
                    for(auto k = l.begin(); k != l.end(); k++)
                    {
                        MRL::ReporterItem &ri = *k;
                        wxString item = wxString::Format("%s %8.8s %s",ri._timeStamp.FormatISOCombined(' '),ri._state,ri._valueStr);
                        w->GetHistory()->Append(item);
                    }
                    rr.createGraph(rg._items[i], w->GetGraph()->graph());
                }
            }
        }
        rr.unlock();
    }
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
                    const std::string &p = r->pathAsString();
                    std::string k = p + ":" + tag; // source name in database
                    InputViewerHistory *w = _history[k];
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
                        w->GetGraph()->graph().update(k,timeStamp,v); // add the graph lines
                        w->GetGraph()->redraw();
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
