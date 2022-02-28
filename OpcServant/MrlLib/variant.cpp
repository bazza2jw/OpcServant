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
#include "variant.h"
#include <wx/log.h>

//
std::map<std::string, MRL::VariantStreamerBase *> MRL::VariantStreamerBase::_map; // portable name
std::map<std::string, MRL::VariantStreamerBase *> MRL::VariantStreamerBase::_typemap; // name returned by typeid().name
//
// This is the object's type
#define JSON_OBJECT_TYPE "__TYPE__"
/*!
    \brief setXml
*/
void MRL::setXml(wxXmlNode *n, boost::any &a) {
    const char *id = a.type().name();
    MRL::VariantStreamerBase *p = MRL::VariantStreamerBase::findByTypename(id);
    if (p) {
        n->AddAttribute("Type", wxString(p->id())); // note the type
        wxString v(p->toString(a));
        (void)new wxXmlNode(n, wxXML_TEXT_NODE, "Value", v);
    }
    else {
        if (!a.empty()) {
            n->AddAttribute("Type", "string"); // note the type
            wxString v(boost::any_cast<std::string>(a));
            (void) new wxXmlNode(n, wxXML_TEXT_NODE, "Value", v);
        }
    }
}

/*!
    \brief getXml
*/
void MRL::getXml(wxXmlNode *n, boost::any &a) {
    std::string t = n->GetAttribute("Type").ToStdString();
    MRL::VariantStreamerBase *p = MRL::VariantStreamerBase::find(t);
    std::string s = n->GetNodeContent().ToStdString();
    if (p) {
        a = p->fromString(s); // cast to the correct type
    }
    else {
        a = boost::any(false); // default to bool instead of empty
    }
}


/*!
    \brief setXml
*/
void MRL::setXml(wxXmlNode *n, Variant &a) {
    wxString v(toString(a));
    MRL::VariantStreamerBase *p = MRL::VariantStreamerBase::findByTypename(a.type().name());
    if(p)
    {
        n->AddAttribute("Type", wxString(p->id()));
        (void)new wxXmlNode(n, wxXML_TEXT_NODE, "Value", v);
    }
}

/*!
    \brief getXml
*/
void MRL::getXml(wxXmlNode *n, Variant &a) {
    std::string t = n->GetAttribute("Type").ToStdString();
    std::string s = n->GetNodeContent().ToStdString();
    a = MRL::VariantStreamerBase::variantFromString(s,t);
}



/*!
    \brief operator >>
    \param is
    \param a
    \return
*/
wxDataInputStream &operator  >> (wxDataInputStream &is, boost::any &a) {
    std::string t;
    is >> t;
    MRL::VariantStreamerBase *p = MRL::VariantStreamerBase::find(t);
    if (p) {
        p->read(is, a);
    }
    else {
        wxLogError("Unknown Variant Type  %s", t.c_str());
    }
    return is;
}

/*!
    \brief operator <<
    \param os
    \param a
    \return
*/
wxDataOutputStream &operator << (wxDataOutputStream &os, const boost::any &a) {
    MRL::VariantStreamerBase *p = MRL::VariantStreamerBase::findByTypename(a.type().name());
    if (p) {
        p->write(os, a);
    }
    else {
        wxLogError("Unknown Variant Type  %s", a.type().name());
    }

    return os;
}


/*!
    \brief operator >>
    \param is
    \param s
    \return
*/
wxDataInputStream &operator  >> (wxDataInputStream &is, std::string &s) {
    wxString v;
    is >> v;
    s = v.ToStdString();
    return is;
}

/*!
    \brief operator <<
    \param os
    \param s
    \return
*/
wxDataOutputStream &operator << (wxDataOutputStream &os, const std::string &s) {
    wxString v(s);
    os << v;
    return os;
}

/*!
    \brief operator >>
    \param is
    \param d
    \return
*/
wxDataInputStream &operator  >> (wxDataInputStream &is, wxDateTime &d) {
    wxString s;
    is >> s;
    d.ParseISOCombined(s);
    return is;
}

/*!
    \brief operator <<
    \param os
    \param d
    \return
*/
wxDataOutputStream &operator << (wxDataOutputStream &os, const wxDateTime &d) {
    os << d.FormatISOCombined();
    return os;
}

/*!
    \brief operator >>
    \return
*/
wxDataInputStream &operator  >> (wxDataInputStream &is, wxTimeSpan &t) {
    wxLongLong l;
    is >> l;
    t = wxTimeSpan::Seconds(l);
    return is;
}

