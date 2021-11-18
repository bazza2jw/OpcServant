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
#ifndef STORAGETREE_H
#define STORAGETREE_H
#include <map>
#include <string>
#include <list>

namespace MRL {
/*!
     * \brief The StorageNode struct
     */
    struct StorageNode
    {
        unsigned _id = 0;
        std::string _browseName;
        unsigned _parent = 0;
    };

    class StorageTree {

        public:

        private:
            StorageNode _root;

        public:
            StorageTree();
    };
}
#endif // STORAGETREE_H
