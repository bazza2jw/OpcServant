/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef WEBDATAMODEL_H
#define WEBDATAMODEL_H

#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WTreeView.h>

namespace MRL {
    /*!
        \brief The WebDataModel class
        Model that handles the navigation tree
    */
    class WebDataModel : public Wt::WStandardItemModel {
        public:
            enum {
                // the columns
                Item = 0, Value,Type, Status,  NumberColumns
            };
        public:
            WebDataModel() :  Wt::WStandardItemModel(0, Value + 1) {
                setHeaderData(Item, Wt::Orientation::Horizontal, std::string("Item"));
                setHeaderData(Value, Wt::Orientation::Horizontal, std::string("Value"));
            }

    };
    typedef std::shared_ptr<WebDataModel> WebDataModelRef;
}
#endif // WEBDATAMODEL_H