/*!
    \brief operator <<
    \return
*/
wxDataOutputStream &operator << (wxDataOutputStream &os, const wxTimeSpan &t) {
    os << t.GetSeconds();
    return os;
}


/*!
    \brief operator >>
    \return
*/
wxDataInputStream &operator  >> (wxDataInputStream &is, size_t &t) {
    is >> t;
    return is;
}

/*!
    \brief operator <<
    \return
*/
wxDataOutputStream &operator << (wxDataOutputStream &os, const size_t t) {
    os << wxLongLong(t);
    return os;
}



// The default type set
static MRL::VariantStreamer<char> _Char("char") ;
//
static class BoolStreamer : public MRL::VariantStreamer<bool> {
public:
    BoolStreamer() : VariantStreamer("bool") {}
    std::string toString(const boost::any &a) {
        bool d = boost::any_cast<bool>(a);
        return d ? "true" : "false";
    }

    boost::any fromString(const std::string &s) {
        bool f = (s == "true");
        return boost::any(f);
    }
    void toJson(Json::Value &v, const boost::any &a) {
        // default JSON handling
        v = boost::any_cast<bool>(boost::any(a));
    }

    virtual std::string toString(const MRL::Variant &v)
    {
        return boost::get<bool>(v)?std::string("true"):std::string("false");
    }

    virtual MRL::Variant toVariant(const std::string &s)
    {
        return MRL::Variant((s == "true")?true:false);
    }


} _BoolS;
//
static class IntStreamer : public MRL::VariantStreamer<int> {
public:
    IntStreamer() : VariantStreamer("int") {}
    std::string toString(const boost::any &a) {
        return std::to_string(boost::any_cast<int>(a));
    }

    boost::any fromString(const std::string &s) {
        int i = std::stoi(s);
        return boost::any(i);
    }
    void toJson(Json::Value &v, const boost::any &a) {
        // default JSON handling
        v = boost::any_cast<int>(boost::any(a));
    }

    virtual std::string toString(const MRL::Variant &v)
    {
        return std::to_string(boost::get<int>(v));
    }

    virtual MRL::Variant toVariant(const std::string &s)
    {
        return MRL::Variant(std::stoi(s));
    }


} _Int;

static class UnsignedStreamer : public MRL::VariantStreamer<unsigned> {
public:
    UnsignedStreamer() : VariantStreamer("unsigned") {}
    std::string toString(const boost::any &a) {
        return std::to_string(boost::any_cast<unsigned>(a));
    }

    boost::any fromString(const std::string &s) {
        unsigned i = std::stoul(s);
        return boost::any(i);
    }
    void toJson(Json::Value &v, const boost::any &a) {
        // default JSON handling
        v = boost::any_cast<unsigned int>(boost::any(a));
    }

    virtual std::string toString(const MRL::Variant &v)
    {
        return std::to_string(boost::get<unsigned int>(v));
    }

    virtual MRL::Variant toVariant(const std::string &s)
    {
        return MRL::Variant((unsigned int)std::stoul(s));
    }



} _Unsigned;




static class DoubleStreamer : public MRL::VariantStreamer<double> {
public:
    DoubleStreamer() : VariantStreamer("double") {}
    std::string toString(const boost::any &a) {
        return std::to_string(boost::any_cast<double>(a));
    }

    boost::any fromString(const std::string &s) {
        double i = std::stod(s);
        return boost::any(i);
    }
    void toJson(Json::Value &v, const boost::any &a) {
        // default JSON handling
        v = boost::any_cast<double>(boost::any(a));
    }
    virtual std::string toString(const MRL::Variant &v)
    {
        return std::to_string(boost::get<double>(v));
    }

    virtual MRL::Variant toVariant(const std::string &s)
    {
        return MRL::Variant(std::stod(s));
    }


} _Double;

static class LongLongStreamer : public MRL::VariantStreamer<long long> {
public:
    LongLongStreamer() : VariantStreamer("longlong") {}
    std::string toString(const boost::any &a) {
        return std::to_string(boost::any_cast<long long>(a));
    }

    boost::any fromString(const std::string &s) {
        long long  i = std::stoll(s);
        return boost::any(i);
    }
    virtual void toJson(Json::Value &v, const boost::any &a) {
        // default JSON handling
        v = Json::Int64(boost::any_cast<long long>(boost::any(a)));
    }

    virtual std::string toString(const MRL::Variant &v)
    {
        return std::to_string(boost::get<long long>(v));
    }

    virtual MRL::Variant toVariant(const std::string &s)
    {
        return MRL::Variant(std::stoll(s));
    }

} _LongLong;

