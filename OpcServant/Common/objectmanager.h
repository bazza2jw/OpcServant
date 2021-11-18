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
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include "database.h"
#include "datastore.h"
#include <Common/common.h>
#include <Common/Daq/daqcommon.h>
#include <Common/Opc/opccommon.h>
#include <MrlLib/CTML.h>
#include <Wt/WLayout.h>
#include <Wt/WTabWidget.h>
#ifdef USE_GUI
#include <Common/Gui/AddObjectDialog.h>
#include <Common/Gui/SetupDialog.h>
#include <Common/Gui/GridPanel.h>
#endif
//
#include <Wt/WTabWidget.h>
#include <Common/Web/webpanel.h>
#include <Common/Web/configurationwebdialog.h>
//
#ifdef USE_GUI
#include <wx/imaglist.h>
class wxWindow;
#endif

namespace Wt {
    class WObject;
}
namespace MRL {
    /*!
        \brief The ObjectManager class
        Object Factory and (life cycle) manager class
        For each type of object the manager class:
        Database interface to add,delete,update and creae
        The user interface to edit an object can be client GUI (direct)  or web or Client GUI (via OPC)
    */

    constexpr const char *OBJECTDATABASE = "OBJECTDATABASE.DB";
    constexpr const char *FOLDER_TYPE = "Folder";

    enum {
        ObjectManagerAdd = 2000,
        ObjectManagerDelete,
        ObjectManagerProperties,
        ObjectManagerView
    };

    class ObjectDataValidator;


    /*!
     * \brief The ObjectManager class
     */
    class ObjectManager {
            std::string _name;//!< name of object type
            typedef std::unique_ptr<ObjectManager> ObjectManagerRef;
            typedef std::map<std::string, ObjectManagerRef> MAP; // map of object managers
            typedef std::map<unsigned, ObjectManager *> IDMAP; // map type id to object manager

            static MAP _map;//!< lookup dictionary
            static IDMAP _idmap; //!< translate id to name
            unsigned _type; //!< type id
            std::string _webIconName; //!< name of icon resource
            //
            //
            // user interface objects
            #ifdef USE_GUI
            int _iconIndex = 0; //!< index into image list for GUI icon - needed for tree controls
            static wxImageList _imageList; //!< Image list
            std::string _iconFileName; //!< name of icon file
            wxIcon _icon;
            #endif


        public:
            /*!
                \brief ObjectManager
                \param name type name
            */
            ObjectManager(const std::string &n);
            /*!
                \brief ~ObjectManager
            */
            virtual ~ObjectManager() {
                // removal from map deletes - implicit deregister
                _idmap.erase(_type);
            }

            static void initialise();

            std::string &webIconName()  {
                return  _webIconName;
            }
            #ifdef USE_GUI
            /*!
                \brief iconIndex
                \return index into image array
            */
            int iconIndex() {
                return _iconIndex;
            }
            /*!
                \brief iconFileName
                \return name of icon file
            */
            const std::string &iconFileName() {
                return _iconFileName;
            }
            #endif
            /*!
                \brief clear
            */
            static void clear() {
                _map.clear();
            }

            /*!
                \brief name
                \return object manager
            */
            const std::string &name() {
                return _name;
            }
            #ifdef USE_GUI

            // list of images for decorating data tree views - the type id keys
            static wxImageList &imageList() {
                return _imageList;
            }
            #endif
            /*!
                \brief type
            */
            virtual unsigned type() {
                return _type; // this is a hash of the object type name
            }

            /*!
                \brief find
                \param n - look up name
                \return  pointer to object manager
            */
            static ObjectManager *find(const std::string &n) {
                if(_map.find(n) == _map.end()) return nullptr;
                return _map[n].get();
            }

            /*!
                \brief find
                \param i
                \return object manager or null
            */
            static ObjectManager *find(unsigned i) {
                if(_idmap.find(i) == _idmap.end()) return nullptr;
                return  _idmap[i];
            }
            /*!
             * \brief findByObjectId
             * \param i
             * \return object manager or null
             */
            static ObjectManager * findByObjectId(unsigned i) {
                unsigned id = Common::objectType(i);
                return find(id);
            }

            /*!
                \brief idToName
                \param i
                \return name
            */
            static const std::string &idToName(unsigned i) {
                static std::string ret;
                auto o = find(i);
                if (o) return o->name();
                return ret;
            }

