#ifndef MRLLIB_H
#define MRLLIB_H
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
#include "mrllib_global.h"
#include <vector>
#include <list>
#include <memory>
#ifdef USE_GUI
#include <wx/propgrid/property.h>
#endif
#include <MrlLib/trace.hpp>
#include <wx/sstream.h>
#include <MrlLib/json/json.h>
#include <wx/xml/xml.h>
#include <boost/any.hpp>
#include <MrlLib/trace.hpp>
#include <Wt/WComboBox.h>
#include <wx/dir.h>
#include <boost/bimap.hpp>
//
#define ISO_DATE_FMT "yyyy-MM-dd HH:mm:ss"
#define ISO_DATE_Q_FMT "yyyy-MM-ddTHH:mm:ss"
#define ISO_DATE_I_FMT "yyyy-MM-ddTHH:mm:ss"

namespace MRL {

/*!
     * \brief getTextWidth
     * \param f
     * \param n
     * \return
     */
    int getTextWidth(wxFont &f, int n);

    template <typename T>
    /*!
        \brief stringToNumber
        \param Text
        \return
    */
    inline T stringToNumber(const std::string &Text) { //Text not by const reference so that the function can be used with a
        if (!Text.empty()) {
            //character array as argument
            std::stringstream ss(Text);
            T result;
            return ss >> result ? result : T(0);
        }MRLLIBSHARED_EXPORT
        return T(0);
    }

    template <typename T>
    /*!
        \brief isType
        \param a
        \return true if any type is type
    */
    inline bool isType(boost::any &a) {
        return a.type().hash_code() == typeid(T).hash_code();
    }

    /*!
        \brief stringToBool
        \param s
        \return true on success
    */
    MRLLIBSHARED_EXPORT bool stringToBool(const std::string &s);
    /*!
     * \brief boolToString
     * \param f
     * \return True or False
     */
    inline const char * boolToString(bool f) { return f?"True":"False";}

    /*!
        \brief stringToXml
        \param s
        \param doc
        \return true on success
    */
    inline bool stringToXml(const std::string &s, wxXmlDocument &doc) {
        wxString in(s);
        wxStringInputStream is(in);
        return doc.Load(is);
    }

    /*!
        \brief xmlToString
        \param doc
        \param s
        \return true on success
    */
    inline bool xmlToString(wxXmlDocument &doc, std::string &s) {
        wxString out;
        wxStringOutputStream os(&out);
        bool ret = doc.Save(os);
        s = out.ToStdString();
        return ret;
    }

    /*!
        \brief stringToJson
        \param s
        \param v
        \return true on success
    */
    inline bool stringToJson(const std::string &s, Json::Value &v) {
        if (!s.empty()) {
            try {
                std::istringstream is;
                is.str(s);
                v.clear();
                is >> v;
                return true;
            }
            catch (...) {

            }
        }
        return false;
    }

    /*!
        \brief jsonToString
        \param v
        \param s
        \return true on success
    */
    inline bool jsonToString(Json::Value &v, std::string &s) {
        try {
            std::ostringstream os;
            os << v << std::endl;
            s = os.str();
            return true;
        }
        catch (...) {

        }
        return false;
    }

    /*!
     * \brief toLong
     * \param l
     * \param h
     * \return unsinged long
     */
    inline uint32_t  toLong(uint16_t l, uint16_t h)
    {
        return l | (h << 16);
    }




#define XML_MIN_SIZE (20)

   // helpers for property grids
   MRLLIBSHARED_EXPORT int stringTimeToInt(const std::string &s);
#ifdef USE_GUI
    void SetChoice(wxChoice *w, const std::string &s);
    std::string GetChoice(wxChoice *w);
    void propertiesToJson(wxPropertyGridManager *manager, Json::Value &v);
    void jsonToProperties(wxPropertyGridManager *manager, Json::Value &v);
    void clearProperties(wxPropertyGridManager *manager);
    wxPGProperty * findProperty(wxPropertyGridManager *manager, const std::string &name);
#endif
    /*!
    * \brief SetChoice
    * \param c
    * \param s
    */
   void SetChoice(Wt::WComboBox *c, const std::string &s);
   /*!
    * \brief GetChoice
    * \param c
    * \return current selection
    */
   std::string GetChoice(Wt::WComboBox *c);

   typedef std::vector<std::string> StringVector;

   /*!
     * \brief toFloat
     * \param s
     * \return float value
     */
    inline float toFloat(const wxString &s)
    {
        double v;
        s.ToDouble(&v);
        return (float)v;
    }
    /*!
     * \brief toShort
     * \param s
     * \return short value
     */
    inline uint16_t toShort(const wxString &s)
    {
        long v;
        s.ToLong(&v);
        return (uint16_t)(v);
    }
    /*!
     * \brief stringToBin
     * \param s
     * \param b
     * \param l
     */
    void stringToBin(const int8_t *s, uint8_t *b, int l);
    /*!
     * \brief bytesToString
     * \param b
     * \param s
     * \param l
     */
    void bytesToString(uint8_t *b, int8_t *s, int l);



    /*!
     * \brief getSerialPortList
     * abstract for OS here
     * \param sp
     * \return number of ports found
     */
    int getSerialPortList(wxArrayString &sp);

    /*!
     * \brief getSerialPortList
     * \param sp
     * \return number of ports found
     */
    int getSerialPortList(std::list<std::string> &sp);


    typedef std::map<std::string, std::string> StringMap;

    // separator used to separate path and input
#define TAG_SEPERATOR ":"



}

#endif // MRLLIB_H
