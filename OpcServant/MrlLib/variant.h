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
#ifndef VARIANT_H
#define VARIANT_H
#include "mrllib.h"
#include "propertytree.h"
#include <map>
#include <typeinfo>
#include <string>
#include <functional>
#include <memory>
#include <boost/any.hpp>
#include <wx/datstrm.h>
#include <wx/xml/xml.h>
#include <boost/variant.hpp>
#include <list>
#include <boost/bimap.hpp>
//

namespace MRL {
    typedef boost::variant<int, unsigned int, double, std::string, bool, time_t, void *, long long, unsigned long long> Variant;
    typedef std::list<Variant> VariantList; //!< list of variants
    typedef boost::shared_ptr<VariantList> VariantListPtr;
    typedef std::list<std::string> StringList;
    typedef std::map<std::string,Variant> VariantMap;
    typedef boost::shared_ptr<VariantMap> VariantMapPtr;
    std::string toString(const Variant &v);

    /*!
        \brief PropertyPath
    */
    typedef NodePath<std::string> PropertyPath;
}


// These classes and functions enable casting between strings and variants
//
//
MRLLIBSHARED_EXPORT wxDataInputStream &operator  >> (wxDataInputStream &is, boost::any &a);
MRLLIBSHARED_EXPORT wxDataOutputStream &operator << (wxDataOutputStream &os, const boost::any &a);
// Streams
MRLLIBSHARED_EXPORT wxDataInputStream &operator  >> (wxDataInputStream &, std::string &);
MRLLIBSHARED_EXPORT wxDataOutputStream &operator << (wxDataOutputStream &, const std::string &);

/*!
    \brief operator >>
    \param is
    \param f
    \return
*/
inline wxDataInputStream &operator  >> (wxDataInputStream &is, bool &f) {
    char c = is.Read8();
    f = c != 0;
    return is;
}

/*!
    \brief operator <<
    \param os
    \param c
    \return
*/
inline wxDataOutputStream &operator << (wxDataOutputStream &os, char c) {
    os.Write8(c);
    return os;
}

/*!
    \brief operator >>
    \param is
    \param c
    \return
*/
inline wxDataInputStream &operator  >> (wxDataInputStream &is, char &c) {
    c = is.Read8();
    return is;
}

/*!
    \brief operator <<
    \param os
    \param f
    \return
*/
inline wxDataOutputStream &operator << (wxDataOutputStream &os, bool f) {
    os.Write8(f ? 1 : 0);
    return os;
}




//
// Basic types we need
//
MRLLIBSHARED_EXPORT wxDataInputStream &operator  >> (wxDataInputStream &, wxDateTime &);
MRLLIBSHARED_EXPORT wxDataOutputStream &operator << (wxDataOutputStream &, const wxDateTime &);
MRLLIBSHARED_EXPORT wxDataInputStream &operator  >> (wxDataInputStream &, wxTimeSpan &);
MRLLIBSHARED_EXPORT wxDataOutputStream &operator << (wxDataOutputStream &, const wxTimeSpan &);
//
//
// to stream variants in native form needs a map of stream objects to type name
//
namespace MRL {
    // encode into XML
    MRLLIBSHARED_EXPORT void setXml(wxXmlNode *, boost::any &);
    MRLLIBSHARED_EXPORT void getXml(wxXmlNode *, boost::any &);
    //
    MRLLIBSHARED_EXPORT void setJson(Json::Value &, boost::any &);
    MRLLIBSHARED_EXPORT void getJson(Json::Value &, boost::any &);

    MRLLIBSHARED_EXPORT void setXml(wxXmlNode *, Variant &);
    MRLLIBSHARED_EXPORT void getXml(wxXmlNode *, Variant &);
    //
    MRLLIBSHARED_EXPORT void setJson(Json::Value &, Variant &);
    MRLLIBSHARED_EXPORT void getJson(Json::Value &, Variant &);



    /*!
        \brief The VariantStreamerBase class
    */
    class MRLLIBSHARED_EXPORT VariantStreamerBase {
            std::string _id; // class idenitfier string
        protected:
            static std::map<std::string, VariantStreamerBase *> _map; // portable name
            static std::map<std::string, VariantStreamerBase *> _typemap; // name returned by typeid().name

