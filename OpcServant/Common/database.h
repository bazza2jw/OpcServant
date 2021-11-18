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
#ifndef DATABASE_H
#define DATABASE_H
#include <MrlLib/mrllib.h>
#include <MrlLib/sqlitedb.h>
#include <MrlLib/variantpropertytree.h>
#include <Wt/WObject.h>

namespace MRL {
constexpr const char *DEFAULT_RECIPE = "(default)";
/*!
     \brief The DatabaseNode struct

     The stores the data for the objects.
     The object directory is stored in a property tree
     The data is keyed by an object id

     Recipes allow configurations to be modified. There is always a "(default)" recipe.
     The data is in three parts:
     Data independent of recipe
     Recipe dependent data
     Notes/comments for object


*/
struct DatabaseNode {
    unsigned _id = -1;      //!< the node Id keys database access
    unsigned _type = 0;     //!< the type (hash of the type name)
    DatabaseNode() = default;
    DatabaseNode(const DatabaseNode &) = default;
    unsigned id() const {
        return _id;
    }
    unsigned type() {
        return _type;
    }
};


typedef PropertyTree<std::string, DatabaseNode> DataTree; //!< tree
typedef PropertyTree<std::string, DatabaseNode>::PropertyNode DataNode; //!< tree node
typedef std::map<unsigned, DataNode *> DataNodeIdMap;


typedef std::list<DatabaseNode> DatabaseNodeList;
/*!
    \brief The Database class
    Configuration database on the server side
*/
class Database : public Wt::WObject {
    ReadWriteMutex _mutex; // control access
    unsigned _nextId = 100;
    std::string _dir;  //!< Data directory
    std::string _file; //!< Data filename

    SQLiteDB _db; //!< the database connection - the database is private to the server only access through server methods
    DataTree _tree; //!< index to objects as a tree
    DataNodeIdMap _idMap; //!< map object id to node hence to path
    //
    // Prepare statements - matches functions
    //
    SqlLiteStatement _addData;
    SqlLiteStatement _getData;
    SqlLiteStatement _deleteData;
    SqlLiteStatement _updateData;

    //
    SqlLiteStatement _addRecipeData;
    SqlLiteStatement _getRecipeData;
    SqlLiteStatement _deleteRecipeData;
    SqlLiteStatement _deleteAllRecipeData;
    //
    SqlLiteStatement _addNotes;
    SqlLiteStatement _getNotes;
    SqlLiteStatement _deleteNotes;

    SqlLiteStatement _addAlias;
    SqlLiteStatement _getAlias;
    SqlLiteStatement _deleteAlias;
    SqlLiteStatement _updateAlias;
    SqlLiteStatement _listAlias;




public:
    /*!
     * \brief Database
     * \param d
     * \param f
     */
    Database(const std::string &d = "", const std::string &f = "") :  _dir(d), _file(f)  {}
    /*!
     * \brief ~Database
     */
    virtual ~Database() {
        _idMap.clear();
    }

    bool createDatabase(); // create database table, indices and triggers

    // Accessors

    SQLiteDB &db() {
        return _db;
    }
    DataTree &tree() {
        return _tree;
    }
    const std::string &dir() {
        return _dir;
    }
    const std::string &file() {
        return _file;
    }

    void setFile(const std::string &f) {
        _file = f;
    }

    void setDir(const std::string &d) {
        _dir = d;
    }

    bool exists(const PropertyPath &path) {
        return _tree.exists(path);
    }

