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
#ifndef VARIANTPROPERTYTREE_H
#define VARIANTPROPERTYTREE_H

#include "mrllib.h"
#include "propertytree.h"
#include "variant.h"

namespace MRL {


inline const std::string valueToString(const MRL::Variant &v) {
    return toString(v);
}

template <typename T> const T &valueToType(const MRL::Variant &v) {
    return boost::get<T>(v);
}


/*!
    \brief The VariantTree class
*/
template <typename V>
class ValueTree : public PropertyTree<std::string, V> {

public:

    typedef  Node<std::string, V> ValueNode;
    typedef NodePath<std::string> ValuePath;
    /*!
        \brief VariantPropertyTree
    */
    ValueTree() {}
    virtual ~ValueTree() {}


    template <typename P>
    /*!
     * \brief exists
     * \param path base path
     * \param c sub item
     * \return true if the item exists
     */
    bool existsub(P &path,const std::string &c) {
        bool ret = false;
        if (!c.empty()) {
            path.push_back(c);
            ret =  PropertyTree<std::string, V>::exists(path);
            path.pop_back();
        }
        return ret;
    }



    //
    template <typename P, typename T>
    /*!
        \brief setValue
        \param path
        \param v
    */
    void setValue(P path, const T &v) {
        T a(v);
        this->set(path, a);
    }


    //
    template <typename T>
    /*!
        \brief setValue
        \param path
        \param v
    */
    void setValue(MRL::PropertyPath &path, const std::string &c,  const T &v) {
        if (!c.empty()) {
            path.push_back(c);
            V a(v);
            this->set(path, a);
            path.pop_back();
        }
    }

    //
    template<typename P>
    /*!
        \brief getAsWxString
        \param path
        \return
    */
    wxString getAsWxString(P path) {
        try {
            V &a = this->get(path);
            if (!a.empty()) {
                std::string s = valueToString(a); // intelligent conversion
                return wxString(s);
            }
        }
        catch (...) {

        }
        return wxString("");
    }

    /*!
     * \brief getAsWxString
     * \param p
     * \param s
     * \return
     */
    wxString getAsWxString(MRL::PropertyPath &p, const std::string &s) {
        try {
            ReadLock l(this->mutex());
            p.push_back(s);
            std::string s = getAsString(p); // intelligent conversion
            p.pop_back();
            return wxString(s);
        }
        catch (...) {
        }
        return wxString("");
    }


    template<typename P>
    /*!
        \brief getAsWxString
        \param path
        \return
    */
    std::string getAsString(P path) {
        try {
            ReadLock l(this->mutex());
            V &a = this->get(path);
            if (!a.empty()) {
                std::string s = valueToString(a); // intelligent conversion
                return s;
            }
        }
        catch (...) {

        }
        return std::string("");
    }


    /*!
        \brief getAsString
        \param path
        \return
    */
    std::string getAsString(MRL::PropertyPath &p, const std::string &s) {
        try {
            ReadLock l(this->mutex());
            p.push_back(s);
            std::string s = getAsString(p); // intelligent conversion
            p.pop_back();
            return s;
        }
        catch (...) {
        }
        return std::string("");
    }



    template <typename T, typename P>
    /*!
        \brief getValue
        \param path
        \return
    */
    T getValue(P path) {
        try {
            ReadLock l(this->mutex());
            auto *n = this->root().find(path);
            if (n) {
                V &a = n->data();
                if (!a.empty()) {
                    return  valueToType<T>(a);
                }
            }
        }
        catch (...) {

        }
        return T();
    }

    template <typename T>
    /*!
        \brief getValue
        \param path
        \return
    */
    T getValue(MRL::PropertyPath &p, const std::string &c) {
        if (!c.empty()) {
            try {
                ReadLock l(this->mutex());
                p.push_back(c);
                auto *n = this->root().find(p);
                p.pop_back();
                if (n) {
                    V &a = n->data();
                    if (!a.empty()) {
                        return  valueToType<T>(a);
                    }
                }
            }
            catch (...) {

            }
        }
        return T();
    }



    template <typename P>
    /*!
        \brief list
        \param path
        \param l
    */
    int list(const P &path, wxArrayString  &l) {
        l.Clear();
        auto i =  this->node(path);
        if (i) {
            for (auto j = i->children().begin(); j != i->children().end(); j++) {
                l.Add(wxString(j->first));
            }
        }
        return l.Count();
    }
    //
    //


    /*!
        \brief MRL::VariantPropertyTree::load
        \param file
        \return
    */
    bool load(const std::string &file) { // load from file
        try {
            wxXmlDocument doc;
            if (doc.Load(file)) {
                fromXml(doc);
                return true;
            }
            return false;
        }
        catch (...) {
            wxLogDebug("Exception in load");
        }
        return false;
    }

    /*!
        \brief MRL::VariantPropertyTree::save
        \param file
        \return
    */
    bool save(const std::string &file) { // save to file
        try {
            wxXmlDocument doc;
            toXml(doc);
            return doc.Save(file);
        }
        catch (...) {
            wxLogDebug("Exception in save");
        }
        return false;
    }

    /*!
        \brief MRL::VariantPropertyTree::sync
        \param tree
    */
    void sync(ValueTree &/*tree*/) {

    }

