/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "buiapp.h"
#include "../NodeFlow/NodeEditorMainframe.h"

BuiApp::BuiApp() : wxApp ()
{
    
}
/*!
 * \brief OnInit
 * \return
 */
bool BuiApp::OnInit()
{
    wxApp::OnInit(); // parse the command line
    wxInitAllImageHandlers();
    wxXmlResource::Get()->InitAllHandlers();
    auto w = new NodeEditorMainframe(nullptr);
    w->Show();
    return true;
}

/*!
 * \brief OnInitCmdLine
 * \param parser
 */
void BuiApp::OnInitCmdLine (wxCmdLineParser &parser)
{
    wxApp::OnInitCmdLine(parser);
}

/*!
 * \brief OnCmdLineParsed
 * \param parser
 * \return
 */
bool BuiApp::OnCmdLineParsed (wxCmdLineParser &parser)
{
    wxApp::OnCmdLineParsed(parser);
    return true;
}

/*!
 * \brief OnExit
 * \return
 */
int  BuiApp::OnExit()
{
    return wxApp::OnExit();
}

