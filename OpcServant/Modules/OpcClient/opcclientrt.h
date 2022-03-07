#ifndef OPCCLIENTRT_H
#define OPCCLIENTRT_H
#include <Common/Daq/rtobject.h>
#include <open62541cpp/open62541client.h>

namespace MRL
{
class OpcClientRT : public RTObject
{
    std::string _host;
    std::string _user;
    std::string _password;
    bool _useSecurity = false;

    UA_UInt32 _subId = 0; // subscription id
    Open62541::Client _client; // the client connection
    Open62541::monitorItemFunc _callback;
    StringVector _inputs; // set of inputs - this is dynamic
    std::map<unsigned, std::string> _monitorNameMap;
public:
    OpcClientRT(unsigned id);
    /*!
        \brief start
    */
    virtual void start();
    /*!
        \brief stop
    */
    virtual void stop();
    /*!
     * \brief useProcessTimer
     * \return
     */
    virtual bool useProcessTimer() {
        return true;   // used to drive state machines
    }
    /*!
        \brief process
    */
    virtual void process();
    /*!
     * \brief hasInputs
     * \return true if has inputs
     */
    virtual bool hasInputs() const { return true;}
    /*!
     * \brief inputs
     * \return array of input names
     */
    virtual StringVector & inputs() { return _inputs;}


};
}
#endif // OPCCLIENTRT_H