static class UnsignedLongLongStreamer : public MRL::VariantStreamer<unsigned long long> {
public:
    UnsignedLongLongStreamer() : VariantStreamer("unsignedlonglong") {}
    std::string toString(const boost::any &a) {
        return std::to_string(boost::any_cast<unsigned long long>(a));
    }

    boost::any fromString(const std::string &s) {
        unsigned long long i = std::stoull(s);
        return boost::any(i);
    }
    void toJson(Json::Value &v, const boost::any &a) {
        // default JSON handling
        v = Json::UInt64(boost::any_cast<unsigned long long >(boost::any(a)));
    }

    virtual std::string toString(const MRL::Variant &v)
    {
        return std::to_string(boost::get<unsigned long long>(v));
    }

    virtual MRL::Variant toVariant(const std::string &s)
    {
        return MRL::Variant(std::stoull(s));
    }


} _UnsignedLongLong;
//

static class StringStreamer : public MRL::VariantStreamer<std::string> {
public:
    StringStreamer() : VariantStreamer("string") {}

} _String;

//
// need to define to and from string functions for XML encoding
static class DateTimeStreamer : public MRL::VariantStreamer<wxDateTime> {
public:
    DateTimeStreamer() : VariantStreamer("DateTime") {

    }

    std::string toString(const boost::any &a) {
        wxDateTime d = boost::any_cast<wxDateTime>(a);
        return d.FormatISOCombined().ToStdString();
    }

    boost::any fromString(const std::string &s) {
        wxDateTime d;
        wxString w(s);
        d.ParseISOCombined(w);
        return boost::any(d);
    }

    void toJson(Json::Value &v, const boost::any &a) {
        wxDateTime d = boost::any_cast<wxDateTime>(a);
        v[JSON_OBJECT_TYPE] = id(); // set the object type
        v["Date"] = d.FormatISOCombined().ToStdString();
    }

    void fromJson(Json::Value &v, boost::any &a) {
        wxDateTime d;
        wxString w(v.asString());
        d.ParseISOCombined(w);
        a = boost::any(d);
    }

    virtual std::string toString(const MRL::Variant &v)
    {
        time_t t = (time_t)(boost::get<int>(v));
        wxDateTime d(t);
        return d.FormatISOCombined().ToStdString();
    }

    virtual MRL::Variant toVariant(const std::string &s)
    {
        wxDateTime d;
        d.ParseISOCombined(s);
        return MRL::Variant((int) d.GetTicks());
    }



} _dateTime;

/*!
*/
static class TimeSpanStreamer : public MRL::VariantStreamer<wxTimeSpan> {
public:
    TimeSpanStreamer() : VariantStreamer("TimeSpan") {}

    std::string toString(const boost::any &a) {
        wxTimeSpan t = boost::any_cast<wxTimeSpan>(a);
        return std::to_string(t.GetSeconds().ToLong());
    }

    boost::any fromString(const std::string &s) {
        wxLongLong l = std::stol(s);
        wxTimeSpan t = wxTimeSpan::Seconds(l);
        return boost::any(t);
    }

    void toJson(Json::Value &v, const boost::any &a) {
        wxTimeSpan t = boost::any_cast<wxTimeSpan>(a);
        v[JSON_OBJECT_TYPE] = id(); // set the object type
        v["Time"] = int(t.GetSeconds().ToLong());
    }

    void fromJson(Json::Value &v, boost::any &a) {
        a = boost::any(wxTimeSpan::Seconds(v.get("Time", 0).asUInt()));
    }

    virtual std::string toString(const MRL::Variant &v)
    {
        return std::to_string(boost::get<int>(v));
    }

    virtual MRL::Variant toVariant(const std::string &s)
    {
        int i = std::stoi(s);
        return MRL::Variant((int)i);
    }


} _timeSpan;
//

/*!
    \brief setJson
    \param v
    \param a
*/
void MRL::setJson(Json::Value &v, Variant &a) {

    if (typeid(int).hash_code() == a.type().hash_code()) {
        v = boost::get<int>(a);
    }
    else if (typeid(unsigned).hash_code() == a.type().hash_code()) {
        v = boost::get<unsigned>(a);
    }
    else if (typeid(double).hash_code() == a.type().hash_code()) {
        v = boost::get<double>(a);
    }
    else if (typeid(std::string).hash_code() == a.type().hash_code()) {
        v =  boost::get<std::string>(a);
    }
    else if (typeid(bool).hash_code() == a.type().hash_code()) {
        v = boost::get<bool>(a);
    }
    else if (typeid(std::string).hash_code() == a.type().hash_code()) {
        v =  boost::get<std::string>(a);
    }
    else if (typeid(time_t).hash_code() == a.type().hash_code()) {
        v = Json::LargestUInt(boost::get<time_t>(a));
    }
    else if (typeid(long long).hash_code() == a.type().hash_code()) {
        v = Json::LargestInt(boost::get<long long>(a));
    }
    else if (typeid(unsigned long long).hash_code() == a.type().hash_code()) {
        v = Json::LargestUInt(boost::get<unsigned long long>(a));
    }
    else if (typeid(size_t).hash_code() == a.type().hash_code()) {
        v = Json::LargestUInt(boost::get<size_t>(a)); // one way
    }
    else {
        v = "Unknown";
    }
}

