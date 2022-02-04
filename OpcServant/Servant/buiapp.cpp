/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "buiapp.h"
#include <Wt/WException.h>
#include <Common/common.h>
#include <Common/plugin.h>
#include <Common/objectmanager.h>
#include "Web/buiwebapplication.h"
#include "Common/Gui/SystemPropertiesDialog.h"
#ifdef USE_GUI
#include "Gui/Mainframe.h"
#endif
#include <MrlLib/states.h>
#include "serverobject.h"
#include <Common/Daq/daq.h>
#include <Common/Daq/daqcommon.h>
//
#include <Wt/Auth/AuthModel.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/RegistrationModel.h>


/*!
    \brief OnInit
    \return true on success
*/
bool MRL::BuiApp::OnInit() {
    try {
        // Building without GUI means smaller executable and smaller wx libraries
        #ifdef USE_GUI
        wxApp::OnInit(); // parse the command line
        wxInitAllImageHandlers();
        wxXmlResource::Get()->InitAllHandlers();
        _inactivity.reset(new LastActivityTimeDetector()); // add the message filter
        #else
        wxAppConsole::OnInit(); // parse the command line
        #endif
        // comment out to enable error message from wx - esp load library errors
#ifndef TRACE_ON
       if(!_enableErrorLog) _nullLog.reset(new wxLogNull()); // switches off warning dialogs
#endif

        STATES::States::initialiseStates();
        //
        if (Common::initialise())
            // At this point we have the boot strap config in
        {
            PropertyPath p;
            p.push_back("System");
            // What sub systems do we enable
            _enableGui |= stringToBool(SETTINGS().getValue<std::string>(p, "EnableGui"));
            _enableWeb |= stringToBool(SETTINGS().getValue<std::string>(p, "EnableWeb"));
            _enableOpc |= stringToBool(SETTINGS().getValue<std::string>(p, "EnableOpc"));
            //
            //
            // Load plugin modules
            std::string pluginDir(Common::baseDir());
            pluginDir += "/";
            pluginDir += PLUGIN_DIR;
            MRL::Plugin::loadPlugins(pluginDir);
            MRL::Plugin::initialiseAll(); // get the plugins started and registering interface objects
            //
            //
            (void) new TopLevelObjectManager("Site");
            (void) new TopLevelObjectManager("Equipment");
            (void) new TopLevelObjectManager("Publishers");
            (void) new FolderObjectManager();
            //
            // DAQ object available for connection
            _daqThread = std::make_unique<MRL::DaqThread>();
            //
            // Start threads in the dependancy order
            // OPC needs to be ready before the DAQ thread
            //
            if (_enableOpc) {
                // start the opc interface
                TRC("Starting OPC Thread");
                _opcThread = std::make_unique<MRL::OpcThread>();
                _opcThread->start();
                wxThread::Sleep(100);
            }
            //
            //
            #ifdef USE_GUI
            if (_enableGui) {
                TRC("Starting GUI")
                // GUI is not needed (wanted) if running as a service
                // Create GUI framework
                _mainFrame = new Mainframe(nullptr);
                // Show it
                if (_disableExit) {
                    _mainFrame->setExitEnabled(false);
                };
                //
                _mainFrame->Show();
                // Link navigation pane to model
                MRL::Common::display().setStore(_mainFrame->GetNavigation());
                //
                // start the GUI Interface
                MRL::Plugin::initialiseAllGui();
                //
                ProcessPendingEvents();
            }
            #endif
            //


            if (_enableWeb && MRL::Common::display().webStore()) {
                TRC("Starting Web")
                // start the web interface
                // Now start the web thread
                static const char *av[] = {"OpcServant", "--docroot=.;/resources", "--http-address=0.0.0.0", "--http-port=8082","--resources-dir=./resources"};
                //
                std::string av0 = wxAppConsole::argv[0].ToStdString();
                _webThread = std::make_unique<Wt::WServer>(av0);
                _webThread->setServerConfiguration(5, (char **)(av), WTHTTP_CONFIGURATION);
                _webThread->addEntryPoint(Wt::EntryPointType::Application,
                [](const Wt::WEnvironment & env) {
                    auto app = std::make_unique<MRL::BuiWebApplication>(UI_USER,env);
                    app->setTwoPhaseRenderingThreshold(0);
                    app->setTitle("Opc Servant");
                    app->useStyleSheet("styles.css");
                    app->messageResourceBundle().use(Wt::WApplication::appRoot() + "about");
                    app->refresh();
                    return std::move(app);
                },"/ui");
                //
                _webThread->addEntryPoint(Wt::EntryPointType::Application,
                [](const Wt::WEnvironment & env) {
                    auto app = std::make_unique<MRL::BuiWebApplication>(UI_ADMIN,env);
                    app->setTwoPhaseRenderingThreshold(0);
                    app->setTitle("Opc Servant Admin");
                    app->useStyleSheet("styles.css");
                    app->messageResourceBundle().use(Wt::WApplication::appRoot() + "about");
                    app->refresh();
                    return std::move(app);
                },"/admin");
                //
                _webThread->addEntryPoint(Wt::EntryPointType::Application,
                [](const Wt::WEnvironment & env) {
                    auto app = std::make_unique<MRL::BuiWebApplication>(UI_REPORT,env);
                    app->setTwoPhaseRenderingThreshold(0);
                    app->setTitle("Opc Servant Reports");
                    app->useStyleSheet("styles.css");
                    app->messageResourceBundle().use(Wt::WApplication::appRoot() + "about");
                    app->refresh();
                    return std::move(app);
                },"/report");
                //
                // Set up the model for web interfaces
                MRL::Plugin::initialiseAllWeb();
                //

                //
                // initialise the model for the Wt model and link to the configuration and runtime
                _webThread->start(); // start the server thread
            }
            //
            wxThread::Sleep(100); // give everything a chance to get going
            //
            //
            // Start the DAQ thread
            TRC("Starting DAQ")
            _daqThread->start();
            wxThread::Sleep(100);
            //
            MRL::Common::display().syncWithDatabase(MRL::Common::configuration()); // initial set up of models from configuration
            //
            _serverObject = std::make_unique<ServerObject>();
        }
        else {
            #ifdef USE_GUI
            auto w = new SystemPropertiesDialog(nullptr);
            w->ShowModal();
            #else
            wxLogDebug("Failed to initialise Common");
            #endif
        }
    }
    catch (Wt::WException &e) {
        wxLogDebug("Web Exception %s", e.what());
    }
    catch (std::exception &e) {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
    //
    return true;
}

/*!
    \brief OnInitCmdLine
    \param parser
*/
void MRL::BuiApp::OnInitCmdLine(wxCmdLineParser &parser) {
    #ifdef USE_GUI
    wxApp::OnInitCmdLine(parser);
    #else
    wxAppConsole::OnInitCmdLine(parser);
    #endif
    //
    parser.AddSwitch("web");
    parser.AddSwitch("gui");
    parser.AddSwitch("opc");
    parser.AddSwitch("disableExit"); // if running views a top level window we do not want the app to exit
    parser.AddSwitch("enableErrorLog"); // enable the system warning messages
}

/*!
    \brief OnCmdLineParsed
    \param parser
    \return true on success
*/
bool MRL::BuiApp::OnCmdLineParsed(wxCmdLineParser &parser) {
    // Override flags
    _enableWeb |= parser.Found("web");
    _enableGui |= parser.Found("gui");
    _enableOpc |= parser.Found("opc");
    _disableExit = parser.Found("disableExit");
    _enableErrorLog = parser.Found("enableErrorLog");
    return true;
}

/*!
    \brief OnExit
    \return exit code
*/
int  MRL::BuiApp::OnExit() {
    wxLogDebug("OnExit()");
    try {
        // Shutdown Web
        if (_webThread && _webThread->isRunning()) {
            _webThread->stop();
            while (_webThread->isRunning()) {
                wxThread::Sleep(100);
            }
        }


        if (_opcThread && _opcThread->GetThread()->IsAlive()) {
            _opcThread->stop();
            while (_opcThread->GetThread()->IsRunning()) {
                wxThread::Sleep(100);
            }
        }



        if (_daqThread && _daqThread->GetThread()->IsAlive()) {
            _daqThread->stop(); // deletes active objects
            while (_daqThread->GetThread()->IsRunning()) {
                wxThread::Sleep(200);
            }
        }
        // give a chance for all threads to finish

        //
        // dead threads should not be deleted on exit
        //
        _webThread.reset(nullptr);
        _opcThread.reset(nullptr);
        _daqThread.reset(nullptr);
        //
        //
        // close and delete any open top level windows
        for(auto iw = MRL::Common::daq().topWindows().begin(); iw != MRL::Common::daq().topWindows().end(); iw++)
        {
            wxWindow *w = iw->second;
            if(w)
            {
                w->Destroy();
            }
        }
        //
        MRL::Common::daq().topWindows().clear();
        MRL::Common::daq().tabWindows().clear();
        //
        // shut everything down
        ObjectManager::clear(); // remove factories
        ObjectManager::imageList().RemoveAll();
        Plugin::terminateAll(); // unload plugins
        Plugin::clear(); // release plugins
        //Common::clear(); // delete all singletons
    }
    catch (std::exception &e) {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }

    //
    //
    if(_shutdownOnExit) wxShutdown();
    //
    #ifdef USE_GUI
    return wxApp::OnExit();
    #else
    return wxAppConsole::OnExit();
    #endif
}

