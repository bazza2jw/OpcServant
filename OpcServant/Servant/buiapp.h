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
#ifndef BUIAPP_H
#define BUIAPP_H
#include <MrlLib/mrllib.h>
#include <wx/cmdline.h>
#include "wx/xrc/xmlres.h"
#include <wx/docview.h>
#include <wx/image.h>
#include <wx/thread.h>
#include <Wt/WServer.h>
#include "Opc/opcthread.h"
#include "Daq/daqthread.h"
#include "serverobject.h"

class Mainframe;
namespace MRL {

    // This class allows to determine the last time the user has worked with
    // this application:
    class LastActivityTimeDetector : public wxEventFilter {
        public:
            LastActivityTimeDetector() {
                wxEvtHandler::AddFilter(this);
                m_last = wxDateTime::Now();
            }
            virtual ~LastActivityTimeDetector() {
                wxEvtHandler::RemoveFilter(this);
            }
            virtual int FilterEvent(wxEvent &event) {
                // Update the last user activity
                const wxEventType t = event.GetEventType();
                if (t == wxEVT_KEY_DOWN || t == wxEVT_MOTION ||
                        t == wxEVT_LEFT_DOWN ||
                        t == wxEVT_RIGHT_DOWN ||
                        t == wxEVT_MIDDLE_DOWN) {
                    m_last = wxDateTime::Now();
                }
                // Continue processing the event normally as well.
                return Event_Skip;
            }
            // This function could be called periodically from some timer to
            // do something (e.g. hide sensitive data or log out from remote
            // server) if the user has been inactive for some time period.
            bool IsInactiveFor(const wxTimeSpan &diff) const {
                return wxDateTime::Now() - diff > m_last;
            }
        private:
            wxDateTime m_last;
    };

    /*!
     * \brief The BuiApp class
     */

    class BuiApp :
        #ifdef USE_GUI
        public wxApp
        #else
        public wxAppConsole
        #endif
    {
            std::unique_ptr<Wt::WServer> _webThread; //! Web server thread
            std::unique_ptr<OpcThread> _opcThread;  //! OPC server thread
            std::unique_ptr<DaqThread> _daqThread;  //! Data collection / control thread
            std::unique_ptr<wxLogNull> _nullLog;
            std::unique_ptr<LastActivityTimeDetector> _inactivity; // detect keystokes and mouse presses
            Mainframe *_mainFrame = nullptr;

            bool _enableGui = true; // which sub systems are enabled - No GUI if running as deamon
            bool _enableWeb = true; // Web application
            bool _enableOpc = true; // OPC interface
            bool _disableExit = false; // is exit disabled on the main window - if true the main window is hidden
            bool _enableErrorLog = false;
            bool _shutdownOnExit = false;
            //
            //
            std::unique_ptr<ServerObject> _serverObject ;
        public:

            /*!
                \brief OnInit
                \return
            */
            virtual bool OnInit();
            /*!
                \brief OnInitCmdLine
                \param parser
            */
            void OnInitCmdLine(wxCmdLineParser &parser);
            /*!
                \brief OnCmdLineParsed
                \param parser
                \return true on success
            */
            bool OnCmdLineParsed(wxCmdLineParser &parser);
            /*!
                \brief OnExit
                \return
            */
            int OnExit();

            // Accessors
            Wt::WServer *webThread() {
                return  _webThread.get();   //! Web server thread
            }
            OpcThread    *opcThread() {
                return  _opcThread.get();   //! OPC server thread
            }
            DaqThread    *daqThread() {
                return  _daqThread.get();   //! Data collection / control thread
            }
            Mainframe     *mainFrame() {
                return _mainFrame;   // the main frame window
            }

            LastActivityTimeDetector * inactvity()
            {
                return _inactivity.get();
            }

            void setShutdownOnExit(bool f = true) {_shutdownOnExit = f;}

    };
}

wxDECLARE_APP(MRL::BuiApp);
#endif // BUIAPP_H
