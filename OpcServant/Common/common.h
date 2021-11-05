/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided with open62541.
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef COMMON_H
#define COMMON_H
#include "common_global.h"
#include <MrlLib/mrllib.h>
#include <Common/database.h>
#include <MrlLib/anypropertytree.h>
#include <Common/displaypropertytree.h>
#include <Wt/WObject.h>
#include <Wt/WWidget.h>
#include <MrlLib/mrlsignals.h>
#include <Common/messages.h>
#include <wx/html/htmprint.h>

#ifndef OPCSERVANT_DEF_APP_DIR
#define OPCSERVANT_DEF_APP_DIR "/usr/local/OpcServant"
#endif

namespace MRL {

    class DaqCommon;
    class OpcCommon;
    constexpr const char *DEFAULT_APPLICATION_DIR = OPCSERVANT_DEF_APP_DIR;
    constexpr const char *DEFAULT_SETTINGS_FILE = "data/settings.dat";
    //
    /*!
     * \brief The LogMessage struct
     */
    struct LogMessage {
        std::string _state; // message alarm state - include states.h for possible values
        time_t _ts; // time stamp
        std::string _source; // message source
        std::string _msg; // message
        LogMessage(const std::string &s, const std::string &m, const std::string &st = "Ok") :
            _state(st), _source(s), _msg(m) {
            _ts = wxDateTime::GetTimeNow();
        }
        LogMessage(const LogMessage &) = default;
    };

    /*!
     * \brief The Common class
     */
    class Common : public Wt::WObject {
            //

            static wxHtmlEasyPrinting  * _htmlPrinter;
            VariantPropertyTree _settings; // Settings - core configuration
            AnyPropertyTree _globals; // application global values - variants can hold any object
            static Common *_instance;  // singleton instance
            std::string _configFileName = DEFAULT_SETTINGS_FILE;
            std::string _baseDir = DEFAULT_APPLICATION_DIR;
            MRL::Database _db; //!< the configuration database
            VariantPropertyTree  _runtime; //!< tree of runtime data
            OpcCommon *_opc = nullptr;  //!< OPC shared data
            std::unique_ptr<DataItemTree> _display; // collection of models and tree to handle web and gui display
            //
            DaqCommon *_daq = nullptr;  //!< DAQ common data
            //
            MessageQueue _guiQueue; // the queue for messages sent to the GUI and Web threads
            //
            bool _enableGui = false; // which sub systems are enabled - No GUI if running as deamon
            bool _enableWeb = false; // Web application
            //
            // could use a bimap
            static StringMap _aliasMap;
            static StringMap _reverseAliasMap;
            //
            bool _enableDiagnosticLog = false; // true if diagnostic messages are to be logged
            //

        public:
            /*!
                \brief Common
            */
            Common() {
                _instance = this;
            }
            virtual ~Common();

            /*!
                \brief clear
            */
            static void clear();

            /*!
                \brief initialise
                \param dir
                \param f
                \return true on success
            */
            static bool initialise(const std::string &dir = DEFAULT_APPLICATION_DIR,
                                   const std::string &f = DEFAULT_SETTINGS_FILE);


            static wxHtmlEasyPrinting  * htmlPrinter()  { return   _htmlPrinter;}


            /*!
                \brief display
                \return
            */
            static DataItemTree &display() {
                return *(instance()->_display);
            }

            //
            // Accessors
            //
            /*!
             * \brief enableDiagnoticLog
             * \param f
             */
            void setEnableDiagnoticLog(bool f = true)
            {
                _enableDiagnosticLog = f;
            }

            bool enableDiagnoticLog() const
            {
                return _enableDiagnosticLog;
            }

            static VariantPropertyTree &runTime() {
                return instance()->_runtime;
            }

            /*!
             * \brief objectType
             * \param id
             */
            static unsigned objectType(unsigned id)  {
                DataNodeIdMap &m = Common::configuration().idMap();
                DataNode *d  = m[id];
                return d?d->data().type():0;
            }

            static VariantPropertyTree &settings() {
                return  instance()->_settings;
            }
            /*!
                \brief globals
                \return
            */
            static AnyPropertyTree &globals() {
                return  instance()->_globals;
            }
            /*!
                \brief instance
                \return
            */
            static Common *instance() {
                wxASSERT(_instance);
                return _instance;
            }
            /*!
                \brief configFileName
                \return
            */
            static std::string configFileName() {
                return instance()->_configFileName;
            }
            /*!
                \brief setConfigFileName
                \param s
            */
            static void setConfigFileName(const std::string &s) {
                instance()->_configFileName = s;
            }

