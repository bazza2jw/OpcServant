#include "opcclientrt.h"
#include <open62541cpp/monitoreditem.h>
/*!
 * \brief MRL::OpcClientRT::OpcClientRT
 * \param id
 */
MRL::OpcClientRT::OpcClientRT(unsigned id) : RTObject(id),
    _callback([this](Open62541::ClientSubscription& c, Open62541::MonitoredItem * m, UA_DataValue* v) {
    if(m && (c.id() == _subId))
    {
        std::string sid = _monitorNameMap[m->id()];
        if(v->hasValue)
        {
            RTObject::publish();
            Message m(MESSAGEID::Publish_Object,0,dbId());
            Open62541::Variant var;
            var.assignFrom(v->value);
            this->updateValue(sid,var);
            this->toUpdateValue(m,sid,var);
            this->updateObject();
            // now post the value
            message().emit(m);
        }
    }
})
{

}

/*!
    \brief start
*/
void MRL::OpcClientRT::start()
{
    MRL::RTObject::start();
    _host =  configuration().getString("/ServerHost");
    _user = configuration().getString("/Username");
    _password = configuration().getString("/Password");
    _useSecurity = configuration().getBool("/UseSecurity");
    //
    _inputs.clear();
    MRL::PropertyPath lp;
    lp.push_back("Subs");
    MRL::StringList l;
    configuration().listChildren("/Subs",l);
    for(auto i = l.begin(); i != l.end(); i++)
    {
        _inputs.push_back(*i);
    }
    //
    _state =  STATE_CONNECTING;
}
/*!
    \brief stop
*/
void MRL::OpcClientRT::stop()
{
    MRL::RTObject::stop();
// close the client
    if(_subId > 0) _client.removeSubscription(_subId);
    _subId = 0;
    _client.disconnect();
    _state = STATE_IDLE;
}
/*!
    \brief process
*/
void MRL::OpcClientRT::process()
{
    MRL::RTObject::process();
    if(started())
    {
        switch (_state) {
        case STATE_IDLE:
            break;
        case STATE_CONNECTING:
        {

            bool res = _useSecurity?_client.connectUsername(_host,_user,_password):_client.connect(_host);
            if(res)
            {
                //
                Open62541::UANodeTree _tree(Open62541::NodeId::Objects); // browse the objects at the server end
                _client.browseTree(Open62541::NodeId::Objects,_tree); // browse the node tree and cache it
                //
                // create the subscriptions
                //
                MRL::StringList l;
                configuration().listChildren("/Subs",l);
                //
                _subId = 0;
                _client.addSubscription(_subId); // create the subscription
                //
                MRL::PropertyPath lp;
                lp.push_back("Subs");
                // seek path
                for(auto i = l.begin(); i != l.end(); i++)
                {
                    //
                    std::string s = configuration().getValue<std::string>(lp,*i);
                    //
                    MRL::PropertyPath p;
                    p.toList(s); // parse
                    Open62541::UANodeTree::Path ps; // copy to opc path (really the same)
                    for(auto i = p.begin(); i != p.end(); i++) ps.push_back(*i);
                    //
                    if(_tree.exists(ps))
                    {
                        Open62541::NodeId &nodeId = _tree.get(ps);
                        //
                        Open62541::ClientSubscription * cs = _client.subscription(_subId);
                        if(cs)
                        {
                            unsigned mdc = cs->addMonitorNodeId(_callback, nodeId);  // returns monitor id
                            if (!mdc) {
                                wxLogDebug("Failed to add monitor data change" );
                            }
                            else
                            {
                                _monitorNameMap[mdc] = *i;
                            }
                        }
                        else
                        {
                            wxLogDebug("Failed to find subscription" );
                        }
                    }
                    else
                    {
                        wxLogDebug("Failed to find node");
                    }
                }
            }
            else
            {
                _time.Start();
                _state = STATE_WAIT;
            }
        }
            break;
        case STATE_CONNECTED:
            if(_client.getConnectStatus() == UA_SESSIONSTATE_CLOSED)
            {
                // become disconnected
                _client.disconnect();
                _state = STATE_WAIT;
                _time.Start();
            }
            else
            {
                _client.runIterate(10); // drive the client
            }
            break;
        case STATE_WAIT:
            if(_time.Time() > 5000) _state = STATE_CONNECTING; // throttle the reconnect attempts
            break;
        default:
            break;
        }
    }
}


