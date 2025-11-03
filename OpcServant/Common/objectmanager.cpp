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
#include "objectmanager.h"
#include <MrlLib/scrypt.h>
#include <Wt/WObject.h>
#include <Wt/WPopupMenu.h>
#include "displaypropertytree.h"
#include <Common/Web/webaddobjectdialog.h>
#include <Common/Daq/daqcommon.h>
#ifdef USE_GUI
#include <Common/Gui/AddObjectDialog.h>
#include <Common/Gui/SetupDialog.h>
#include <Common/Gui/GridPanel.h>
#include <Common/Gui/BourbonCommonUI.h>
#endif
//
#include <Wt/WTabWidget.h>
#include <Common/Web/webpanel.h>
#include <Common/Web/configurationwebdialog.h>
//

static CommonImageList *commonImages;

MRL::ObjectManager::MAP  MRL::ObjectManager::_map; // the lookup map
MRL::ObjectManager::IDMAP  MRL::ObjectManager::_idmap; // the lookup map
#ifdef USE_GUI
wxImageList MRL::ObjectManager::_imageList(32,32,true); //!< image list for tree items
#endif

/*!
    \brief MRL::ObjectManager::ObjectManager
    \param n
    \param ui
*/
MRL::ObjectManager::ObjectManager(const std::string &n) : _name(n) {
    std::unique_ptr<ObjectManager> ptr(this); // add to map - self register
    _map.insert(std::make_pair(n, std::move(ptr)));
    _type = fast_crc32(n[0], (unsigned char *)n.c_str(), n.size()); // set the type id
    _idmap[_type] = this; // map id to type
    _webIconName =    std::string(OPCSERVANT_WT_RESOURCE_DIR "objectIcons/") + _name + ".png";
    //
    #ifdef USE_GUI
    _iconFileName =   Common::baseDir() + "/icons/" + _name + ".png";
    _icon = wxIcon(_iconFileName, wxBITMAP_TYPE_PNG);
    if(_icon.IsOk())
    {
        _iconIndex = imageList().Add(_icon);
    }
    else
    {
        _iconIndex = imageList().Add(commonImages->Bitmap("m_Default"));
    }
    #endif
    //
}

/*!
 * \brief MRL::ObjectManager::initialise
 */
void MRL::ObjectManager::initialise()
{
    commonImages = new CommonImageList();
}
/*!
    \brief MRL::ObjectManager::create
    \param t
    \param path
    \return true on success
*/
bool MRL::ObjectManager::create(const PropertyPath &path) {
    if (Common::configuration().add(path, type())) {
        Common::display().add(path, type());
        createRT(path); // create in run time space
    }
    return true;
}

/*!
    \brief MRL::ObjectManager::remove
    \param t
    \param s
    \return true on success
*/
bool MRL::ObjectManager::remove(const PropertyPath &s) {
    removeRT(s);
    Common::display().remove(s);
    Common::configuration().remove(s);
    MRL::Common::configuration().saveToDb();
    return true;
}


/*!
    \brief MRL::ObjectManager::removeRT
    \param t
    \param s
    \return true on success
*/
bool MRL::ObjectManager::removeRT(const PropertyPath &s) {
    Common::runTime().remove(s);
    return true;
}

/*!
    \brief MRL::ObjectManager::create
    build the object in runtime space
    \param t
    \param path
    \return true on success
*/
bool MRL::ObjectManager::createRT(const PropertyPath &path) {
    Common::runTime().set(path, type());
    return true;
}

/*!
 * \brief MRL::ObjectManager::edit
 * \return true on success
 */
bool MRL::ObjectManager::edit(Wt::WWidget *w, const PropertyPath &s) {
    WebDialogBase::showDialog<ConfigurationWebForm>(w,s); // web based edit
    return true;
}


/*!
    \brief MRL::ObjectManager::update
    \param s
    \return true on success
*/
bool MRL::ObjectManager::update(const PropertyPath &s) {
    Common::display().update(s); // update the displays
    return true;
}
#ifdef USE_GUI
/*!
    \brief MRL::ObjectManager::buildMenu
    \param m
    \param s
    \return  true on success
*/
bool MRL::ObjectManager::buildMenu(wxMenu *m,  const PropertyPath &/*s*/) {
    if (canAdd()) m->Append(ObjectManagerAdd, _("Add"), _("Add child object to this item"));
    if (canDelete()) m->Append(ObjectManagerDelete, _("Delete"), _("Delete this item"));
    if (canEdit()) m->Append(ObjectManagerProperties, _("Properties"), _("Edit this item's configuration"));
    if (canView()) m->Append(ObjectManagerView, _("View"), _("View this item"));
    return (m->GetMenuItemCount() > 0);
}
#endif
/*!
    \brief MRL::ObjectManager::buildMenu
    \param m
    \param s
    \return true on success
*/
bool MRL::ObjectManager::buildMenu(Wt::WPopupMenu *m,  const PropertyPath &/*s*/) {
    if (canAdd()) {
        Wt::WMenuItem *i = m->addItem(std::make_unique<Wt::WMenuItem>(_TR("Add")));
        i->setData((void *)ObjectManagerAdd);
    }

    if (canDelete()) {
        Wt::WMenuItem *i = m->addItem(std::make_unique<Wt::WMenuItem>(_TR("Delete")));
        i->setData((void *)ObjectManagerDelete);
    }

    if (canEdit()) {
        Wt::WMenuItem *i = m->addItem(std::make_unique<Wt::WMenuItem>(_TR("Properties")));
        i->setData((void *)ObjectManagerProperties);
    }

    if (canView()) {
        Wt::WMenuItem *i = m->addItem(std::make_unique<Wt::WMenuItem>(_TR("View")));
        i->setData((void *)ObjectManagerView);
    }
    return true;
}

