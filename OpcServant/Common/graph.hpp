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
#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <string>
#include <wx/dc.h>
#include <wx/datetime.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/colour.h>
#include <wx/gdicmn.h>
#include <time.h>
#include <wx/dcmemory.h>
#include <memory>

namespace MRL
{
/*!
 * \brief The Graph class
 */
class  Graph
{
    time_t LastDateTime = time(0);
    double    last_v = 0.0;
    struct dataPt {
        double t;
        double v;
        dataPt(double ts, double vs) : t(ts), v(vs) {}
        dataPt(const dataPt &) = default;
    };

    typedef std::list<dataPt> dtValueList;
    /*!
     * \brief The DataMap class
     */
    class DataMap :public dtValueList
    {
    public:
        wxColour colour;
        DataMap() {}
        DataMap(wxColour &c) : colour(c) {}
        DataMap(const DataMap &p) = default;
    };
    /*!
     * \brief The LimitLine struct
     * Where the alarm or threshold limits are
     */
    struct LimitLine
    {
    public:
        //
        double value; // limit value
        wxColour colour; // colour to use
        //
        LimitLine(double v = 0, wxColour c = *wxRED) : value(v), colour(c) {}
        LimitLine(const LimitLine &l) = default;
    };
    //
    typedef std::vector<LimitLine> LimitList;
    //
    LimitList limits; // list of alarm limit lines
    //
    typedef std::unique_ptr<DataMap> DataMapPtr;
    typedef std::map<std::string, DataMapPtr > DATAMAP;
    //
    DATAMAP plotMap; // data
    bool logMode = false;  // are we in log mode ?
    //
    time_t minTime = time(0);
    time_t maxTime = time(0);
    time_t halfTime = time(0); // scale bits
    //
    double minY = 0.0;
    double maxY = 1.0;
    //
    double scaleMin = 0.0;
    double scaleMax = 1.0;
    double scaleSave = 0.0;
    double scaleStep = 2.0;
    double timeRange = 0.0;
    double maxTimeRange = 3600 * 24;
    //
    wxRect plotRect; // drawing area for the graphs
    wxSize legendSize;
    wxSize dateSize;
    wxSize timeSize;

    //
    void seekMaxMinYLine(DataMap &l);
    void seekMaxMinTimeLine(DataMap &l);
    void plotTimeLegend(wxDC &,time_t d,int x,int y, const wxSize &tsize, const wxSize &dsize);
    //
public:
    //
    Graph();
    virtual ~Graph() {}
    //
    size_t maxPointsPerLine = 10000;
    constexpr static int MinTimeRange = 4;
    //
    void plot(wxDC &p,const wxRect &r, bool greyBackground = true);
    void scaleGraph();
    void update(const std::string &tag, const time_t dt, double v);
    void setData(const std::string &tag, std::vector<double> &t, std::vector<double> &v)
    {
        if((t.size() > 0 ) && (v.size() > 0) && (t.size() == v.size()))
        {
            if(!hasLine(tag)) {
                addLine(tag);
            }
            // load in to the map and sort
            std::unique_ptr<DataMap> &m = plotMap[tag];
            for(int i = 0; i < t.size(); i++) {
                dataPt pt(t[i],v[i]);
                m->push_back(pt);
            }
        }
    }
    DataMapPtr & addLine(const std::string &tag, wxColour c = wxNullColour);
    void getMinMaxY(double &miny, double &maxy) const {
        miny = minY;
        maxy = maxY;
    }
    //
    bool hasLine(const std::string &name) {
        return (plotMap.find(name) != plotMap.end());
    }
    void setMaxPoints(int n) {
        maxPointsPerLine = n;
    }
    bool getLogMode() const {
        return logMode;
    }
    void setLogMode(bool f) {
        logMode = f;
    }
    void clear() {
        plotMap.clear();
        limits.clear();
    }
    void addTimeLegend(wxDC &);
    void addYlegend(wxDC &);
    void addLogYlegend(wxDC &DC);
    void drawLimitLines(wxDC &DC);

    void plotLines(wxDC &);
    void addLimitLine(double v, wxColour c)
    {
        LimitLine l(v,c);
        limits.push_back(l); // add it
    };
    wxString getValue(const wxPoint &); // return the string value of the given point as wxString for UI use
    time_t timeFromPoint(const wxPoint &); // return the timestamp from the given point
    //
    void plotToBitmap(wxBitmap &bm, int width, int height)
    {
        bm.Create(width,height);// resize the bitmap
        wxMemoryDC dc;
        dc.SelectObject(bm);
        wxRect r(0,0,width,height);
        scaleGraph();
        plot(dc,r);
    }
    //
    void setMaxTimeRange(int r)
    {
        maxTimeRange = r;
    }
};
}
#endif

