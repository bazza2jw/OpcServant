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
#ifndef ANYPROPERTYTREE_H
#define ANYPROPERTYTREE_H
#include "mrllib.h"
#include "variantpropertytree.h"

namespace MRL {

    inline std::string valueToString(const boost::any &v) {
        return VariantStreamerBase::anyToString(v);
    }

    template <typename T> T &valueToType(const boost::any &v) {
        return boost::any_cast<T>(v);
    }

    /*!
        \brief The AnyTree class
    */
    class  AnyPropertyTree : public ValueTree<boost::any> {
        public:

            const std::string AnyNodeType(ValueNode *n) {
                return n->data().type().name(); // get the type of the data at a node
            }
            //
            template <typename P, typename T>
            /*!
                \brief setPointer
                \param path
                \param v
            */
            void setPointer(P path, T *p) {
                auto sp = std::shared_ptr<T>(p);
                setValue(path, sp);
            }

            template <typename T, typename P>
            /*!
                \brief getPointer
                \param path
                \return pointer to value or null
            */
            T *getPointer(P path) {
                T *ret = nullptr;
                auto n = node(path);
                if (n) {
                    return getPointer<T>(n);
                }
                return ret;
            }

            template <typename T>
            /*!
                \brief getPointer
                \param n
                \return pointer to value or null
            */
            T *getPointer(ValueNode *n) {
                T *ret = nullptr;
                if (n) {
                    try {
                        boost::any &a = n->data();
                        if (!a.empty() && isType<std::shared_ptr<T> >(a)) {
                            std::shared_ptr<T> *s = boost::any_cast<std::shared_ptr<T> >(&a);
                            ret = s->get();
                        }
                    }
                    catch (...) {

                    }
                }
                return ret;
            }


    };



}

#endif // VARIANTPROPERTYTREE_H
