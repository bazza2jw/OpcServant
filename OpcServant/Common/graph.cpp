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

#include "graph.hpp"
#include <math.h>
#include <wx/font.h>
#include <wx/fontdata.h>
#include <wx/gdicmn.h>
#include <wx/pen.h>
#include <iostream>
/*
*Function: Graph
*/
MRL::Graph::Graph()
{
}
/*
*Function:SeekMaxMinYLine
*/
void MRL::Graph::seekMaxMinYLine(DataMap &l)
{
    for(auto j = l.begin(); j != l.end(); j++)
    {
        if(minY > (*j).v)
        {
            minY = (*j).v;
        }
        else if(maxY < (*j).v)
        {
            maxY = (*j).v;
        };
    };
}
/*
*Function: TimeFromPoint
*Purpose: to return the time stamp from a point
*
*/
time_t MRL::Graph::timeFromPoint(const wxPoint &pt)
{
    time_t d; // invalid datetime
    if(timeRange > 0)
    {
        double dx = pt.x - plotRect.GetLeft();
        if(dx > 0)
        {
            int dt = (int)(dx * (double)timeRange /(double)plotRect.GetWidth());
            d = minTime + dt; // the selected date time
        };
    };
    return d;
}
/*
*Function: GetValue
*Purpose: to return the string value of the given point
*/
wxString MRL::Graph::getValue(const wxPoint &pt) // return the string value of the given point
{
    wxString s = "";
    if(timeRange > 0)
    {
        if(plotRect.Contains(pt))
        {
            double dx = pt.x - plotRect.GetLeft();
            if(dx > 0)
            {
                double dy = plotRect.GetBottom() - pt.y;
                if(dy > 0)
                {
                    int dt = (int)(dx * (double)timeRange /(double)plotRect.GetWidth());
                    time_t d = minTime + dt;
                    if(logMode)
                    {
                        double v = dy /(double)plotRect.GetHeight() * (scaleMax - scaleMin) + scaleMin;
                        if(v > 1.0)
                        {
                            v = pow(10,v - 1.0); // convert
                        };
                        s.Format("%8.6g",v);
                    }
                    else
                    {
                        double v = dy /(double)plotRect.GetHeight() * (scaleMax - scaleMin) + scaleMin;
                        s.Format("%8.6g",v);
                    };
                    wxDateTime dd(d);
                    s = s + " " + dd.FormatISOCombined(' ');
                };
            };
        };
    };
    return s;
}
/*
*Function:SeekMaxMinTimeLine
*/
void MRL::Graph::seekMaxMinTimeLine(DataMap &l)  // seek max and min in a line
{
    if(l.size() > 0)
    {
        if(minTime > l.front().t)
        {
            minTime = l.front().t;
        };
        if(maxTime < l.back().t)
        {
            maxTime = l.back().t;
        };
    };
};
/*
*Function:ScaleGraph()
*/
void MRL::Graph::scaleGraph()
{
    minY = 0;
    maxY = 1;
    scaleMin = 0;
    scaleMax = 1;
    timeRange = 1;
    minTime = 0;
    maxTime = 2;
    halfTime = 1;
    if(plotMap.size() > 0)
    {

        int npts = 0;
        DataMapPtr &ms = (*plotMap.begin()).second;
        //
        minTime = ms->front().t;
        maxTime = ms->front().t;
        minY = maxY = ms->front().v;

        //
        for(auto i = plotMap.begin(); i != plotMap.end(); i++,npts++)
        {
            DataMapPtr &m = i->second;
            if(m->size() > 0)
            {
                // is the first time
                seekMaxMinTimeLine(*m);
                seekMaxMinYLine(*m);
            };
        };

        //
        // now look in the limits line list
        //
        if(limits.size() > 0)
        {
            for(unsigned j = 0; j < limits.size(); j++)
            {
                if(limits[j].value < minY)
                {
                    minY = limits[j].value;
                }
                else if (limits[j].value > maxY)
                {
                    maxY = limits[j].value;
                };
            };
        };
        //
        // we have the time range
        timeRange = maxTime - minTime;
        halfTime = minTime + (int)(timeRange/2);
        scaleMax = scaleSave = scaleMin = 0;
        //
        //
        //
        if((minY >= 0) && (maxY > 0))
        {
            //
            scaleMax = maxY;
            //
            if(minY > 0)
            {
                if((maxY / minY) < 1.2)
                {
                    scaleMin = minY;
                };
            }
        }
        else if((minY < 0) && (maxY <= 0))
        {
            scaleMin = minY;
            if(maxY < 0)
            {
                if((minY / maxY) < 1.2)
                {
                    scaleMax = maxY;
                };
            }
            logMode = false;
        }
        else
        {
            scaleMin = -maxY;
            scaleMax =  maxY;
            if(minY != maxY)
            {
                logMode = false;
            };
        };
        //
        if(scaleMin == scaleMax)
        {
            scaleMax += 10.0; // they are the same - increment max
        };
        //
        if(logMode)
        {
            scaleMin = 0;
            if(scaleMax > 1)
            {
                scaleMax = ceil(log10(scaleMax)) + 1; // add one on to allow for the range 0 - 1
            };
        };
        scaleSave = scaleMax;
        //
    }
}
/*!
 * \brief MRL::Graph::addLine
 * \param tag
 * \param c
 * \return the data set created
 */
