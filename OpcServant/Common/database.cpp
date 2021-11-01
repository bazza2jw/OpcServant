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
#include "database.h"
#include "objectmanager.h"
#include <fstream>
#include <istream>
#include <ostream>

/*!
    \brief MRL::Database::createDatabase
    \return
*/
bool MRL::Database::createDatabase() { // create database table, indices and triggers
    std::string  f = file() + ".DB";
    if (db().OpenConnection(f, dir())) {
        //
        // The non-recipe data part
        db().Execute(
            "CREATE TABLE IF NOT EXISTS DATA_TABLE "
            "(F_ID INTEGER NOT NULL PRIMARY KEY,F_TYPE UNSIGNED BIG INTEGER,F_DATA VARCHAR(1));"
        );

        //
        db().Execute(
            "CREATE TABLE IF NOT EXISTS NOTES_TABLE "
            "(F_ID INTEGER NOT NULL PRIMARY KEY, F_DATA VARCHAR(1));"
        );

        //
        // Recipe Table
        db().Execute(
            "CREATE TABLE IF NOT EXISTS RECIPE_TABLE "
            "(F_ID INTEGER,F_RECIPE VARCHAR(1), F_DATA VARCHAR(1), PRIMARY KEY(F_ID,F_RECIPE));"
        );

        db().Execute(
            "CREATE TABLE IF NOT EXISTS ALIAS_TABLE "
            "(F_ID VARCHAR(1), F_DATA VARCHAR(1), PRIMARY KEY(F_ID));"
        );

        //

        _addData.setDatabase(&_db);
        _getData.setDatabase(&_db);
        _deleteData.setDatabase(&_db);
        _updateData.setDatabase(&_db);
        //
        _addRecipeData.setDatabase(&_db);
        _getRecipeData.setDatabase(&_db);
        _deleteRecipeData.setDatabase(&_db);
        _deleteAllRecipeData.setDatabase(&_db);
        //
        _addNotes.setDatabase(&_db);
        _getNotes.setDatabase(&_db);
        _deleteNotes.setDatabase(&_db);
        //
        _addAlias.setDatabase(&_db);
        _getAlias.setDatabase(&_db);
        _deleteAlias.setDatabase(&_db);
        _updateAlias.setDatabase(&_db);
        _listAlias.setDatabase(&_db);
        //
        _updateData.prepare("UPDATE DATA_TABLE SET F_DATA=? WHERE F_ID=?;");
        _addData.prepare("INSERT  OR REPLACE INTO DATA_TABLE (F_ID,F_TYPE,F_DATA) VALUES (?,?,?);");
        _getData.prepare("SELECT F_DATA FROM DATA_TABLE WHERE F_ID = ?;");
        _deleteData.prepare("DELETE FROM DATA_TABLE WHERE F_ID = ?;");
        //
        _addRecipeData.prepare("INSERT  OR REPLACE INTO RECIPE_TABLE (F_ID,F_RECIPE, F_DATA) VALUES (?,?,?);");
        _getRecipeData.prepare("SELECT F_DATA FROM RECIPE_TABLE WHERE F_ID = ? AND F_RECIPE = ?;");
        _deleteRecipeData.prepare("DELETE FROM RECIPE_TABLE WHERE F_ID = ? AND F_RECIPE = ?;");
        _deleteAllRecipeData.prepare("DELETE FROM RECIPE_TABLE WHERE F_ID = ?;");
        //
        _addNotes.prepare("INSERT  OR REPLACE INTO NOTES_TABLE (F_ID,F_DATA) VALUES (?,?);");
        _getNotes.prepare("SELECT F_DATA FROM NOTES_TABLE WHERE F_ID = ?;");
        _deleteNotes.prepare("DELETE FROM NOTES_TABLE WHERE F_ID = ?;");
        //
        _updateAlias.prepare("UPDATE ALIAS_TABLE SET F_DATA=? WHERE F_ID=?;");
        _addAlias.prepare("INSERT OR REPLACE INTO ALIAS_TABLE (F_ID,F_DATA) VALUES (?,?);");
        _getAlias.prepare("SELECT F_DATA FROM ALIAS_TABLE WHERE F_ID = ?;");
        _deleteAlias.prepare("DELETE FROM ALIAS_TABLE WHERE F_ID = ?;");
        _listAlias.prepare("SELECT F_ID,F_DATA FROM ALIAS_TABLE;");
        //
        return true;
    }
    return false;
}

