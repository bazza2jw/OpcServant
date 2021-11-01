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
#ifndef DISPLAYPROPERTYTREE_H
#define DISPLAYPROPERTYTREE_H
#include "datapropertyitem.h"
#include "database.h"
#include "Web/webdatamodel.h"


namespace MRL {


    typedef PropertyTree<std::string, DataPropertyItem > DATAITEMTREE;
    /*!
        \brief The DataItemTree class
    */
    class DataItemTree  {
            //
            DataPropertyItem _root; // the root of the trees
            #ifdef USE_GUI
            wxDataViewTreeCtrl *_guiStore = nullptr; // the tree display that is the  model
            #endif
            std::shared_ptr<WebDataModel> _webStore; // the web store
            DATAITEMTREE _tree;        // property tree to index models
            //
            bool _syncRequest = false;
            bool _syncWithDbRequest = false;
            //

        public:
            /*!
                \brief DataItemTree
            */
            DataItemTree() {

            }
            /*!
                \brief webStore
                \return
            */
            std::shared_ptr<WebDataModel> &webStore() {
                if (!_webStore) {
                    _webStore = std::make_shared<WebDataModel>();
                    _root.setItem(_webStore->invisibleRootItem());
                    _tree.root().data().setItem(_webStore->invisibleRootItem());
                }
                return _webStore;
            }
            #ifdef USE_GUI

            /*!
                \brief setStore
                \param s
            */
            void setStore(wxDataViewTreeCtrl *s);
            #endif

            // sync requests
            bool syncRequest() const { return _syncRequest;}

            bool syncWithDbRequest() const { return _syncWithDbRequest;}


            /*!
                \brief root
                \return
            */
            DataPropertyItem &root() {
                return _root;
            }
            #ifdef USE_GUI

            /*!
                \brief store
                \return
            */
            const wxDataViewTreeCtrl *guiStore() const {
                return _guiStore;
            }
            #endif
            /*!
                \brief syncWithDatabase - only called on start up before web is running
                \param d
            */
            void syncWithDatabase(Database &d) {
                clearAll();
                syncWithDatabase(d.tree().rootNode(), _tree.rootNode());
            }

            /*!
             * \brief clearAll
             */
            void clearAll()
            {
                deleteAll();
                resetGui();
                resetWeb();
            }
            /*!
                \brief sync
            */
            void sync() {
                clearAll();
                syncNode(_tree.rootNode());
            }
            /*!
                \brief add
                \param path
                \param type
            */
            void add(const PropertyPath &path, unsigned type) {
                DataPropertyItem i;
                i.setType(type);
                auto n = _tree.set(path, i);
                add(n);
            }
            /*!
                \brief remove
                \param path
            */
            void remove(const PropertyPath &path) {
                auto n = _tree.node(path);
                if (n) {
                    remove(n);
                }
            }
            /*!
                \brief update
                \param path
            */
            void update(const PropertyPath &path) {
                auto n = _tree.node(path);
                if (n) {
                    update(n);
                }
            }
            #ifdef USE_GUI
            /*!
                \brief findDataItemPath
                \param i
                \param path
                \return
            */
            bool findDataItemPath(const wxDataViewItem &i, MRL::PropertyPath &path);
            #endif
            /*!
                \brief findDataWebItemPath
                \param i
                \param path
                \return
            */
            bool findDataItemPath(const Wt::WStandardItem *i, MRL::PropertyPath &path);

            //
            /*!
                \brief deleteAll
            */
            void deleteAll();
            //
            /*!
                \brief processGui
            */
            void processGui();
            /*!
                \brief processWeb
            */
            void processWeb();
            /*!
                \brief processDelete
            */
            void processDelete();

        protected:

            /*!
                \brief syncNode
                \param n
            */
            void syncNode(DATAITEMTREE::PropertyNode *n) {
                if (n) {
                    add(n);
                    for (auto i = n->children().begin(); i != n->children().end(); i++) {
                        syncNode(i->second);
                    }
                }
            }

            //
            // MRL Database interface
            /*!
                \brief syncWithDatabase
                \param src
                \param dest
            */
            void syncWithDatabase(DataTree::PropertyNode *src, DATAITEMTREE::PropertyNode *dest);

            /*!
                \brief syncRemoveMissing
            */
            void syncRemoveMissing(DataTree::PropertyNode *src, DATAITEMTREE::PropertyNode *dest);

            /*!
                \brief syncAddNew
                \param src
                \param dest
            */
            void syncAddNew(DataTree::PropertyNode *src, DATAITEMTREE::PropertyNode *dest);

            //
            // Model operations
            //
            void add(DATAITEMTREE::PropertyNode *n);
            void remove(DATAITEMTREE::PropertyNode *n);
            void update(DATAITEMTREE::PropertyNode *n);
            #ifdef USE_GUI
            void addGui(DATAITEMTREE::PropertyNode *n);
            void removeGui(DATAITEMTREE::PropertyNode *n);
            void updateGui(DATAITEMTREE::PropertyNode *n);
            #endif
            void addWeb(DATAITEMTREE::PropertyNode *n);
            void removeWeb(DATAITEMTREE::PropertyNode *n);
            void updateWeb(DATAITEMTREE::PropertyNode *n);
            //
            void resetGui() {
                #ifdef USE_GUI
                if (_guiStore) {
                    _guiStore->DeleteAllItems();
                    _root.guiItem() = _guiStore->AppendContainer(wxDataViewItem(), "Objects");
                    _tree.root().data().setItem(_root.guiItem());
                }
                #endif
            }
            void resetWeb() {
                if (_webStore) {
                    const_cast<Wt::WStandardItem *>(_root.webItem())->removeRows(0, _root.webItem()->rowCount());
                    _root.setItem(_webStore->invisibleRootItem());
                    _tree.root().data().setItem(_webStore->invisibleRootItem());
                }
            }

    };

}
#endif // DISPLAYPROPERTYTREE_H