            /*!
                \brief baseDir
                \return
            */
            static const std::string &baseDir() {
                return instance()->_baseDir;
            }
            /*!
                \brief configuration
                \return
            */
            static Database &configuration() {
                return instance()->_db;
            }

            static OpcCommon &opc() {
                return *(instance()->_opc);
            }

            // Declare signals
            // emitted to forward information messages to message loggers
            SIGNAL1(messageNotify, const LogMessage &)
            SIGNAL1(auditNotify, const LogMessage &)
            SIGNAL1(diagnosticNotify, const LogMessage &)

            /*!
                \brief logMessage
                \param m
            */
            void logMessage(const LogMessage &m) {
                messageNotify().emit(m);
            }
            /*!
             * \brief auditMessage
             * \param m
             */
            void auditMessage(const LogMessage &m) {
                auditNotify().emit(m);
            }
            /*!
             * \brief diagnosticMessage
             * \param m
             */
            void diagnosticMessage(const LogMessage &m) {
                if(_enableDiagnosticLog) // trace logging
                {
                    diagnosticNotify().emit(m);
                }
            }



            /*!
                \brief logMessage
                \param s
                \param m
                \param st
            */
            void logMessage(const std::string &s, const std::string &m, const std::string &st = "Ok") {
                LogMessage msg(s, m, st);
                logMessage(msg);
            }

            /*!
             * \brief auditMessage
             * \param s
             * \param m
             * \param st
             */
            void auditMessage(const std::string &s, const std::string &m, const std::string &st = "Ok") {
                LogMessage msg(s, m, st);
                auditMessage(msg);
            }
            /*!
             * \brief diagnosticMessage
             * \param s
             * \param m
             * \param st
             */
            void diagnosticMessage(const std::string &s, const std::string &m, const std::string &st = "Ok") {
                if(_enableDiagnosticLog) // trace logging
                {
                    LogMessage msg(s, m, st);
                    diagnosticMessage(msg);
                }
            }


            /*!
                \brief daq
                \return DAQ common data
            */
            static DaqCommon &daq() {
                return *(instance()->_daq);
            }
            /*!
             * \brief enableGui
             * \return
             */
            bool enableGui() const {
                return _enableGui;   // which sub systems are enabled - No GUI if running as deamon
            }
            /*!
             * \brief enableWeb
             * \return
             */
            bool enableWeb() {
                return _enableWeb;   // Web application
            }
            /*!
             * \brief setEnableGui
             * \param f
             */
            void setEnableGui(bool f)  {
                _enableGui = f;   // which sub systems are enabled - No GUI if running as deamon
            }
            /*!
             * \brief setEnableWeb
             * \param f
             */
            void  setEnableWeb(bool f) {
                _enableWeb = f;   // Web application
            }

            /*!
             * \brief hasObject
             * \param path
             * \return true if the object exists in the DAQ
             */
            static bool hasObject(const PropertyPath &path)
            {
                return  configuration().find(path) != 0;
            }

            /*!
             * \brief objectPathToId
             * \param path
             */
            static unsigned objectPathToId(const PropertyPath &path)
            {
                return MRL::Common::configuration().find(path);
            }
            /*!
             * \brief aliasMap
             * \return
             */
            static StringMap & aliasMap()
            {
                return  _aliasMap;
            }

            /*!
             * \brief findAlias
             * \param s
             * \return
             */
            static std::string findAlias(const std::string &s)
            {
                if(_aliasMap.find(s) != _aliasMap.end())
                {
                    return _aliasMap[s];
                }
                return std::string();
            }

            /*!
             * \brief reverseAliasMap
             * \return
             */
            static StringMap & reverseAliasMap()
            {
                return  _reverseAliasMap;
            }

            static std::string findReverseAlias(const std::string &s)
            {
                if(_reverseAliasMap.find(s) != _reverseAliasMap.end())
                {
                    return _reverseAliasMap[s];
                }
                return std::string();
            }

            static bool checkUser(const std::string &u, const std::string &pw);


            static bool checkUserAdmin(const std::string &u);
    };


    // Easy access references
    inline VariantPropertyTree &SETTINGS() {
        return Common::settings();
    }
    inline AnyPropertyTree &GLOBALS() {
        return Common::globals();
    }
    inline Common &COMMON() {
        return *Common::instance();
    }

    inline VariantPropertyTree &RUNTIME() {
        return Common::runTime();
    }

}
#endif // COMMON_H
