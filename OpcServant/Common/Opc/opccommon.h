#ifndef OPCCOMMON_H
#define OPCCOMMON_H

#include <open62541cpp/open62541server.h>
#include <open62541cpp/servermethod.h>
#include <open62541cpp/serverobjecttype.h>
#include <open62541cpp/historydatabase.h>
#include "opcserver.h"
#include <wx/msgqueue.h>

namespace MRL {




    /*!
        \brief The OpcCommon class
        register type object factories and server methods
    */
    typedef std::unique_ptr<Open62541::ServerObjectType> ServerObjectTypeRef;
    typedef std::map<std::string, ServerObjectTypeRef > ServerObjectTypeMap;
    typedef std::unique_ptr<Open62541::ServerMethod>  ServerMethodPtr;
    typedef std::map<std::string, ServerMethodPtr> ServerMethodMap;
    //
    class OpcCommon {
            ServerObjectTypeMap _typeMap; //!< set of type object
            ServerMethodMap _methodMap; //!< set of methods
            OpcServer *_server = nullptr;
            int _nameSpaceIndex = 2;
            bool _active = false; //!< true if the OPC server is active
        public:
            OpcCommon()
            {
            }

            bool active() const {
                if(_server)
                {
                    return _server->running();
                }
                return false;
            }
            /*!
                \brief nameSpaceIndex
                \return
            */
            int nameSpaceIndex() const {
                return _nameSpaceIndex;
            }

            void setNameSpaceIndex(int i)  {
                _nameSpaceIndex = i;
            }


            /*!
                \brief clear
            */
            void clear() {
                _typeMap.clear();
                _methodMap.clear();
            }

            /*!
                \brief add
                \param n
                \param p
            */
            template <typename T>
            void add(const std::string &n) {
                ServerObjectTypeRef r(new T(server(), n));
                std::string nm = "OpcServantType." + n;
                Open62541::NodeId tn(nameSpaceIndex(), nm); // give it an easy to find name
                r->setNameSpace(nameSpaceIndex());
                r->addType(tn); // add to OPC Server types
                _typeMap[n] = std::move(r);

            }

            /*!
                \brief add
                \param n
                \param p
            */
            void add(const std::string &n, Open62541::ServerMethod *p) {
                ServerMethodPtr r(p);
                _methodMap[n] = std::move(r);
            }


            /*!
                \brief typeMap
                \return
            */
            ServerObjectTypeMap &typeMap() {
                return _typeMap;
            }

            /*!
                \brief methodMap
                \return
            */
            ServerMethodMap &methodMap() {
                return _methodMap;
            }

            /*!
                \brief type
                \param n
                \return
            */
            Open62541::ServerObjectType *type(const std::string &n) {
                if(_typeMap.find(n) != _typeMap.end()) return _typeMap[n].get();
                return nullptr;
            }

            /*!
                \brief method
                \param n
                \return
            */
            Open62541::ServerMethod *method(const std::string &n) {
                if(_methodMap.find(n) != _methodMap.end()) return _methodMap[n].get();
                return nullptr;
            }

            //accessors
            OpcServer &server() const {
                wxASSERT(_server);
                return *_server;
            }
            void setServer(OpcServer *s) {
                _server = s;
            }




    };

}
#endif // OPCCOMMON_H
