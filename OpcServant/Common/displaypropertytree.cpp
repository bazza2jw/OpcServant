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
#include "displaypropertytree.h"
#include "common.h"
#include "objectmanager.h"
#ifdef USE_GUI
/*!
    \brief setStore
    \param s
*/
void MRL::DataItemTree::setStore(wxDataViewTreeCtrl *s) {
    _guiStore = s;
    if (s) {
        _guiStore->SetImageList(&ObjectManager::imageList());
        _root.guiItem() = _guiStore->AppendContainer(wxDataViewItem(), "Objects");
        _tree.root().data().setItem(_root.guiItem());
    }
}
#endif
/*!
    \brief add
    \param n
*/
void MRL::DataItemTree::add(DATAITEMTREE::PropertyNode *n) {
    if (n) n->data().setOp(DataPropertyItem::Add);
}

/*!
    \brief remove
    \param n
*/
void MRL::DataItemTree::remove(DATAITEMTREE::PropertyNode *n) {
    if (n) n->data().setOp(DataPropertyItem::Remove);

}

/*!
    \brief update
    \param n
*/
void MRL::DataItemTree::update(DATAITEMTREE::PropertyNode *n) {
    if (n) n->data().setOp(DataPropertyItem::Update);
}
/*!
    \brief MRL::DataItemTree::deleteAll
    Also reset the root items
*/
void  MRL::DataItemTree::deleteAll() {
    _tree.root().clear();

}

void MRL::DataItemTree::processGui() {
    auto f = [this](DATAITEMTREE::PropertyNode & n) -> bool {
        if (n.data().waitGui()) {
            switch (n.data().op()) {
                case DataPropertyItem::Add: // create the Web/GUI items in the models
                    addGui(&n);
                    n.data().clearWaitGui();
                    break;
                case DataPropertyItem::Remove:  // remove the items from the models
                    removeGui(&n);
                    n.data().clearWaitGui();
                    if (!n.data().waitWeb()) n.data().setOp(DataPropertyItem::Delete);
                    break;
                case DataPropertyItem::Update: // trigger an update
                    updateGui(&n);
                    n.data().clearWaitGui();
                    break;
                case DataPropertyItem::Reset: {
                resetGui();
                }
                break;
                default:
                    break;
            }
        }
        return true;
    };
    //
    _tree.root().iterateNodes(f); // clear data in tree
    processDelete();
}


void MRL::DataItemTree::processWeb() {
    auto f = [this](DATAITEMTREE::PropertyNode & n) -> bool {
        if (n.data().waitWeb()) {
            switch (n.data().op()) {
                case DataPropertyItem::Add: // create the Web/GUI items in the models
                    addWeb(&n);
                    n.data().clearWaitWeb();
                    break;
                case DataPropertyItem::Remove:  // remove the items from the models
                    removeWeb(&n);
                    n.data().clearWaitWeb();
                    if (!n.data().waitGui()) n.data().setOp(DataPropertyItem::Delete);
                    break;
                case DataPropertyItem::Update: // trigger an update
                    updateWeb(&n);
                    n.data().clearWaitWeb();
                    break;
                case DataPropertyItem::Reset: {
                    resetWeb();
                }
                break;

                default:
                    break;
            }
        }
        return true;
    };
    //
    _tree.root().iterateNodes(f); // action in web context
    processDelete(); // garbage collect
}

void MRL::DataItemTree::processDelete() {
    auto f = [](DATAITEMTREE::PropertyNode & n) -> bool {
        if (n.data().waitGui()) {
            switch (n.data().op()) {
                case DataPropertyItem::Delete: // delete garbage nodes
                    if (n.parent()) {
                        n.parent()->removeChild(n.name());
                    }
                    break;
                default:
                    break;
            }
        }
        return true;
    };
    //
    _tree.root().iterateNodes(f); // clear data in tree
}


/*!
    \brief MRL::DataItemTree::addWeb
    \param n
*/
void MRL::DataItemTree::addWeb(DATAITEMTREE::PropertyNode *n) {   //
    if (_webStore && n && (n->data().webItem() == nullptr)) {
        // get the parent
        auto p = n->parent();
        if (p) {
            const Wt::WStandardItem *r = p->data().webItem(); // get the parent item
            if (r) {
                // append to container
                ObjectManager *o = ObjectManager::find(n->data().type());
                if (o) {
                    // append to container
                    Wt::WStandardItem *j = const_cast<Wt::WStandardItem *>(r);
                    int k = j->rowCount();
                    j->appendRow(std::make_unique< Wt::WStandardItem>(n->name()));
                    Wt::WStandardItem *i = j->child(k);
                    n->data().setItem(i);
                    i->setIcon(o->webIconName()); // set the icon TO DO
                    i->setColumnCount(WebDataModel::NumberColumns);
                }
            }
        }
    }
}
#ifdef USE_GUI

/*!
    \brief MRL::DataItemTree::addGui
    \param n
*/
void MRL::DataItemTree::addGui(DATAITEMTREE::PropertyNode *n) {
    // do not add item if dataviewitem is already there
    if (_guiStore && n && (!n->data().guiItem().IsOk())) {
        ObjectManager *oi = ObjectManager::find(n->data().type());
        if (oi) {
            if (oi->canAdd()) { // do we want a container or terminal leaf
                wxDataViewItem i = _guiStore->AppendContainer(n->parent()->data().guiItem(), n->name(), oi->iconIndex());
                n->data().setItem(i);
            }
            else {
                wxDataViewItem j = _guiStore->AppendItem(n->parent()->data().guiItem(), n->name(), oi->iconIndex());
                n->data().setItem(j);
            }
        }
    }
}