    DataNodeIdMap &idMap() {
        return _idMap;
    }

public:
    // Database access
    bool addData(unsigned id, unsigned type, const std::string &data);
    bool getData(unsigned id, std::string &data);
    bool updateData(unsigned id, const std::string &data);
    bool deleteData(unsigned id);
    //
    bool addRecipeData(unsigned id, const std::string &r,  const std::string &data);
    bool getRecipeData(unsigned id, const std::string &r,  std::string &data);
    bool deleteRecipeData(unsigned id, const std::string &r);
    bool deleteAllRecipeData(unsigned id);
    //
    //
    bool addNotes(unsigned id, const std::string &data);
    bool getNotes(unsigned id, std::string &data);
    bool deleteNotes(unsigned id);
    //
    bool addAlias(const std::string &id, const std::string &data);
    bool getAlias(const std::string & id, std::string &data);
    bool updateAlias(const std::string & id, const std::string &data);
    bool deleteAlias(const std::string & id);
    bool listAlias(StringMap &,StringMap &);
    //
public:
    bool load(); // load a configuration file
    bool save(); // load a configuration file
    //
    // tree based access
    //
    unsigned find(const PropertyPath &path); // map from path to id
    unsigned find(const std::string &s) // helper
    {
        PropertyPath p;
        p.toList(s);
        return find(p);
    }
    unsigned typeId(const PropertyPath &path); // get the object type id
    unsigned typeId(const std::string &s) // helper
    {
        PropertyPath p;
        p.toList(s);
        return typeId(p);
    }
    bool add(const PropertyPath &path, unsigned type); // create an object in the tree
    bool updateData(const PropertyPath &path, const std::string &data); // create an object in the tree
    bool updateNotes(const PropertyPath &path, const std::string &data); // create an object in the tree
    bool updateRecipe(const PropertyPath &path, const std::string &recipe, const std::string &data); // create an object in the tree
    bool getData(const PropertyPath &path, std::string &data); // create an object in the tree
    bool getRecipe(const PropertyPath &path, const std::string &recipe, std::string &data); // create an object in the tree
    bool getNotes(const PropertyPath &path, std::string &data); // create an object in the tree
    bool remove(const PropertyPath &path); // remove from tree
    bool removeRecipe(const PropertyPath &path, const std::string &r); // remove a recipe
    //
    /*!
     * \brief updateData
     * \param path
     * \param data
     * \return true on success
     */
    bool updateData(const PropertyPath &path, Json::Value &data) { // create an object in the tree
        std::string d;
        jsonToString(data, d);
        return updateData(path, d);
    }
    /*!
     * \brief updateData
     * \param id
     * \param data
     * \return true on success
     */
    bool updateData(unsigned id, Json::Value &data)
    {
        std::string d;
        jsonToString(data, d);
        return updateData(id, d);
    }

    /*!
     * \brief updateNotes
     * \param path
     * \param data
     * \return true on success
     */
    bool updateNotes(const PropertyPath &path, Json::Value &data) { // create an object in the tree
        std::string d;
        jsonToString(data, d);
        return updateNotes(path, d);

    }

    /*!
     * \brief updateRecipe
     * \param path
     * \param recipe
     * \param data
     * \return true on success
     */
    bool updateRecipe(const PropertyPath &path, const std::string &recipe, Json::Value &data) {
        std::string d;
        jsonToString(data, d);
        return updateRecipe(path, recipe, d);

    }

    /*!
     * \brief getData
     * \param id
     * \param data
     * \return true on success
     */
    bool getData(unsigned id, Json::Value &data) {
        try {
            std::string d;
            if (getData(id, d)) {
                stringToJson(d, data);
                return true;
            }
        }
        catch (Json::Exception &e) {
            std::cerr << "getData() " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "getData() Exception" << std::endl;
        }
        return false;
    }

    /*!
     * \brief getData
     * \param path
     * \param data
     * \return true on success
     */
    bool getData(const PropertyPath &path, Json::Value &data) {
        try {
            std::string d;

            if (getData(path, d)) {

                stringToJson(d, data);
                return true;
            }
        }
        catch (Json::Exception &e) {
            std::cerr << "getData() " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "getData() Exception" << std::endl;
        }

        return false;
    }
    /*!
     * \brief getRecipe
     * \param path
     * \param recipe
     * \param data
     * \return true on success
     */
    bool getRecipe(const PropertyPath &path, const std::string &recipe, Json::Value &data) {
        std::string d;
        try {
            if (getRecipe(path, recipe, d)) {
                stringToJson(d, data);
                return true;
            }
        }
        catch (Json::Exception &e) {
            std::cerr << "getRecipe() " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "getRecipe() Exception" << std::endl;
        }
        return false;
    }
    /*!
     * \brief getNotes
     * \param path
     * \param data
     * \return true on success
     */
    bool getNotes(const PropertyPath &path, Json::Value &data) {
        std::string d;
        if (getNotes(path, d)) {
            stringToJson(d, data);
            return true;
        }
        return false;
    }

