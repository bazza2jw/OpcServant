#include "httpsendrt.h"

#include <Common/Daq/daqcommon.h>
#include <Common/messageids.h>
#include <Common/stockobjectids.h>
#include <MrlLib/variantpropertytree.h>
#include <Common/objectmanager.h>

/*!
    \brief HttpSendRt
    \param id
*/
MRL::HttpSendRt::HttpSendRt(int id) : RTObject(id) {
    Daq::instance()->publishMessage().connect(this, &HttpSendRt::publishQueueItem);
}
/*!
    \brief start
*/
void MRL::HttpSendRt::start() {
    RTObject::start();
    // get the configuration
    //
    _filter = configuration().getAsString("Filter");
    _url = configuration().getAsString("URL");
    //
    bool ei = configuration().getValue<bool>("Enabled");
    setEnabled(ei);
    //
}
/*!
    \brief stop
*/
void MRL::HttpSendRt::stop() {
    RTObject::stop();
}
/*!
    \brief MRL::HttpSendRt::onOneSecond
    \param t
*/
void  MRL::HttpSendRt::onOneSecond(time_t t) {
    RTObject::onOneSecond(t);
    if (started() && enabled()) {

    }

}


/*!
    \brief MRL::HttpSendRt::process
*/
void MRL::HttpSendRt::process() {
    RTObject::process();
    if (started() && enabled()) {
        // process any waiting input commands
    }
}

/*!
    \brief publishQueueItem
    \param msg
    \return
*/
void  MRL::HttpSendRt::publishQueueItem(const Message &msg) {
    // publish the message
    Message &m = const_cast<Message &>(msg);
    switch (m.id()) {
        case MESSAGEID::Publish_Object: {
            if (enabled()) {
                // unpack the message
                unsigned id = 0;
                int state = 0;
                time_t ts = 0;
                std::string v;
                std::string tag;
                //
                m.get(PARAMETERID::ObjectId, id); // set the source of the message
                m.asString(PARAMETERID::Value, v);
                m.get(PARAMETERID::Tag, tag);
                m.get(PARAMETERID::Status, state);
                m.get(PARAMETERID::Timestamp, ts);
                //
                // get source as string
                //
                // get object type as string
                DataNodeIdMap &m = Common::configuration().idMap();
                DataNode *d  = m[id];
                if (d) {
                    auto o =  ObjectManager::find(d->data().type());
                    if (o) {
                        // filter - either empty string or look for string within source name
                        if ((_filter.length() < 1) || (_filter == "*") || (src.find(_filter) != std::string::npos)) {
                            std::string src = _url;
                            src += "?" + std::to_string(id);
                            // add the tag
                            if (!tag.empty()) {
                                src += ".";
                                src += tag;
                            }
                            src += "=" + v;
                            // now do the transaction

                        }
                    }
                }
            }
        }
        break;
        default:
            break;
    }
}