            /*!
                \brief isTopLevel
                \return  true if top level item
            */
            virtual bool isTopLevel() {
                return false; // top level items are created on startup if necessary
            }

            /*!
                \brief canBeChildOf
                \param type
                \return true if parent is valid for child
            */
            virtual bool canBeChildOf(unsigned type) {
                return canBeChildOf(idToName(type));
            }

            /*!
                \brief canBeChildOf
                \param type
                \return true if allowed
            */
            virtual bool canBeChildOf(const std::string &type) {
                if (type == FOLDER_TYPE) { // can be a child of a folder
                    return true;
                }
                return false;
            }
            //
            // There are stores for configuration data and runtime data
            //
            // Runtime
            virtual bool removeRT(const PropertyPath &s);
            virtual bool createRT(const PropertyPath &path);
            // Configuration
            virtual bool create(const PropertyPath &path);
            virtual bool remove(const PropertyPath &s);
            virtual bool update(const PropertyPath &s);  // updated as an object so includes all children
            //
            // Variants for GUI and Web
            #ifdef USE_GUI
            virtual bool edit(wxWindow */*w*/, const PropertyPath &/*s*/);
            virtual bool view(wxWindow */*parent*/, const PropertyPath &/*s*/) {
                return false;
            }
            virtual bool buildMenu(wxMenu *m,  const PropertyPath &s);
            #endif
            virtual bool view(Wt::WObject */*parent*/, const PropertyPath &/*s*/) {
                return false;
            }
            virtual bool edit(Wt::WWidget */*w*/, const PropertyPath &/*s*/);
            virtual bool buildMenu(Wt::WPopupMenu *m,  const PropertyPath &s);

            // flags for operations
            virtual bool canEdit() {
                return true; //!< can the object be editted
            }
            virtual bool canDelete() {
                return true; //!< can it be deleted
            }
            virtual bool canView() {
                return true; //!< can it be viewed
            }

            virtual bool canAdd() {
                return false; //!< default to cannot be parent
            }

            virtual bool canRun() {
                return false; //!< is the object an active runtime object ?
            }
            #ifdef USE_GUI
            /*!
                \brief add
                Add from GUI
                \param w
                \return true on success
            */
            virtual bool add(wxWindow *w, const PropertyPath &parent, PropertyPath &newObjectPath);
            #endif
            /*!
                \brief add
                Add from web interface
                \param w
                \return  true on success
            */
            virtual bool add(Wt::WWidget *w, const PropertyPath &parent);

            /*!
                \brief listNames
                \param l
            */
            static void listNames(std::list<std::string> &l) {
                l.clear();
                for (auto i = _map.begin(); i != _map.end(); i++) {
                    l.push_back(i->first);
                }
            }
            //
            // Object creation
            virtual bool createBObject(unsigned, BObjectRef &)
            {
                return false;
            }

            virtual bool createBObject(const PropertyPath &p, BObjectRef &r)
            {
                unsigned i = Common::configuration().find(p);
                return createBObject(i,r);
            }


            // Runtime Object creation
            virtual bool createRtObject(unsigned, RtObjectRef &)
            {
                return false;
            }

            /*!
             * \brief createRtObject
             * \param p
             * \param r
             * \return true on success
             */
            virtual bool createRtObject(const PropertyPath &p, RtObjectRef &r)
            {
                unsigned i = Common::configuration().find(p);
                return createRtObject(i,r);
            }

            /*!
             * \brief setupOPCInstance
             * initalise OPC value nodes and map node contexts
             * \param server
             * \param id
             * \param node
             * \return true on success
             */
            virtual bool setupOPCInstance(OpcServer &server, unsigned id, Open62541::NodeId &node );

            /*!
             * \brief opcHistoryCount
             * \return  number of historianised nodes per instance
             */

            virtual size_t opcHistoryCount() const { return 0;}

            /*!
             * \brief writeConfigurationPage
             * \param id
             */
            virtual void writeConfigurationPage(unsigned id, CTML::Node &s ); // format and write the configuration as HTML

            /*!
             * \brief writeConfigurationPage
             * \param id
             * \param s
             */
            virtual void writeConfigurationPage(unsigned id, std::string &s ); // format and write the configuration as HTML

            /*!
             * \brief createTabWindow
             * \param parent
             * \param id
             * \return window object pointer
             */
            virtual wxWindow * createTabWindow(wxWindow *parent, unsigned id);
            /*!
             * \brief createTabWindow
             */
            virtual void createTabWindow(Wt::WTabWidget * /*tab*/,unsigned /*id*/);
            /*!
             * \brief createWebWindow
             * \return wt widget
             */
            virtual Wt::WContainerWidget * createWebWindow( Wt::WContainerWidget */*root*/, unsigned /*id*/);