/*!
 * \brief MRL::Database::resetDatabase
 */
void MRL::Database::resetDatabase()
{
     db().Execute("DELETE FROM DATA_TABLE;");
     db().Execute("DELETE FROM NOTES_TABLE;");
     db().Execute("DELETE FROM RECIPE_TABLE;");
     db().Execute("DELETE FROM ALIAS_TABLE;");

}
// Database access
/*!
    \brief MRL::Database::addData
    \param id
    \param data
    \return
*/
bool MRL::Database::addData(unsigned id, unsigned type, const std::string &data) {
    _addData.reset();
    _addData.clear();
    _addData.bindInt(1, int(id));
    _addData.bindUnsigned(2, type);
    _addData.bindString(3, data);
    return _addData.exec();
}

/*!
    \brief MRL::Database::getData
    \param id
    \param data
    \return
*/
bool MRL::Database::getData(unsigned id, std::string &data) {
    _getData.reset();
    _getData.clear();
    _getData.bindInt(1, int(id));
    if (_getData.next()) {
        if (_getData.columnCount() > 0) {
            data = _getData.data(0);
            return true;
        }
    }
    return false;
}

/*!
    \brief MRL::Database::deleteData
    \param id
    \return
*/
bool MRL::Database::deleteData(unsigned id) {
    _deleteData.reset();
    _deleteData.clear();
    _deleteData.bindInt(1, int(id));
    return _deleteData.exec();
}


// Database access
/*!
    \brief MRL::Database::addData
    \param id
    \param data
    \return
*/
bool MRL::Database::addAlias(const std::string &id, const std::string &data) {
    _addAlias.reset();
    _addAlias.clear();
    _addAlias.bindString(1, id);
    _addAlias.bindString(2, data);
    return _addAlias.exec();
}

/*!
    \brief MRL::Database::getData
    \param id
    \param data
    \return
*/
bool MRL::Database::getAlias(const std::string &id, std::string &data) {
    _getAlias.reset();
    _getAlias.clear();
    _getAlias.bindString(1, id);
    if (_getAlias.next()) {
        if (_getAlias.columnCount() > 0) {
            data = _getAlias.data(0);
            return true;
        }
    }
    return false;
}

/*!
    \brief MRL::Database::deleteData
    \param id
    \return
*/
bool MRL::Database::deleteAlias(const std::string &id) {
    _deleteAlias.reset();
    _deleteAlias.clear();
    _deleteAlias.bindString(1, id);
    return _deleteAlias.exec();
}

/*!
 * \brief MRL::Database::updateAlias
 * \param id
 * \param data
 * \return
 */
bool MRL::Database::updateAlias(const std::string &id, const std::string &data)
{
    _updateAlias.reset();
    _updateAlias.clear();
    _updateAlias.bindString(1, data);
    _updateAlias.bindString(2, id);
    return _updateData.exec();
}

/*!
 * \brief MRL::Database::listAlias
 * \param l
 * \return
 */
bool MRL::Database::listAlias(StringMap &l, StringMap &r)
{
    l.clear();
    r.clear();
    _listAlias.reset();
    _listAlias.clear();
    while(_listAlias.next())
    {
        if (_listAlias.columnCount() > 1) {
            std::string s =  _listAlias.data(0);
            std::string d = _listAlias.data(1);
            l[s] = d; //
            r[d] = s; // reverse look up
        }
    }
    return true;
}