        public:
            // self register
            VariantStreamerBase(const std::string &s) : _id(s) {
                _map[s] = this;
            }
            virtual ~VariantStreamerBase() {
                _map.erase(_id);
            }
            //
            virtual const char *id() const {
                return _id.c_str();
            }
            virtual bool read(wxDataInputStream &,   boost::any &) {
                return false;
            }
            virtual bool write(wxDataOutputStream &, const boost::any &) {
                return false;
            }
            // Used for XML encoding
            virtual std::string toString(const boost::any &a) {
                return boost::any_cast<std::string>(a);
            }
            virtual boost::any fromString(const std::string &s) {
                return boost::any(s);
            }

            virtual std::string toString(const Variant &v)
            {
                    return boost::get<std::string>(v);
            }

            virtual Variant toVariant(const std::string &s)
            {
                return Variant(s);
            }


            //
            virtual void toJson(Json::Value &v, const boost::any &a) {
                v = boost::any_cast<std::string>(a);
            } // setJson should handle base types
            //
            virtual void fromJson(Json::Value &v, boost::any &a) {
                a = v.asString();
            } // getJson should handle base types
            //
            static VariantStreamerBase *find(const std::string &s) {
                if(_map.find(s) == _map.end() ) return nullptr;
                return _map[s];
            }
            static VariantStreamerBase *findByTypename(const std::string &s) {
                if(_typemap.find(s) == _typemap.end()) return nullptr;
                return _typemap[s];
            }
            //
            /*!
                \brief anyToString
                \param a
                \return
            */
            static std::string anyToString(const boost::any &a) {
                VariantStreamerBase *v = findByTypename(a.type().name());
                if (v) {
                    return v->toString(a);
                }
                std::string uk = "(Unknown:" + std::string(a.type().name()) + ")";
                return uk;
            }
            /*!
                \brief anyFromString
                \param s
                \param t
                \return
            */
            static boost::any anyFromString(const std::string &s, const std::string &t) {
                VariantStreamerBase *v = find(t);
                if (v) {
                    return v->fromString(s);
                }
                return boost::any(std::string("Unknown"));
            }

            static void anyToJson(Json::Value &val, const boost::any &a) {
                VariantStreamerBase *v = findByTypename(a.type().name());
                if (v) {
                    v->toJson(val, a);
                }
            }
            /*!
                \brief anyFromString
                \param s
                \param t
                \return
            */
            static boost::any anyFromJson(Json::Value &val, const std::string &t) {
                VariantStreamerBase *v = find(t);
                if (v) {
                    boost::any a;
                    v->fromJson(val, a);
                    return a;
                }
                return boost::any(std::string("Unknown"));
            }

            /*!
                \brief anyToString
                \param a
                \return
            */
            static std::string variantToString(const Variant &a) {
                VariantStreamerBase *v = findByTypename(a.type().name());
                if (v) {
                    return v->toString(a);
                }
                std::string uk = "(Unknown:" + std::string(a.type().name()) + ")";
                return uk;
            }
            /*!
                \brief anyFromString
                \param s
                \param t
                \return
            */
            static Variant variantFromString(const std::string &s, const std::string &t) {
                VariantStreamerBase *v = find(t);
                if (v) {
                    return v->toVariant(s);
                }
                return Variant(std::string("Unknown"));
            }




    };

    // generic temlate
    template <typename T>
    /*!
        \brief The VariantStreamer class
    */
    class VariantStreamer : public VariantStreamerBase {
        public:
            /*!
                \brief VariantStreamer
                \param s
            */
            VariantStreamer(const std::string &s) : VariantStreamerBase(s) {
                _typemap[typeid(T).name()] = this; // internal name which may be mangled
            }
            /*!
                \brief read
                \param is
                \param a
                \return
            */
            bool read(wxDataInputStream &is, boost::any &a) {
                try {
                    T v;
                    is >> v;
                    a = boost::any(v);
                    return true;
                }
                catch (...) {

                }
                return false;
            }
            /*!
                \brief write
                \param os
                \param a
                \return
            */
            bool write(wxDataOutputStream &os, const boost::any &a) {
                try {
                    os << id() << boost::any_cast<T>(a);
                    return true;
                }
                catch (...) {

                }
                return false;
            }

            /*!
                \brief fromString
                \param s
                \return
            */
            virtual boost::any fromString(const std::string &s) {
                T v = boost::any_cast<T>(boost::any(s)); // go from string to type
                return boost::any(v);
            }
    };

}



#endif // VARIANT_H
