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
#ifndef OBJECTDATAITEM_H
#define OBJECTDATAITEM_H
#include <MrlLib/variantpropertytree.h>
#include <Common/objectdatavalidator.h>
namespace MRL {
    /*!
           \brief The ObjectDataItem class
    */
    class ObjectDataItem  : public Variant {
            static unsigned _nextId;
            unsigned _objectId = 0; // the object id - this receives notifications
            unsigned _nodeId = 0; // the node id to be assigned
            //
            // can the value be written by OPC
            bool _opcWriteValue = false;
            //
            //
        public:
            /*!
                \brief ObjectDataItem
                \param oId
                \param oWV
            */
            ObjectDataItem(unsigned oId = 0, bool oWV = false) : _objectId(oId), _opcWriteValue(oWV) {
                _nodeId = _nextId++;
            }

            ObjectDataItem(const ObjectDataItem &m) = default;
            // Accessors
            bool opcWriteValue() const {
                return _opcWriteValue;
            }
            void setOpcWriteValue(bool f) {
                _opcWriteValue = f;
            }
            unsigned objectId() const {
                return _objectId;   // the object id - this receives notifications
            }
            unsigned nodeId() const {
                return _nodeId;   // the node id to be assigned
            }
            void setObjectId(unsigned v) {
                _objectId = v;   // the object id - this receives notifications
            }
            void setNodeId(unsigned v)  {
                _nodeId = v;   // the node id to be assigned
            }

    };

    /*!
        \brief The ObjectDataItemTree class
    */

    class ObjectDataItemTree : public ValueTree<ObjectDataItem> {
        public:
            ObjectDataItemTree() {}
            virtual ~ObjectDataItemTree() {}
    };

}
#endif // OBJECTDATAITEM_H
