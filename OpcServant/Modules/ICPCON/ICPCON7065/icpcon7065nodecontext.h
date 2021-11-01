#ifndef ICPCON7065NODECONTEXT_H
#define ICPCON7065NODECONTEXT_H
#include <open62541cpp/open62541server.h>
#include <open62541cpp/nodecontext.h>

namespace MRL {
    class ICPCON7065RT;
    /*!
        \brief The ICPCON7065NodeContext class
    */
    class ICPCON7065NodeContext : public Open62541::NodeContext {
            int _rtId = 0; // id of DAQ object that maps on to this context
            int _output = 0;
        public:
            ICPCON7065NodeContext(int id, int out) : Open62541::NodeContext("ICPCON7065Write"), _rtId(id), _output(out) {}

            virtual ~ICPCON7065NodeContext() {}
            /*!
                \brief readData
                \param node
                \param range
                \param value
                \return
            */
            virtual bool readData(Open62541::Server &server,  Open62541::NodeId &node, const UA_NumericRange *range, UA_DataValue &value) ;

            /*!
                \brief writeData
                \param server
                \param node
                \param range
                \param value
                \return
            */
            virtual bool writeData(Open62541::Server &server,  Open62541::NodeId &node, const UA_NumericRange *range, const UA_DataValue &value);

    };
}
#endif // ICPCON7065NODECONTEXT_H