#ifdef USE_GUI
/*!
    \brief add
    Add from GUI
    \param w
    \return true on success
*/
bool MRL::ObjectManager::add(wxWindow *w, const PropertyPath &path, PropertyPath &newObjectPath) {
    AddObjectDialog dlg(w, path);
    if (dlg.ShowModal() == wxID_OK) {
        PropertyPath p = path;
        p.push_back(dlg.GetObjectName()->GetValue().ToStdString());
        ObjectManager *o = ObjectManager::find(dlg.GetType()->GetString(dlg.GetType()->GetSelection()).ToStdString());
        if (o) {
            if (o->create(p)) {
                MRL::Common::configuration().saveToDb();
                newObjectPath = p;
                return true;
            }
        }
    }
    return false;
}

/*!
 * \brief MRL::ObjectManager::edit
 * \param w
 * \param s
 * \return true on success
 */
bool MRL::ObjectManager::edit(wxWindow *w, const PropertyPath &s) {
    SetupDialog dlg(w,s);
    return dlg.ShowModal() == wxID_OK;
}

#endif
/*!
    \brief add
    Add from web interface
    \param w
    \return  true on success
*/
bool MRL::ObjectManager::add(Wt::WWidget *w, const PropertyPath &path) {
    WebDialogBase::showDialog<WebAddObjectForm>(w, path);
    return false;
}



/*!
    \brief MRL::ObjectManager::setupOPCInstance
    \param server
    \param id
    \param node
    \return true on success
*/
bool MRL::ObjectManager::setupOPCInstance(OpcServer &server, unsigned id, Open62541::NodeId &node) {
    DataNode *d = Common::configuration().idMap()[id];
    if (d) {
        RtObjectRef &r = Common::daq().objects()[id]; // get the current config and runtime data
        if (r) {
            r->initialiseOpc(server, node);
            return true;
        }
    }
    return false;
}


/*!
    \brief writeConfigurationPage
    \param id
*/
void MRL::ObjectManager::writeConfigurationPage(unsigned id, CTML::Node &s) { // format and write the configuration as HTML
    // Get the configuration - fairly crude default that dumps the configuration tree
    Json::Value v;
    if (Common::configuration().getData(id, v)) {
        // convert to tree
        VariantPropertyTree t;
        t.fromJson(v);
        t.setInt("ID",id);

        auto f = [&s](VariantPropertyTree::ValueNode *in, PropertyPath &p){
            CTML::Node n("p");
            std::string txt = std::string(p.size(),'>') + in->name() + " = " + toString(in->data());
            s.AppendChild(n.SetContent(txt));
        };
        // walk tree and add to document
        t.iterateWithPath(f);
    }
    else {
        CTML::Node n("p");
        s.AppendChild(n.SetContent("Configuration Not Found"));
    }
}

/*!
    \brief writeConfigurationPage
    \param id
    \param s
*/
void MRL::ObjectManager::writeConfigurationPage(unsigned id, std::string &s) { // format and write the configuration as HTML
    CTML::Node d("html");
    writeConfigurationPage(id, d);
    s = d.ToString(CTML::Readability::MULTILINE);
}



/*!
* \brief createTabWindow
* \param id
* \return  window for tab - similar to UI but without navigator
*/
wxWindow * MRL::ObjectManager::createTabWindow(wxWindow *parent, unsigned id)
{
    return new GridPanel(parent, id);
}


/*!
 * \brief MRL::ObjectManager::createTabWindow
 * \param tab
 * \param id
 */
void MRL::ObjectManager::createTabWindow(Wt::WTabWidget * tab, unsigned id)
{
    MRL::RtObjectRef &r =  MRL::Common::daq().objects()[id];
    if (r) {
        tab->addTab(std::make_unique<MRL::WebPanel>(id),r->path().back(), Wt::ContentLoading::Eager);
    }
}

/*!
 * \brief MRL::ObjectManager::createWebWindow
 * \param root
 * \param id
 * \return true on success
 */
Wt::WContainerWidget * MRL::ObjectManager::createWebWindow( Wt::WContainerWidget *root, unsigned id)
{
    return root->addWidget(std::make_unique<MRL::WebPanel>(id));
}