/*!
    \brief getJson
    \param v
    \param a
*/
void MRL::getJson(Json::Value &v, Variant &a) {
    switch (v.type()) {
    case Json::intValue:      ///< signed integer value
        a = v.asInt();
        break;
    case Json::uintValue:     ///< unsigned integer value
        a = v.asUInt();
        break;
    case Json::realValue:     ///< double value
        a = v.asDouble();
        break;
    case Json::stringValue:   ///< UTF-8 string value
        a = v.asString();
        break;
    case Json::booleanValue:  ///< bool value
        a = v.asBool();
        break;
    case Json::nullValue: ///< 'null' value
    case Json::arrayValue:    ///< array value (ordered list)
    case Json::objectValue:    ///< object value (collection of name/value pairs).
        a = int(0);
        break;
    default:
        break;
    }
}


/*!
    \brief setJson
    \param v
    \param a
*/
void MRL::setJson(Json::Value &v, boost::any &a) {
    MRL::VariantStreamerBase::anyToJson(v, a);
}

/*!
    \brief getJson
    \param v
    \param a
*/
void MRL::getJson(Json::Value &v, boost::any &a) {
    // Get FROM JSON any any
    switch (v.type()) {
    case Json::nullValue:     ///< 'null' value
        a = std::string("");
        break;
    case Json::intValue:      ///< signed integer value
        a = v.asInt();
        break;
    case Json::uintValue:     ///< unsigned integer value
        a = v.asUInt();
        break;
    case Json::realValue:     ///< double value
        a = v.asDouble();
        break;
    case Json::stringValue:   ///< UTF-8 string value
        a = v.asString();
        break;
    case Json::booleanValue:  ///< bool value
        a = v.asBool();
        break;
    case Json::arrayValue:    ///< array value (ordered list)
        break;
    case Json::objectValue: { ///< object value (collection of name/value pairs).
        // Workout object type
        a = MRL::VariantStreamerBase::anyFromJson(v, v.get(JSON_OBJECT_TYPE, "").asString());
    }
    break;
    default:
        break;
    }
}

/*!
    \brief MRL::toString
    Could use visitors but .... this may be more efficent - exploits all type names start with different letter WARNING !!!!!
    \param v
    \return
*/
std::string MRL::toString(const Variant &v) {
    std::string res = "";
    const char *tn = v.type().name();

    try
    {
            if (typeid(int).hash_code() == v.type().hash_code()) {
                return std::to_string(boost::get<int>(v));
            }
            else if (typeid(unsigned).hash_code() == v.type().hash_code()) {
                return std::to_string(boost::get<unsigned>(v));
            }
            else if (typeid(double).hash_code() == v.type().hash_code()) {
                return std::to_string(boost::get<double>(v));
            }
            else if (typeid(std::string).hash_code() == v.type().hash_code()) {
                return boost::get<std::string>(v);
            }
            else if (typeid(bool).hash_code() == v.type().hash_code()) {
                return std::string(boost::get<bool>(v) ? "true" : "false");
            }
            else if (typeid(std::string).hash_code() == v.type().hash_code()) {
                return boost::get<std::string>(v);
            }
            else if (typeid(time_t).hash_code() == v.type().hash_code()) {
                return std::to_string(boost::get<time_t>(v));
            }
            else if (typeid(long long).hash_code() == v.type().hash_code()) {
                return std::to_string(boost::get<long long>(v));
            }
            else if (typeid(unsigned long long).hash_code() == v.type().hash_code()) {
                return std::to_string(boost::get<unsigned long long>(v));
            }
            else if (typeid(size_t).hash_code() == v.type().hash_code()) {
                return std::to_string(boost::get<size_t>(v));
            }
            else {
                return "Unknown";
            }
    }
    catch(...)
    {
        std::cerr << "Unknown Type " << tn << std::endl;
    }

    return res;
}