MRL::Graph::DataMapPtr &MRL::Graph::addLine(const std::string &tag, wxColour c)
{
    static const char * colour_list[] =
    {
        "MEDIUM FOREST GREEN",
        "MEDIUM GOLDENROD",
        "MEDIUM ORCHID",
        "MEDIUM SEA GREEN",
        "MEDIUM SLATE BLUE",
        "MEDIUM SPRING GREEN",
        "MEDIUM TURQUOISE",
        "MEDIUM VIOLET RED",
        "MIDNIGHT BLUE"
    };

    if(c == wxNullColour) c = wxColour(colour_list[plotMap.size() % 8]);
    DataMapPtr l(new DataMap(c));
    plotMap[tag] = std::move(l);
    return plotMap[tag];
}

/*
*Function: Update
*Inputs:line name , time stamp, value
*/
void  MRL::Graph::update(const std::string &tag,const time_t dt, double v)
{
    if(dt > (time_t)maxTime)
    {
        if(!hasLine(tag))
        {
            addLine(tag);
        };
        //
        DataMapPtr &m =  plotMap[tag];
        dataPt pt((double)dt,v);
        //
        m->push_back(pt);
        //
        while(m->size() > maxPointsPerLine)
        {
            m->pop_front();
        };
        LastDateTime = dt;
        last_v = v;
    }
}

/*
*Function:void PlotTimeLegend(QDateTime &d,int x,int y,int w,int h)
*plots a time legend on the x-axis
*Inputs:date/time, x mid point , y top, width , line height

*/
void MRL::Graph::plotTimeLegend(wxDC &DC, time_t d, int x, int y, const wxSize &tsize, const wxSize &dsize)
{
    wxDateTime dt(d);
    wxString s = dt.FormatISOTime();
    DC.DrawText(s,x - tsize.GetWidth() / 2, y + tsize.GetHeight()/2);
    s =dt.FormatISODate();
    DC.DrawText(s, x - dsize.GetWidth() / 2, y + (tsize.GetHeight() * 2));
}

/*!
 * \brief MRL::Graph::addTimeLegend
 * \param DC
 * \param plotRect
 */
void  MRL::Graph::addTimeLegend(wxDC &DC)
{
    DC.SetTextForeground(*wxBLACK);
    if(plotRect.GetWidth() / 4 > dateSize.GetWidth())
    {
        plotTimeLegend(DC, minTime,plotRect.GetLeft(),plotRect.GetBottom(),timeSize,dateSize);
        plotTimeLegend(DC, maxTime,plotRect.GetRight(),plotRect.GetBottom(),timeSize,dateSize);
    };
    plotTimeLegend(DC, halfTime,(plotRect.GetLeft() + plotRect.GetRight())/2,plotRect.GetBottom(),timeSize,dateSize);
    DC.SetPen(*wxGREY_PEN);
    for(int i = 1; i < 4; i++)
    {
        int x = plotRect.GetLeft() + (plotRect.GetWidth() * i) / 4;
        DC.DrawLine(x,plotRect.GetBottom(),x,plotRect.GetTop());
    };
}
/*!
 * \brief MRL::Graph::addYlegend
 * \param DC
 * \param plotRect
 */
