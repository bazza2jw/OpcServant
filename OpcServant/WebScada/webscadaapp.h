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
#ifndef WEBSCADAAPP_H
#define WEBSCADAAPP_H
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
#include <MrlLib/mrllib.h>
#include <wx/cmdline.h>
#include "wx/xrc/xmlres.h"
#include <wx/docview.h>
#include <wx/image.h>
#include <wx/thread.h>
#include <Wt/WServer.h>
namespace MRL
{
//
// wxWidgets is used to give an application framework
// esp services not provided by Wt
//
    class WebScadaApp :
            public wxAppConsole
    {
        std::unique_ptr<Wt::WServer> _webThread; //! Web server thread
    public:

        /*!
         * \brief OnInit
         * \return
         */
        virtual bool OnInit();
        /*!
         * \brief OnInitCmdLine
         * \param parser
         */
        void OnInitCmdLine (wxCmdLineParser &parser);
        /*!
         * \brief OnCmdLineParsed
         * \param parser
         * \return true on success
         */
        bool OnCmdLineParsed (wxCmdLineParser &parser);
        /*!
         * \brief OnExit
         * \return
         */
        int OnExit();

        // Accessors
        Wt::WServer * webThread() { return  _webThread.get();} //! Web server thread

    };
}

wxDECLARE_APP(MRL::WebScadaApp);

#endif // WEBSCADAAPP_H