/*!
    \brief MRL::Database::addRecipeData
    \param id
    \param r
    \param data
    \return
*/
bool MRL::Database::addRecipeData(unsigned id, const std::string &r,  const std::string &data) {
    _addRecipeData.reset();
    _addRecipeData.clear();
    _addRecipeData.bindInt(1, int(id));
    _addRecipeData.bindString(2, r);
    _addRecipeData.bindString(3, data);
    return _addRecipeData.exec();

}

/*!
    \brief MRL::Database::getRecipeData
    \param id
    \param r
    \param data
    \return
*/
bool MRL::Database::getRecipeData(unsigned id, const std::string &r,  std::string &data) {
    _getRecipeData.reset();
    _getRecipeData.clear();
    _getRecipeData.bindInt(1, int(id));
    _getRecipeData.bindString(2, r);
    if (_getRecipeData.next()) {
        if (_getRecipeData.columnCount() > 0) {
            data = _getRecipeData.data(0);
            return true;
        }
    }
    return false;

}

/*!
    \brief MRL::Database::deleteRecipeData
    \param id
    \param r
    \return
*/
bool MRL::Database::deleteRecipeData(unsigned id, const std::string &r) {
    _deleteRecipeData.reset();
    _deleteRecipeData.clear();
    _deleteRecipeData.bindInt(1, int(id));
    _deleteRecipeData.bindString(2, r);
    return _deleteRecipeData.exec();

}

/*!
    \brief MRL::Database::deleteAllRecipeData
    \param id
    \return
*/
bool MRL::Database::deleteAllRecipeData(unsigned id) {
    _deleteAllRecipeData.reset();
    _deleteAllRecipeData.clear();
    _deleteAllRecipeData.bindInt(1, int(id));
    return _deleteAllRecipeData.exec();
}

//
/*!
    \brief MRL::Database::addNotes
    \param id
    \param data
    \return
*/
bool MRL::Database::addNotes(unsigned id, const std::string &data) {
    _addNotes.reset();
    _addNotes.clear();
    _addNotes.bindInt(1, int(id));
    _addNotes.bindString(2, data);
    return _addNotes.exec();

}
/*!
    \brief MRL::Database::getNotes
    \param id
    \param data
    \return
*/
bool MRL::Database::getNotes(unsigned id, std::string &data) {
    _getNotes.reset();
    _getNotes.clear();
    _getNotes.bindInt(1, int(id));
    if (_getNotes.next()) {
        if (_getNotes.columnCount() > 0) {
            data = _getNotes.data(0);
            return true;
        }
    }
    return false;
}

/*!
    \brief MRL::Database::deleteNotes
    \param id
    \return
*/
bool MRL::Database::deleteNotes(unsigned id) {
    _deleteNotes.reset();
    _deleteNotes.clear();
    _deleteNotes.bindInt(1, int(id));
    return _deleteNotes.exec();
}
/*!
    \brief MRL::Database::load
    \return true on success
*/
bool MRL::Database::load() { // load a configuration file
    // load a tree
    Json::Value v;
    //
    // Open file
    // save Json as string
    std::ifstream is;
    std::string f = dir() + "/" + file() + ".cfg";
    is.open(f);
    if (is) {
        is >> v;
        fromJson(v);
        tree().setChanged(false);
        return true;
    }

    return false;
}

/*!
    \brief MRL::Database::save
    \return true on succes
*/
bool MRL::Database::save() { // load a configuration file
    Json::Value v;
    toJson(v);
    std::string f = dir() + "/" + file() + ".cfg";
    std::ofstream os;
    os.open(f);
    if (os) {
        os << v;
        tree().setChanged(false);
        return true;
    }
    return false;
}

//
// tree based access
//
/*!
    \brief MRL::Database::find
    \param path
    \return object id
*/
unsigned MRL::Database::find(const PropertyPath &path) {
    if (tree().exists(path)) {
        DatabaseNode &d = tree().get(path);
        return d._id;
    }
    return 0;
}

/*!
    \brief MRL::Database::typeId
    \param path
    \return object type id
*/
unsigned MRL::Database::typeId(const PropertyPath &path) {
    if (tree().exists(path)) {
        DatabaseNode &d = tree().get(path);
        return d._type;
    }
    return 0;
}