void  MRL::Graph::addYlegend(wxDC &DC)
{
    DC.SetPen(*wxBLACK_PEN);
    int nsteps = (plotRect.GetHeight() / timeSize.GetHeight() / 3) + 1;
    if((nsteps > 1) && (timeRange > 0) && (scaleMax > scaleMin))
    {
        //
        if((scaleMin == 0) && (scaleMax > 2))
        {
            scaleMax = ceil(scaleMax);
            //
            if(nsteps > (int)scaleMax)
            {
                nsteps = (int)scaleMax;
            }
            else
            {
                double pt = floor(log10(scaleMax));
                //
                // Power of ten
                //
                pt = pow(10.0,pt);
                int ns = (int)(scaleMax / pt) + 1;
                if(ns <= nsteps)
                {
                    nsteps = ns;
                }
                else
                {
                    double r = scaleMax;
                    do
                    {
                        pt *= 2;
                        r = ((double)nsteps) * pt;
                    } while(r < scaleMax);
                };
                scaleMax = ((double)nsteps) * pt;
            };
        };
        //
        double sy = (scaleMax - scaleMin) / (double)nsteps;
        int dy = plotRect.GetHeight() / nsteps;
        wxString s;
        // handle the problem of small steps
        const char *Format = (sy < 1.0)?"%8.5g":"%6.1f";
        for(int kk = 0; kk <  nsteps; kk++)
        {
            int y =  plotRect.GetBottom() - kk * dy;
            s = wxString::Format(Format,scaleMin + sy * (double)kk);
            DC.DrawText(s,timeSize.GetWidth()/8, plotRect.GetBottom() - kk * dy);
            DC.DrawLine(plotRect.GetLeft(), y, plotRect.GetRight(), y);
        }
        //

        //
        s = wxString::Format(Format,scaleMax);
        DC.DrawText(s,timeSize.GetWidth()/8, plotRect.GetTop());
        //
    };
}



/*!
 * \brief MRL::Graph::addLogYlegend
 */
void MRL::Graph::addLogYlegend(wxDC &DC)
{
    DC.SetPen(*wxBLACK_PEN);
    int nsteps = (int)scaleMax;                        // this is required number of steps
    int maxsteps = plotRect.GetHeight() / timeSize.GetHeight() / 2; // this is the max number of steps we can have
    //
    if(nsteps > maxsteps)
    {
        // not enough room so put only the top and bottom values
        wxString s;
        DC.DrawText("0",0, plotRect.GetBottom());
        //
        s = wxString::Format("1e+%d", (int)scaleMax);
        DC.DrawText(s,timeSize.GetWidth()/8, plotRect.GetTop());
    }
    else if(nsteps > 0)
    {
        int dy = plotRect.GetHeight() / nsteps;
        //
        double sc = 1.0;
        //
        wxString s;
        for(int kk = 1; kk <  nsteps; kk++)
        {
            s = wxString::Format("%8.1f",sc);
            DC.DrawText(s,	timeSize.GetWidth() / 8, plotRect.GetBottom() - kk * dy);
            sc *=10;
        };
        //
        s = wxString::Format("%8.1f",sc);
        DC.DrawText(s,timeSize.GetWidth()/8,plotRect.GetTop());
        //
        wxPen spen(*wxBLACK_PEN);
        wxPen dpen(*wxBLACK_DASHED_PEN);
        //
        for(int k = 1 ; k < nsteps; k++)
        {
            int y = plotRect.GetBottom() - k * dy;
            DC.SetPen(spen);
            DC.DrawLine(plotRect.GetLeft(), y, plotRect.GetRight(), y);
            //
            // we are in log mode - maybe we should add some log graph lines too
            // is there enough space for some sub interval lines ?
            //
            if(dy > 40) // 40 = pixels for space to draw 8 log lines
            {
                static int ltab[] =
                {
                    //2,3,4,5,6,7,8,9
                    3010,4771,6021,6990,7782,8451,9031,9542
                };
                //
                DC.SetPen(dpen);
                for(int j = 0 ; j < 8; j++)
                {
                    int z = y - dy * ltab[j] / 10000; // get the sub division
                    DC.DrawLine(plotRect.GetLeft(), z, plotRect.GetRight(),z);
                };
            };
        };
    };

}

/*!
 * \brief MRL::Graph::plotLines
 * \param DC
 * \param plotRect
 */
