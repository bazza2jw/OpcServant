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
#ifndef PLUGIN_H
#define PLUGIN_H
#include "common.h"
#include <wx/log.h>
#include <wx/dir.h>
#include <wx/dynlib.h>
#include <wx/utils.h>
#include <wx/stdpaths.h>
/*!

    \author B. J. Hill
    \date __DATE__
    License:  GNU LESSER GENERAL PUBLIC LICENSE 2.1
    (c)  Micro Research Limited 2010 -
*/
class wxDynamicLibrary;

namespace  MRL {
    /*!
        \brief PLUGIN_PATH
    */
    constexpr const char *PLUGIN_PATH = "Plugins"; // path in settings to plugins
    /*!
        \brief PLUGIN_DIR
    */
    constexpr const char *PLUGIN_DIR =  "modules"; // where plugins are found wrt root directory
    /*!
        \brief The Plugin class
    */
    class Plugin {
            wxDynamicLibrary _library;  //!< library object
            std::string _name; //!< plugin (short) name
            typedef std::unique_ptr<Plugin> PluginRef; //!< reference counted shared pointer
            typedef std::map<std::string,PluginRef> MAP;
            static MAP _map; // map of plugins
        public:
            /*!
                \brief Plugin
                \param name
            */
            Plugin(const std::string &name);
            /*!
                \brief ~Plugin
            */
            virtual ~Plugin();
            /*!
                \brief loadPlugins
                \param dir
                \return true on success
            */
            static bool loadPlugins(const std::string &dir);
            /*!
                \brief load
                \param dir
                \param f
                \return true on success
            */
            bool load(wxString &dir, wxString &f);


            /*!
                \brief library
                \return
            */
            wxDynamicLibrary & library()  {
                return _library;
            }

            /*!
             * \brief find
             * \param n
             * \return
             */
            static Plugin * find(const std::string &n)
            {
                Plugin * ret = nullptr;
                auto i = _map.find(n);
                if(i != _map.end())
                {
                    ret = (i->second).get();
                }
                return ret;
            }

            /*!
             * \brief clear
             */
            static void clear()
            {
                for(auto i = _map.begin(); i != _map.end(); i++)
                {
                    PluginRef &r = (i->second);
                    r.release();
                }
                _map.clear();
            }

            //
            // Global initialise
            virtual void initialise(){}
            // Initialise DAQ
            virtual void initialiseDaq() {}
            // Initialise the GUI
            virtual void initialiseGui() {}
            // Initialise OPC
            virtual void initialiseOpc() {}
            // Initialise Web
            virtual void initialiseWeb() {}
            // Clean up
            virtual void terminate() {}
            //
            /*!
             * \brief callAllPlugins
             * \param func
             */
            static void callAllPlugins(std::function<void (Plugin *)> func);
            /*!
             * \brief initialiseAll
             */
            static void initialiseAll();
            /*!
             * \brief terminateAll
             * Full close down sequence
             */
            static void terminateAll();
            /*!
             * \brief initialiseAllGui
             */
            static void initialiseAllGui();
            /*!
             * \brief initialiseAllOpc
             * Called from server initialise
             */
            static void initialiseAllOpc();
            /*!
             * \brief initialiseAllWeb
             */
            static void initialiseAllWeb();
            /*!
             * \brief initialiseAllDaq
             */
            static void initialiseAllDaq();

    };
}
#endif // PLUGIN_H
