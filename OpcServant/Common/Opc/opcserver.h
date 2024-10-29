#ifndef OPCSERVER_H
#define OPCSERVER_H
#include <open62541cpp/open62541server.h>
#include <Common/messages.h>
#include <Common/bobject.h>
#include <open62541cpp/historydatabase.h>
#include <MrlLib/threadHelper.h>

namespace MRL {
    class OpcServer;
    constexpr int DefaultHistorySize = 1500; // default buffer size
    constexpr int HistoryNodeSizeMargin = 100; // add this number to the node cache size to allow for extra nodes beinge added
    /*!
     * \brief The OpcServerHandler class
     */
    class OpcServerHandler : public BObject {
            std::map<unsigned,bool> _nodeMap; // track if an object has been added to OPC
            OpcServer &_server;
        public:
            OpcServerHandler(OpcServer &s) : _server(s)
            {
                setEnabled(true);
            }
            bool processQueueItem(const Message &m);
            void addObject(unsigned id);
            void removeObject(unsigned id);
            void updateObject(unsigned id);
            //
            void setNode(unsigned i, bool f = true)
            {
                _nodeMap[i] = f;
            }
            bool isMapped(unsigned i)
            {
                if(_nodeMap.find(i) != _nodeMap.end()) return _nodeMap[i];
                return false;
            }
    };

    /*!
           \brief The OpcServer class
    */
    class OpcServer : public Open62541::Server {
            unsigned _idx = 2; // application name space
            //
            // top level OPC folders

            Open62541::NodeId _dataFolder;
            Open62541::NodeId _controlFolder;
            //
            OpcServerHandler _handler; // message handler
            //
            std::unique_ptr<Open62541::MemoryHistorian> _historian; // the history database
            //
            int _port = 4840;
            std::string _nameSpace;
            std::string _username;
            std::string _password;
            //

        public:
            /*!
             * \brief OpcServer
             * \param thread
             */
            OpcServer(int port = 4840);
            // destructor
            ~OpcServer();

            /*!
             * \brief setHistorian - configures the historian
             * \param numberNodes
             * \param maxValuesPerNode
             */
            void setHistorian(size_t numberNodes = 100, size_t maxValuesPerNode = 1500)
            {
                // 1500 allows for one day's data for a once per minute update
               _historian.reset(new Open62541::MemoryHistorian(numberNodes,maxValuesPerNode));
               // Enable server as historian - must be done when starting server
               serverConfig().historyDatabase = _historian->database();
               serverConfig().accessHistoryDataCapability = UA_TRUE;
            }
            /*!
             * \brief initialise
             */
            void initialise();    // initalise the server

            /*!
             * \brief terminate
             */
            void terminate()
            {
                serverConfig().accessHistoryDataCapability = UA_FALSE;
                Open62541::Server::terminate();
                _historian.reset(nullptr);
            }
            /*!
             * \brief process
             */
            void process(); // process events when safe to do so

            /*!
             * \brief applicationNamespace
             */
            unsigned applicationNamespace() {
                return _idx;
            }

            /*!
             * \brief handler
             * \return
             */
            OpcServerHandler &handler() {
                return _handler;
            }


            /*!
             * \brief historian
             * \return
             */
            Open62541::MemoryHistorian &historian()
            {
                if(!_historian) throw std::runtime_error("Historian Not Set");
                return  *_historian;
            }
            /*!
             * \brief itemNamespace
             */
            unsigned itemNamespace()
            {
                return _idx;
            }
    };
}
#endif // OPCSERVER_H
