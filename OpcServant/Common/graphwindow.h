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
#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <MrlLib/mrllib.h>
#include <wx/window.h>
#include <Common/graph.hpp>
#include <wx/bitmap.h>
namespace MRL
{
/*!
 * \brief The GraphWindow class
 */
class GraphWindow : public wxWindow
{
    Graph _graph;
public:
    GraphWindow(wxWindow *parent, wxWindowID id = wxID_DEFAULT, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0, const wxString &name=wxPanelNameStr):
        wxWindow (parent, id, pos, size, style, name)
    {
        this->Connect(wxEVT_PAINT, wxPaintEventHandler(GraphWindow::OnPaint), NULL, this);
        _graph.clear();
        SetForegroundColour(*wxBLACK);
        SetBackgroundColour(*wxWHITE);
    }

    /*!
     * \brief graph
     * \return
     */
    Graph & graph() {
        return _graph;
    }
    /*!
     * \brief redraw
     */
    void redraw()
    {
        _graph.scaleGraph();
        Refresh(true);
    }

private:
    virtual void    OnPaint(wxPaintEvent& event);
};
}
#endif // GRAPHWINDOW_H
