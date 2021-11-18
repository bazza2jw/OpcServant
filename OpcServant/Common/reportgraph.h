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
#ifndef REPORTGRAPH_H
#define REPORTGRAPH_H

#include <stdlib.h>
#include <MrlLib/mrllib.h>
#include <Common/mathplot.h>

namespace MRL {
/*!
 * \brief mpFXYVectorPtr
 */
typedef std::unique_ptr<mpFXYVector>  mpFXYVectorPtr;
/*!
 * \brief mpInfoLegendPtr
 */
typedef std::unique_ptr<mpInfoLegend> mpInfoLegendPtr;

/*!
 * \brief The ReportGraph class
 */
class ReportGraph : public mpWindow
{
    //
public:
    /*!
     * \brief ReportGraph
     * \param parent
     * \param id
     */
    ReportGraph(wxWindow* parent,wxWindowID id = wxID_ANY);
    /*!
     * \brief addlayer
     * \param title
     * \param clr
     * \param t
     * \param v
     */
    void addlayer(const wxString &title, const wxColour &clr, std::vector<double> &t, std::vector<double> &v);

};

}
#endif // REPORTGRAPH_H
