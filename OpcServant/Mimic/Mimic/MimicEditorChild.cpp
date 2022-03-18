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

#include "MimicEditorChild.h"
#include "MimicEditorFrame.h"
/*!
 * \brief MimicEditorChild::MimicEditorChild
 * \param parent
 */
MimicEditorChild::MimicEditorChild(wxMDIParentFrame *parent)
    : 	wxMDIChildFrame(parent,wxID_ANY,"Editor")
{
    canvasSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(canvasSizer);
    _panel = new MimicEditorPanel(this);
    canvasSizer->Add(_panel, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));

    wxMenu *menuFile = new wxMenu;

    menuFile->Append(wxID_NEW, "&New window\tCtrl-N", "Create a new flow editor window");
    menuFile->Append(wxID_OPEN, "&Open File...\tCtrl-O", "Open flow file");
    menuFile->Append(wxID_SAVE, "&Save File\tCtrl-S", "Save flow file");
    menuFile->Append(wxID_SAVEAS, "Save &As\tCtrl-A", "Save flow file as");
    menuFile->Append(wxID_CLOSE,"&Close Window","Close this window");
    menuFile->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit the program");



    wxMenuBar *mbar = new wxMenuBar;
    mbar->Append(menuFile, "&File");
    SetMenuBar(mbar);
    this->Connect(wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorChild::onClose), NULL, this);
    this->Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorFrame::onNew), NULL, parent);
    this->Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorFrame::onExit), NULL, parent);
    //
    this->Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorPanel::onOpen), NULL, _panel);
    this->Connect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorPanel::onSave), NULL, _panel);
    this->Connect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorPanel::onSaveAs), NULL, _panel);

}
/*!
 * \brief MimicEditorChild::~MimicEditorChild
 */
MimicEditorChild::~MimicEditorChild()
{
    this->Disconnect(wxID_CLOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorChild::onClose), NULL, this);
    this->Disconnect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorFrame::onNew), NULL, GetParent());
    this->Disconnect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorFrame::onExit), NULL, GetParent());
    //
    this->Disconnect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorPanel::onOpen), NULL, _panel);
    this->Disconnect(wxID_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorPanel::onSave), NULL, _panel);
    this->Disconnect(wxID_SAVEAS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MimicEditorPanel::onSaveAs), NULL, _panel);
}

/*!
 * \brief MimicEditorChild::onClose
 * \param event
 */
void MimicEditorChild::onClose(wxCommandEvent& event)
{
    Close();
}
