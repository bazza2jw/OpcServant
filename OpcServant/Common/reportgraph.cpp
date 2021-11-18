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
#include "reportgraph.h"

/*!
 * \brief ReportGraph
 * \param parent
 * \param id
 */
MRL::ReportGraph::ReportGraph(wxWindow* parent,wxWindowID id) : mpWindow(parent,id,wxDefaultPosition,wxDefaultSize,wxHSCROLL | wxVSCROLL | wxBORDER_STATIC)
{
    mpScaleX * _xScale = new mpScaleX(wxT("Time"), mpALIGN_BORDER_BOTTOM, true, mpX_DATETIME);
    _xScale->SetTicks(true);
    mpScaleY *_yScale = new mpScaleY(wxT("PPB"), mpALIGN_BORDER_LEFT, true);
    _yScale->SetTicks(true);
    SetMargins(5, 5, 20, 20);
    //
    AddLayer(_xScale);
    AddLayer(_yScale);
    //

}
/*!
 * \brief addlayer
 * \param title
 * \param clr
 * \param t
 * \param v
 */
void MRL::ReportGraph::addlayer(const wxString &title, const wxColour &clr, std::vector<double> &t, std::vector<double> &v)
{
    mpFXYVector *pl = new mpFXYVector(title);
    wxPen pn(clr, 1, wxPENSTYLE_SOLID);
    //
    pl->SetPen(pn);
    pl->SetDrawOutsideMargins(false);
    pl->SetContinuity(true);
    pl->ShowName(true);
    pl->SetData(t,v);
    AddLayer(pl);
}