/*!
    \brief MRL::Database::add
    \param path
    \param type
    \return  true on success
*/
bool MRL::Database::add(const PropertyPath &path, unsigned type) { // create an object in the tree
    if (!tree().exists(path)) {
        DatabaseNode d;
        d._id = _nextId++;
        d._type = type;
        idMap()[d._id] = tree().set(path, d);
        //
        ObjectManager *o = ObjectManager::find(type);
        if(o)
        {
            std::stringstream os;
            os << "{" << std::endl << "\t\"Name\" : \"" << path.back() << "\"," << std::endl << "\t\"Type\" : \"" << o->name() << "\"" << std::endl << "}";
            return (addData(d._id, type, os.str()) && addRecipeData(d._id, DEFAULT_RECIPE, os.str()) && addNotes(d._id, "Object Notes"));
        }
    }
    return false;
}


/*!
    \brief MRL::Database::updateData
    \param id
    \param data
    \return
*/
bool MRL::Database::updateData(unsigned id, const std::string &data) {
    _updateData.reset();
    _updateData.clear();
    _updateData.bindString(1, data);
    _updateData.bindInt(2, int(id));
    return _updateData.exec();
}

/*!
    \brief MRL::Database::updateData
    \param path
    \param data
    \return  true on success
*/
bool  MRL::Database::updateData(const PropertyPath &path, const std::string &data) {
    unsigned id = find(path);
    if (id > 0) {
        return updateData(id, data);
    }
    return false;
}

/*!
    \brief MRL::Database::updateData
    \param path
    \param data
    \return  true on success
*/
bool  MRL::Database::updateNotes(const PropertyPath &path, const std::string &data) {
    unsigned id = find(path);
    if (id > 0) {
        return addNotes(id, data);
    }
    return false;
}

/*!
    \brief MRL::Database::updateRecipe
    \param path
    \param recipe
    \param data
    \return  true on success
*/
bool  MRL::Database::updateRecipe(const PropertyPath &path, const std::string &recipe, const std::string &data) { // create an object in the tree
    unsigned id = find(path);
    if (id > 0) {
        return addRecipeData(id, recipe, data);
    }
    return false;
}

/*!
    \brief MRL::Database::getData
    \param path
    \param data
    \return  true on success
*/
bool MRL::Database::getData(const PropertyPath &path,  std::string &data) { // create an object in the tree
    unsigned id = find(path);
    if (id > 0) {
        return getData(id, data);
    }
    return false;
}

/*!
    \brief MRL::Database::getNotes
    \param path
    \param data
    \return  true on success
*/
bool MRL::Database::getNotes(const PropertyPath &path,  std::string &data) { // create an object in the tree
    unsigned id = find(path);
    if (id > 0) {
        return getNotes(id, data);
    }
    return false;
}

/*!
    \brief MRL::Database::getRecipe
    \param path
    \param recipe
    \param data
    \return  true on success
*/
bool MRL::Database::getRecipe(const PropertyPath &path, const std::string &recipe, std::string &data) { // create an object in the tree
    unsigned id = find(path);
    if (id > 0) {
        return getRecipeData(id, recipe, data);
    }
    return false;
}

/*!
    \brief MRL::Database::remove
    \param path
    \return  true on success
*/
bool MRL::Database::remove(const PropertyPath &path) {
    unsigned id = find(path);
    if (id > 0) {
        deleteData(id);
        deleteAllRecipeData(id);
        deleteNotes(id);
        idMap().erase(id);
        tree().remove(path);
        return true;
    }
    return false;
}

/*!
    \brief MRL::Database::removeRecipe
    \param path
    \param r
    \return  true on success
*/
bool MRL::Database::removeRecipe(const PropertyPath &path, const std::string &r) {
    unsigned id = find(path);
    if (id > 0) {
        return deleteRecipeData(id, r);
    }
    return false;
}

