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
#ifndef DATASTORETREE_H
#define DATASTORETREE_H
#include "MrlLib/propertytree.h"
#include <boost/any.hpp>

/*
 * Each UI (+OPC) has its own data store either item model or OPC node store
 * DataStore is an abstracted interface to each interface
 */
namespace MRL {


    typedef unsigned long long DataStoreIndex; //!< index id to object


    /*!
           \brief The DataStoreBase class
           Base store class
    */
    class DataStoreBase {
        protected:
        public:
            DataStoreBase() {}
            virtual ~DataStoreBase() {}
            virtual void add(DataStoreIndex /*i*/) {} //!< add an empty item to the store
            virtual void remove(DataStoreIndex /*i*/) {}//!< remove an item from a store
            virtual void update(DataStoreIndex /*i*/, const boost::any &) {} //!< update one value
            virtual void sync() {} // ask for stores to sync their displays with tree
    };


    template <typename T>
    /*!
        \brief The DataStore class
    */
    class DataStore : public DataStoreBase {
            std::map<DataStoreIndex, T> _store; //!< Object store
        public:
            DataStore() {}
            /*!
                \brief add
                \param i
                \return index
            */
            void add(DataStoreIndex i) {
                _store[i] = T();
            }
            /*!
                \brief remove
                \param i
            */
            void remove(DataStoreIndex i) {
                _store.erase(i);
            }

            /*!
                \brief find
                \param i
                \return object
            */
            T & find(DataStoreIndex i) {
                if(_store.find(i) != _store.end())
                {
                    return _store[i];
                }
                return T();
            }

            /*!
                \brief update
                \param i
                \param o
            */
            void set(DataStoreIndex i, const T &o) {
                _store[i] = o;
            }

            /*!
                \brief update
                \param i
                \param a
            */
            void update(DataStoreIndex i, const boost::any &a) {
                set(i, boost::any_cast<T>(a));
            }

    };

    typedef PropertyTree<std::string, DataStoreIndex> DataStoreTree; //!< tree
    typedef PropertyTree<std::string, DataStoreIndex>::PropertyNode DataStoreNode; //!< tree node
    typedef std::shared_ptr<DataStoreBase> DataStoreBaseRef;

    typedef std::list<boost::any> DataStoreAnyList;
    /*!
        \brief The DataStoreTree class
        The tree is the index and manages the life cycle of a set of stores
    */

    class DataTreeStore {
            DataStoreIndex _next = 100;
            DataStoreTree _tree;     //!<  index tree
            std::map<int, DataStoreBase *> _stores; //!< set of stores to sync with tree
        public:
            /*!
                \brief DataStoreTree
            */
            DataTreeStore() {}

            /*!
            */
            ~DataTreeStore() {

            }

            /*!
                \brief tree
                \return reference to tree
            */
            DataStoreTree &tree() {
                return _tree;   //!<  index tree
            }
            /*!
                \brief store
                \param i
                \return store
            */
            DataStoreBase *store(int i) {
                if(_stores.find(i) == _stores.end()) return nullptr;
                return  _stores[i];
            }

            /*!
                \brief addStore
                \param i
                \param o
            */
            void addStore(int i, DataStoreBase *o) {
                _stores[i] = o;
            }

            //
            template <typename P>
            /*!
                \brief add
                \param path
                \param t
                \return
            */
            DataStoreIndex add(const P &path) {
                DataStoreIndex i = _next++;
                _tree.set(path, i);
                return i;
            }
            /*!
                \brief remove
                \param i
            */
            template <typename P>
            void remove(const P &path) {
                DataStoreIndex i = tree().get(path);
                tree().remove(path);
                for (auto j = _stores.begin(); j != _stores.end(); j++) {
                    DataStoreBase *b = (j->second);
                    b->remove(i);
                }
            }

            /*!
                \brief find
                \param i
                \return object
            */
            template <typename P>
            DataStoreIndex find(const P &path) {
                return tree().get(path);
            }


            /*!
             * \brief update Update one store
             * \param path
             * \param storeIndex
             * \param a
             */
            template <typename P>
            void update(const P &path, int storeIndex, const boost::any &a)
            {
                DataStoreIndex i = find(path);
                if(i > 0)
                {
                    DataStoreBase *b = store(storeIndex);
                    if(b)
                    {
                        b->update(i,a);
                    }
                }
            }

            /*!
             * \brief sync
             * ask all stores to sync with tree - this is a hint
             * One of the stores may have current config or runtime (active data)
             * The other stores can use the hint to update values accordingly
             */
            void sync()
            {
                for(auto i = _stores.begin(); i != _stores.end(); i++)
                {
                    (i->second)->sync();
                }
            }

    };
}
#endif // DATASTORETREE_H