            /*!
             * \brief createTopWindow
             * \return top level UI window
             */
            virtual wxFrame * createTopWindow(unsigned /*id*/)
            {
                return nullptr;
            }

    };


    template <typename R, typename S = SetupDialog, typename W = DefaultConfigurationWebDialog >
    /*!
     * \brief The ObjectManagerRT class
     */
    class ObjectManagerRT : public ObjectManager
    {
    public:
        ObjectManagerRT(const std::string &s) : ObjectManager(s) {}
        virtual bool canBeChildOf(const std::string &type) {
            return (type == "Equipment") || (type == "Folder");
        }
        virtual bool hasInputs() const { return true;}
        virtual bool canRun() {  return true; }

        bool edit(wxWindow *w, const PropertyPath &s) {
            S dlg(w,s);
            return dlg.ShowModal() == wxID_OK;
        }
        bool edit(Wt::WWidget *w, const PropertyPath &path) {
            WebDialogBase::showDialog<W>(w, path);
            return true;
        }

        bool createRtObject(unsigned id, RtObjectRef &r)
        {
           RtObjectRef o( new R(id));
           r = std::move(o); // assign across
           return true;
        }
    };

    template <typename R, typename S= SetupDialog, typename W = DefaultConfigurationWebDialog >
    /*!
     * \brief The ObjectManagerRT class
     */
    class ObjectManagerPublisher : public ObjectManager
    {
    public:
        ObjectManagerPublisher(const std::string &s) : ObjectManager(s) {}
        virtual bool canBeChildOf(const std::string &type) {
            return (type == "Publishers");
        }
        virtual bool hasInputs() const { return true;}
        virtual bool canRun() {  return true; }

        bool edit(wxWindow *w, const PropertyPath &s) {
            S dlg(w,s);
            return dlg.ShowModal() == wxID_OK;
        }
        bool edit(Wt::WWidget *w, const PropertyPath &path) {
            WebDialogBase::showDialog<W>(w, path);
            return true;
        }

        bool createRtObject(unsigned id, RtObjectRef &r)
        {
           RtObjectRef o( new R(id));
           r = std::move(o); // assign across
           return true;
        }
    };


    template <typename R, typename S= SetupI2cDialog, typename W = ConfigurationI2CWebDialog >
    /*!
     * \brief The ObjectManagerI2cRT class
     */
    class ObjectManagerI2cRT : public ObjectManagerRT<R,S,W>
    {
    public:
        ObjectManagerI2cRT(const std::string &s) : ObjectManagerRT<R,S,W>(s) {}
    };



    /*!
        \brief The TopLevelObjectManager class
        These are place holders for sections
    */
    class TopLevelObjectManager : public ObjectManager {
        public:
            TopLevelObjectManager(const std::string &n) : ObjectManager(n) {
                MRL::PropertyPath p;
                p.push_back(n);
                if (!MRL::Common::configuration().exists(p)) {
                    create(p);
                }
            }

            virtual bool canBeChildOf(const std::string &/*type*/) {
                return false;
            }
            // Flags
            bool isTopLevel() {
                return true;
            }
            virtual bool canEdit() {
                return false;
            }
            virtual bool canDelete() {
                return false;
            }
            virtual bool canView() {
                return false;
            }
            virtual bool canAdd() {
                return true;
            }
    };

    /*!
        \brief The FolderObjectManager class
    */

    class FolderObjectManager : public ObjectManager {
        public:
            FolderObjectManager() : ObjectManager(FOLDER_TYPE) {}

            // Manage flags
            bool isTopLevel() {
                return false;
            }
            virtual bool canEdit() {
                return false;
            }
            virtual bool canDelete() {
                return true;
            }
            virtual bool canView() {
                return false;
            }

            virtual bool canAdd() {
                return true;
            }

            virtual bool canBeChildOf(const std::string &type) {
                if (type == FOLDER_TYPE) { // can be a child of a folder
                    return true;
                }
                else {
                    auto o = ObjectManager::find(type);
                    if (o) {
                        if (o->isTopLevel()) { // can be child of a top level item
                            return true;
                        }
                    }
                }
                return false;
            }
    };
}
#endif // OBJECTMANAGER_H
