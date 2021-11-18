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
#ifndef DATAPROPERTYITEM_H
#define DATAPROPERTYITEM_H


#include <MrlLib/mrllib.h>
#include <wx/variant.h>
#include <typeinfo>
#include <string>
#include <MrlLib/variant.h>
#include <memory>
#include <MrlLib/states.h>
#include <Wt/WStandardItem.h>

#ifdef USE_GUI
#include <wx/dataview.h>
#endif

namespace MRL {
    /*!
        \brief The DataPropertyItem class
    */
    class DataPropertyItem {
        public:
            typedef enum {
                None,
                Add, // create the Web/GUI items in the models
                Remove,  // remove the items from the models
                Update, // trigger an update
                Delete,  // delete the node from the tree - set after Remove actioned
                Reset, // reset the models
                Sync,
                SyncWithDb
            } Operation;
        private:
            unsigned _type = 0; // object type - used to key editors and type icon
            wxVariant _value;   // item value to display in tree
            //
            #ifdef USE_GUI
            wxDataViewItem _guiItem;
            #endif
            const Wt::WStandardItem *_webItem = nullptr;
            // State determines colour and icon
            STATES::States::EventAlarmLevel _alarmLevel = STATES::States::StateNone;
            STATES::States::ActiveLevel _activeLevel  = STATES::States::StateDisabled;
            STATES::States::QualityLevel _qualityLevel = STATES::States::QualityBad;
            //
            // vistor operations
            Operation _op = None;
            bool _waitGui = false; // true if waiting for op for the GUI
            bool _waitWeb = false; // waiting for web side to action
            //
        public:
            DataPropertyItem(unsigned t = 0) : _type(t) {}
            DataPropertyItem(const DataPropertyItem &) = default; // simple item copy
            virtual ~DataPropertyItem() {}

            Operation op()  {
                return _op;
            }

            void setOp(Operation o) {
                _op = o;
                _waitGui = true;
                _waitWeb = true;
            }

            bool waitGui() const {
                return _waitGui;
            }
            bool waitWeb() const {
                return _waitWeb;
            }

            void clearWaitGui() {
                _waitGui = false;
            }

            void clearWaitWeb() {
                _waitWeb = false;
            }

            void clear() {
                _op = None;
                _waitGui = false;
                _waitWeb = false;
                #ifdef USE_GUI
                _guiItem = wxDataViewItem();
                #endif
                _webItem = nullptr;
            }

            // Accessors
            wxVariant &value() {
                return _value;   // item value
            }
            STATES::States::EventAlarmLevel alarmLevel() const {
                return _alarmLevel;
            }
            STATES::States::ActiveLevel activeLevel() const {
                return _activeLevel;
            }
            STATES::States::QualityLevel qualityLevel() const {
                return _qualityLevel;
            }
            void setValue(wxVariant &v) {
                _value = v;   // item value
            }
            void setAlarmLevel(STATES::States::EventAlarmLevel v) {
                _alarmLevel = v;
            }
            void setActiveLevel(STATES::States::ActiveLevel v)  {
                _activeLevel = v;
            }
            void setQualityLevel(STATES::States::QualityLevel v) {
                _qualityLevel = v;
            }

            unsigned type() const {
                return _type;
            }

            void setType(unsigned t) {
                _type = t;
            }
            #ifdef USE_GUI
            void setItem(const wxDataViewItem &i) {
                _guiItem = i;
            }
            wxDataViewItem &guiItem() {
                return _guiItem;
            }
            #endif
            void setItem(const Wt::WStandardItem *i) {
                _webItem = i;
            }

            const Wt::WStandardItem *webItem() {
                return _webItem;
            }
    };

}


#endif // DATAPROPERTYITEM_H