/*!
    \brief MRL::DisplayPropertyTree::remove
*/
void MRL::DataItemTree::removeGui(DATAITEMTREE::PropertyNode *n) {                    //
    if (_guiStore && n) {
        //
        if (n->data().guiItem().IsOk()) {
            _guiStore->DeleteChildren(n->data().guiItem()); // delete from store
            _guiStore->DeleteItem(n->data().guiItem());
        }
        auto p = n->parent();
        if (p) {
            _guiStore->GetStore()->ItemChanged(p->data().guiItem()); // notify changed
        }
        n->data().setItem(wxDataViewItem());
    }
}



/*!
    \brief MRL::DataItemTree::updateGui
    \param n
*/
void MRL::DataItemTree::updateGui(DATAITEMTREE::PropertyNode *n) {
    // update the displayed tree item to match to match the item status and value
    if (_guiStore && n) {
        // set the value
        // set the colour and icons according to state
        _guiStore->GetStore()->ItemChanged(n->data().guiItem()); // notify changed
    }
}

#endif
/*!
    \brief MRL::WebDisplayPropertyTree::remove
*/
void MRL::DataItemTree::removeWeb(DATAITEMTREE::PropertyNode *n) {                    //
    if (_webStore && n) {
        //
        if(n->data().webItem())
        {
            int row = n->data().webItem()->row();
            auto p = n->parent();
            if (p) {
                Wt::WStandardItem *i = const_cast<Wt::WStandardItem *>(p->data().webItem());
                if (i) i->removeRow(row);
            }
            n->data().setItem((Wt::WStandardItem *)(0));
        }
    }
}

/*!
    \brief MRL::DataItemTree::updateWeb
    \param n
*/
void MRL::DataItemTree::updateWeb(DATAITEMTREE::PropertyNode *n) {
    if (_webStore && n) {
        // set the value
        // set the colour and icons according to state
    }
}

/*!
    \brief MRL::DisplayPropertyTree::findDataItem
    \return get the path to a data item
*/
bool MRL::DataItemTree::findDataItemPath(const Wt::WStandardItem *i, MRL::PropertyPath &path) {
    // find a data item by path in the store
    path.clear();
    if (_webStore) {
        while ((i != _root.webItem()) && (path.size() < 8)) {
            path.push_front(i->text().toUTF8());
            i = i->parent();
        }

        if (path.size() > 6) {
            wxLogDebug("Error in path search");
        }
        return true;
    }
    return false;
}

#ifdef USE_GUI
/*!
    \brief MRL::DisplayPropertyTree::findDataItem
    \return get the path to a data item
*/
bool MRL::DataItemTree::findDataItemPath(const wxDataViewItem &i, MRL::PropertyPath &path) {
    // find a data item by path in the store
    path.clear();
    if (_guiStore) {
        wxDataViewItem j = i;
        while ((j != _root.guiItem()) && (path.size() < 8)) {
            path.push_front(_guiStore->GetItemText(j).ToStdString());
            j = _guiStore->GetStore()->GetParent(j);
        }

        if (path.size() > 6) {
            wxLogDebug("Error in path search");
        }
        return true;
    }
    return false;
}
#endif

/*!
    \brief syncWithDatabase
    \param src
    \param dest
*/
void MRL::DataItemTree::syncWithDatabase(DataTree::PropertyNode *src, DATAITEMTREE::PropertyNode *dest) {
    if (src && dest && (src->children().size() > 0)) {
        // make these nodes the same
        syncRemoveMissing(src, dest);
        syncAddNew(src, dest);
        // now iterate the child nodes
        for (auto i = src->children().begin(); i != src->children().end(); i++) {
            DATAITEMTREE::PropertyNode *n = dest->child(i->first);
            if (n) {
                add(n);
                syncWithDatabase(i->second, dest->child(i->first)); // recurse
            }
        }
    }
}

/*!
    \brief MRL::DisplayPropertyTree::syncRemoveMissing
*/
void MRL::DataItemTree::syncRemoveMissing(DataTree::PropertyNode *src, DATAITEMTREE::PropertyNode *dest) { // remove from display those that are missing
    std::list<std::string> l;
    for (auto i = dest->children().begin(); i != dest->children().end(); i++) {
        if (!src->hasChild(i->first)) {
            l.push_back(i->first);
        }
    }
    if (l.size() > 0) {
        for (auto j = l.begin(); j != l.end(); j++) {
            remove(dest->child(*j)); // delete items missing on child side
        }
    }
}

/*!
    \brief syncAddNew
    \param src
    \param dest
*/
void MRL::DataItemTree::syncAddNew(DataTree::PropertyNode *src, DATAITEMTREE::PropertyNode *dest) {
    // remove from display those that are missing
    std::list<std::string> l;

    //
    // Get the list of nodes present in src but absent in dest
    for (auto i = src->children().begin(); i != src->children().end(); i++) {
        if (!dest->hasChild(i->first)) {
            l.push_back(i->first);
        }
    }
    //
    // add the missing nodes
    //
    if (l.size() > 0) {
        for (auto j = l.begin(); j != l.end(); j++) {
            auto *c = (src->child(*j));
            if(c)
            {
                auto *n = new  DATAITEMTREE::PropertyNode(*j, dest);
                n->data().setType(c->data().type());
                dest->addChild(n);
            }
            else
            {
                wxLogDebug("NULL Source");
            }
        }
    }
}


