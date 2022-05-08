/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef SCALEOFFSET_H
#define SCALEOFFSET_H
#include <MrlLib/variantpropertytree.h>

namespace MRL
{
/*!
 * \brief The ScaleOffset struct
 */
struct ScaleOffset
{
    double _scale = 1.0;
    double _offset = 0.0;
    double _current = 0.0;
    ScaleOffset(double s = 1.0, double o = 0.0) : _scale(s), _offset(o) {}
    ScaleOffset(const ScaleOffset &s) = default;
    double scale(double v)
    {
        _current = (v*_scale + _offset);
        return _current;
    }
    void setScaleOffset(double s, double o = 0.0) { _scale = s; _offset = o;}
    double current() const { return _current;}
    double unscale(double v)
    {
        if(_scale != 0) return (v - _offset)/_scale; // inverse scaling
        return 0.0;
    }

    double getScale()  const { return _scale;}
    double getOffset() const { return  _offset;}
    void setScale(double v) { _scale = v;}
    void setOffset(double v) {_offset = v;}

    // syntax sugar
    double operator * (double v)
    {
        return v * _scale + _offset;
    }

    double operator / (double v)
    {
        return unscale(v);
    }

};

/*!
 * \brief The ScaleOffsetMap class
 */
class ScaleOffsetMap : public std::map<std::string, ScaleOffset>
{
public:
    ScaleOffsetMap() {}
    ScaleOffsetMap(const MRL::StringVector &l)
    {
        ScaleOffset o(1.0,0.0);
        for(unsigned i = 0; i < l.size(); i++)
        {
           insert_or_assign(l[i],o);
        }
    }
    /*!
     * \brief read
     * \param c
     * \param p
     */
    void read(MRL::VariantPropertyTree &c, MRL::PropertyPath &p) // read from configuration to scale offset set
    {
        // the map must be initialised with tag set before reading or writing
        for(auto i = begin(); i != end(); i++)
        {
            if(c.existsub(p,i->first))
            {
                p.push_back(i->first);
                ScaleOffset  o(c.getValue<double>(p,"Scale"),c.getValue<double>(p,"Offset"));
                ScaleOffset &v = i->second;
                v = o;
                p.pop_back();
            }
            else
            {
                ScaleOffset &o = i->second;
                o.setScale(1.0);
                o.setOffset(0.0);
            }
        }
    }
    /*!
     * \brief write
     * \param c
     * \param p
     */
    void write(MRL::VariantPropertyTree &c, MRL::PropertyPath &p) // write to configuration to scale offset set
    {
        c.eraseChildren(p);
        for(auto i = begin(); i != end(); i++)
        {
            p.push_back(i->first);
            ScaleOffset &o = i->second;
            c.setValue(p,"Scale",o.getScale());
            c.setValue(p,"Offset",o.getOffset());
            p.pop_back();
        }
    }
    /*!
     * \brief scale
     * \param s
     * \param v
     * \return
     */
    double scale(const std::string &s, double v)
    {
        double ret = 0.0;
        auto i = find(s);
        if(i != end())
        {
            ScaleOffset &o = i->second;
            ret = o.scale(v);
        }
        return ret;
    }
    /*!
     * \brief current
     * \param s
     * \param v
     * \return
     */
    double current(const std::string &s, double v)
    {
        double ret = 0.0;
        auto i = find(s);
        if(i != end())
        {
            ScaleOffset &o = i->second;
            ret = o.current();
        }
        return ret;
    }
};

}
#endif // SCALEOFFSET_H