    /*!
        \brief printNode
        \param os
        \param n
        \param level
    */
    void printNode(std::ostream &os, ValueNode *n, int level) {
        if (n) {
            std::string indent(level, ' ');
            os << indent << n->name() << " : "<< valueToString(n->data()) << std::endl;
            if (n->children().size() > 0) {
                level++;
                for (auto i = n->children().begin(); i != n->children().end(); i++) {
                    printNode(os, i->second, level); // recurse
                }
            }
        }
    }

    /*!
        \brief fromJson
        \param n
        \param v
    */
    void fromJson(ValueNode *n, Json::Value &v) {
        if (n && !v.isNull()) {
            // get the value
            if (v.isObject()) {
                Json::Value::Members m = v.getMemberNames();
                for (int i = 0; i < int(m.size()); i++) {
                    Json::Value &cv =  v[m[i]];
                    if (!cv.isNull()) { // can only recurse objects
                        ValueNode *ch = new ValueNode(m[i], n);
                        fromJson(ch, cv);
                        n->addChild(ch);
                    }
                }
            }
            else {
                getJson(v, n->data()); // terminal leaf
            }
        }
    }

    /*!
        \brief toJson
        \param n
        \param v
    */
    void toJson(ValueNode *n, Json::Value &v) {
        if (n) {
            // to save as JSON only nodes with no children can store a value
            if (n->children().size() > 0) {
                for (auto i = n->children().begin(); i != n->children().end(); i++) {
                    ValueNode *nc = i->second;
                    if (nc) {
                        Json::Value c;
                        const std::string &s = i->first;
                        toJson(nc, c);
                        v[s] = c;
                    }
                }
            }
            else {
                setJson(v, n->data()); // set the JSON data
            }
        }
    }

    /*
        \brief toXml
        \param n
        \param parentNode
    */
    void toXml(ValueNode *n, wxXmlNode *parentNode) {
        if (n) {
            setXml(parentNode, n->data()); // set the XML data
            if (n->children().size() > 0) {
                for (auto i = n->children().begin(); i != n->children().end(); i++) {
                    wxXmlNode *x = new wxXmlNode(parentNode, wxXML_ELEMENT_NODE, "NODE");
                    x->AddAttribute("Name", wxString(i->first));
                    toXml(i->second, x);
                }
            }
        }
    }

    //
    // load from XML
    //
    /*!
        \brief fromXml
        \param n
        \param x
    */
    void fromXml(ValueNode *n, wxXmlNode *x) {
        if (n) {
            n->clear();
            getXml(x, n->data());
            wxXmlNode *c = x->GetChildren();
            while (c != nullptr) {
                if (c->GetType() == wxXML_ELEMENT_NODE) { // only want element nodes
                    std::string cn = c->GetAttribute("Name").ToStdString();
                    if (!cn.empty()) { // skip empty nodes
                        ValueNode *ch = new ValueNode(cn, n);
                        fromXml(ch, c); // recurse
                        n->addChild(ch); // add to tree
                    }
                }
                c = c->GetNext();
            }
        }
    }



    //
    // JSON
    //


    /*!
        \brief toJson
        \param v
    */
    bool toJson(Json::Value &v) {
        // whole tree
        try {
            toJson(this->rootNode(), v);
        }
        catch (...) {
            return false;
        }
        return true;
    }

    /*!
        \brief fromJson
        \param v
    */
    bool  fromJson(Json::Value &v) {
        // whole tree
        try {
            this->clear();
            fromJson(this->rootNode(), v);
        }
        catch (...) {
            return false;
        }
        return true;
    }
    //
    // Generate XML
    //

    /*!
        \brief toXml
        \param d
    */
    void toXml(wxXmlDocument &d) {
        wxXmlNode *r = new wxXmlNode(wxXML_ELEMENT_NODE, "root");
        d.SetRoot(r);
        toXml(this->rootNode(), r);
    }

    /*!
        \brief fromXml
        \param d
    */
    void fromXml(wxXmlDocument &d) {
        this->clear();
        fromXml(this->rootNode(), d.GetRoot());
    }
    //


    /*!
        \brief dump the property tree
        \param os
    */
    void dump(std::ostream &os = std::cerr) {
        this->printNode(os, this->rootNode(), 0);
    }

    //
    //  Selena interface to the tree - non virtual/templated
    //
    /*!
        \brief getBool
        \param address
        \return
    */
    bool getBool(std::string address) {
        return getValue<bool>(address);
    }
    /*!
        \brief getInt
        \param address
        \return
    */
    int getInt(std::string address) {
        return getValue<int>(address);

    }
    /*!
        \brief getDouble
        \param address
        \return
    */
    double getDouble(std::string address) {
        return getValue<double>(address);
    }
    /*!
        \brief getString
        \param address
        \return
    */
    std::string getString(std::string address) {
        return getValue<std::string>(address);

    }

    /*!
        \brief setBool
        \param address
        \param v
    */
    void setBool(std::string address, bool v) {
        setValue(address, v);
    }

    /*!
        \brief setInt
        \param address
        \param v
    */
    void setInt(std::string address, int v) {
        setValue(address, v);
    }

    /*!
        \brief setDouble
        \param address
        \param v
    */
    void setDouble(std::string address, double v) {
        setValue(address, v);
    }

    /*!
        \brief setString
        \param address
        \param v
    */
    void setString(std::string address, const std::string &v) {
        setValue(address, v);
    }

    /*!
        \brief erase
        \param address
    */
    void erase(std::string address) {
        this->remove(address);
    }
};

typedef ValueTree<MRL::Variant> VariantPropertyTree;

}






#endif // VARIANTPROPERTYTREE_H