    //

    /*!
        \brief setJson
        \param v
        \param d
    */
    void setJson(Json::Value &v, DatabaseNode &d) {
        v["ID"] = d._id;
        v["TYPE"] = d._type;
    }
    /*!
        \brief getJson
        \param v
        \param d
    */
    void getJson(Json::Value &v, DatabaseNode &d) {
        d._id = v.get("ID", 0).asUInt();
        d._type = v.get("TYPE", 0).asUInt();
        wxLogDebug("Create Node id %d type %X", d._id, d._type);
    }

    /*!
        \brief toJson
        \param n start node
        \param v
    */
    void toJson(DataNode *n, Json::Value &v) {
        if (n) {
            setJson(v["_DATA_"], n->data());
            if (n->children().size() > 0) {
                for (auto i = n->children().begin(); i != n->children().end(); i++) {
                    toJson(i->second, v[i->first]);
                }
            }
        }
    }

    /*!
        \brief fromJson
        \param n
        \param v
    */
    void fromJson(DataNode *n, Json::Value &v) {
        if (n && v.isObject()) {
            getJson(v["_DATA_"], n->data());
            idMap()[n->data().id()] = n;
            Json::Value::Members m = v.getMemberNames();
            for (int i = 0; i < int(m.size()); i++) {
                if (m[i] != "_DATA_") {
                    try {
                        Json::Value &cv =  v[m[i]];
                        if (!cv.isNull()) {
                            DataNode *ch = new DataNode(m[i], n);
                            fromJson(ch, cv);
                            n->addChild(ch);
                        }
                    }
                    catch (Json::Exception &e) {
                        std::cerr << "fromJson JSON Exception " << e.what() << " item " << m[i] << std::endl;
                    }
                }
            }
        }
    }
    /*!
        \brief toJson
        \param v
    */
    void toJson(Json::Value &v) {
        tree().root().data()._id = _nextId; // save next Id into root
        toJson(tree().rootNode(), v);
    }

    /*!
        \brief fromJson
        \param v
    */
    void fromJson(Json::Value &v) {
        tree().clear();
        idMap().clear();
        fromJson(tree().rootNode(), v);
        _nextId = tree().root().data()._id; // get next id from root

    }

    /*!
        \brief saveToDb
        \return true on success
    */
    bool saveToDb() {
        bool ret = false;
        if (db().isConnected()) {
            Json::Value v;
            toJson(v);
            std::string d;
            jsonToString(v, d);
            ret = addData(0, 0, d);
        }
        return ret;
    }

    /*!
        \brief loadFromDb
        \return true on success
    */
    bool loadFromDb() {
        try {
            // load the tree from record zero
            std::string d;
            if (getData(0, d)) {
                Json::Value v;
                if (stringToJson(d, v)) {
                    fromJson(v);
                    return true;
                }
            }
        }
        catch (Json::Exception &e) {
            std::cerr << "loadFromDb() JSON Exception " << e.what() << std::endl;
        }

        return false;
    }

    /*!
        \brief getPath
        map object id to display path
        \param id
        \param path
        \return true on success
    */
    bool getPath(unsigned id, PropertyPath &path) {
        path.clear();
        auto p = idMap()[id];
        if (p) {
            tree().absolutePath(p, path);
            return true;
        }
        return false;
    }

    /*!
     * \brief resetDatabase
     */
    void resetDatabase(); // factory reset - clear tables and set defaults

};
}
#endif // DATABASE_H