void  MRL::Graph::plotLines(wxDC &DC)
{
    if((timeRange > 0) && (scaleMax > scaleMin) && (plotMap.size() > 0))
    {

        int nc = 0;
        for(auto i = plotMap.begin(); i != plotMap.end(); i++)
        {
            std::unique_ptr<DataMap> &m = (*i).second;
            if(m->size() > 1)
            {
                double dx = (double)plotRect.GetWidth() / (double)(maxTime - minTime);
                double dy = (double)plotRect.GetHeight() /(scaleMax - scaleMin);
                if(dy)
                {
                    //

                    DC.SetPen(m->colour);
                    wxPoint pts[m->size()];
                    wxPoint *p = pts;
                    //
                    for(auto j = m->begin(); j != m->end(); j++,p++)
                    {
                        double x = (double) ((*j).t - minTime) * dx;
                        double y = 0.0;
                        //
                        if(logMode)
                        {
                            if((*j).v >= 1.0)
                            {
                                y = (log10((*j).v) + 1.0) * dy;
                            }
                            else
                            {
                                // if less than 1.0 we use a linear scale
                                y = (*j).v * dy;
                            };
                        }
                        else
                        {
                            y = ((*j).v - scaleMin) * dy;
                        };
                        p->x = x + plotRect.GetLeft();
                        p->y = plotRect.GetBottom() - y;
                    };
                    DC.DrawLines(m->size(),pts); // draw the lines
                };
            };
        };

        int maxLen = 0; // find longest string
        for(auto i = plotMap.begin(); i != plotMap.end(); i++)
        {
            if(int((*i).first.length()) > maxLen) maxLen = (*i).first.length();
        }
        //
        wxRect bkg(plotRect.GetLeft(),plotRect.GetTop(),(maxLen + 2) * timeSize.GetWidth()/8, plotMap.size() * (timeSize.GetHeight()+2) + 6  );
        DC.SetBrush(*wxWHITE_BRUSH);
        DC.SetPen(*wxBLACK_PEN);
        DC.DrawRectangle(bkg);
        //
        for(auto i = plotMap.begin(); i != plotMap.end(); i++, nc++)
        {
            std::unique_ptr<DataMap> &m = (*i).second;
            // Now draw the legends
            wxBrush br(m->colour);
            DC.SetBrush(br);
            wxRect lr(plotRect.GetLeft() + 3, plotRect.GetTop() + nc * (timeSize.GetHeight() + 2), timeSize.GetWidth()/8, timeSize.GetHeight());
            DC.DrawRectangle(lr);
            DC.DrawText((*i).first,plotRect.GetLeft() + timeSize.GetWidth()/4, plotRect.GetTop() + nc * (timeSize.GetHeight() + 2));
        }
    };

}

void MRL::Graph::drawLimitLines(wxDC &DC)
{
    if(limits.size() > 0)
    {
        for(unsigned j = 0; j < limits.size(); j++)
        {
            if(scaleMax > scaleMin)
            {
                double y = 0;
                //
                wxPen pn(limits[j].colour,2);
                DC.SetPen(pn);
                //
                double dy = (double)plotRect.GetHeight() /(scaleMax - scaleMin);
                //
                if(logMode)
                {
                    if(limits[j].value > 1)
                    {
                        y = (log10(limits[j].value) + 1.0)* dy;
                    }
                    else
                    {
                        y = limits[j].value * dy;
                    };
                }
                else
                {
                    y = (limits[j].value - scaleMin) * dy;
                };
                //
                DC.DrawLine(plotRect.GetLeft(),plotRect.GetBottom() - (int)y,
                            plotRect.GetRight(),plotRect.GetBottom() - (int)y);
                //
            };
        };
    };
}

/*
*Function:Plot
*draws the graph to the DC
*/
void MRL::Graph::plot(wxDC &DC, const wxRect &rect, bool greyBackground)
{
    DC.SetTextBackground(*wxWHITE);
    DC.SetTextForeground(*wxBLACK);
    //
    DC.Clear();
    //
    DC.SetFont(*wxSMALL_FONT);
    wxFontMetrics m = DC.GetFontMetrics();
    //
    scaleGraph();
    //
    // Workout the legend space needed on the right
    //
    legendSize = DC.GetTextExtent("00000000");
    dateSize = DC.GetTextExtent("2000-01-01");
    timeSize = DC.GetTextExtent("00:00:00");
    //
    // This is the plotting area
    plotRect = wxRect (wxPoint(legendSize.GetWidth(),m.averageWidth),wxPoint(rect.GetRight() - (dateSize.GetWidth() * 2) / 3, rect.GetBottom() -  timeSize.GetHeight()*3));
    //
    bool fTimeLegend = false;
    bool fYLegend = false;
    //
    fYLegend = (rect.GetWidth() > (legendSize.GetWidth() * 4));
    fTimeLegend = (rect.GetHeight() > timeSize.GetHeight() * 10) && (plotRect.GetWidth() > dateSize.GetWidth());
    //
    DC.SetPen(*wxBLACK_PEN);
    DC.SetBrush(*(greyBackground?wxLIGHT_GREY_BRUSH:wxWHITE_BRUSH));
    DC.DrawRectangle(plotRect);
    //
    if(fTimeLegend) addTimeLegend(DC);
    if(fYLegend)
    {
        scaleMax = scaleSave;
        if(logMode)
        {
            addLogYlegend(DC);
        }
        else
        {
            addYlegend(DC);
        }
    };
    drawLimitLines(DC);
    plotLines(DC);
}
