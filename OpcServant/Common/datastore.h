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
#ifndef DATASTORE_H
#define DATASTORE_H
#include <MrlLib/mrllib.h>
#include <Common/database.h>
#include <Wt/WObject.h>
namespace MRL
{
/*!
 * \brief The Datastore class
 * This stores runtime data and is indexed by a property tree
 */
template <typename S>  class Datastore
    {
        typedef unsigned  DatastoreIndex;
        typedef std::map<DatastoreIndex, S> DataStoreAnyMap;
        ReadWriteMutex _mutex;
        DatastoreIndex _nextId = 100; //!< next id for item added to tree
        DataTree _tree; //!< index property tree
        DataStoreAnyMap _store; //!< data store
        S _emptyS; // an empty value
    public:
        Datastore() = default;
        virtual ~Datastore() {}
        DataTree & tree() { return _tree;}
        DataStoreAnyMap & store() { return  _store;}
        //
        // Data store access
        //
        /*!
         * \brief addData
         * \param id
         * \param data
         * \return true on success
         */
        bool addData(DatastoreIndex id, const S & data)
        {
            _store[id] = data;
            return true;
        }

        /*!
         * \brief getData
         * \param id
         * \return true on success
         */
        S & getData(DatastoreIndex id)
        {
            auto i = _store.find(id);
            if(i != _store.end()) return i->second;
            return _emptyS;
        }

        /*!
         * \brief deleteData
         * \param id
         * \return true on success
         */
        bool deleteData(DatastoreIndex id)
        {
            _store.erase(id);
            return true;
        }

        /*!
         * \brief updateData
         * \param path
         * \param data
         * \return true on success
         */
        bool updateData(const PropertyPath &path, const S &data) // create an object in the tree
        {
            DatastoreIndex i = find(path);
            if(i > 0)
            {
                _store[i] = data;
                return true;
            }
            return false;
        }

        /*!
         * \brief deleteData
         * \param path
         * \return true on success
         */
        bool deleteData(const PropertyPath &path)
        {
            DatastoreIndex i = find(path);
            if(i > 0)
            {
                deleteData(i);
                tree().remove(path);
                return true;
            }
            return false;
        }

        /*!
         * \brief clear
         */
        void clear()
        {
            tree().clear();
            store().clear();
        }
        //
        // tree based access
        //
        /*!
            \brief MRL::Datastore::find
            \param path
            \return object id
        */
        DatastoreIndex find(const  PropertyPath &path) {
            if (tree().exists(path)) {
                DatabaseNode &d = tree().get(path);
                return d._id;
            }
            return 0;
        }

        /*!
            \brief MRL::Datastore::typeId
            \param path
            \return object type id
        */
        int typeId(const PropertyPath &path) {
            if (tree().exists(path)) {
                DatabaseNode &d = tree().get(path);
                return d._type;
            }
            return 0;
        }

        /*!
            \brief MRL::Datastore::add
            \param path
            \param type
            \return  true on success
        */
        bool add(const PropertyPath &path, unsigned type) { // create an object in the tree
            if (!tree().exists(path)) {
                DatabaseNode d;
                d._id = _nextId++;
                d._type = type;
                tree().set(path, d);
                return addData(d._id, "");
            }
            return false;
        }

        /*!
         * \brief MRL::Datastore::syncFrom
         * \param d
         */
        void syncFrom(Database &d)
        {
            // create tree from data store structure
            clear();
            auto f = [&](DataNode *p, PropertyPath &dp)
            {
                add(dp,p->data()._type);
            };
            d.tree().iterateWithPath(f);
        }
    };
}
#endif // DATASTORE_H
