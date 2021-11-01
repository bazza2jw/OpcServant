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
#ifndef OBJECTDATAVALIDATOR_H
#define OBJECTDATAVALIDATOR_H
#include <MrlLib/variant.h>
namespace MRL {
    class ObjectDataValidator {
        public:
            ObjectDataValidator() {}
            bool vaildate(const Variant &/*v*/) {return false;}
    };


    template <typename T>
    /*!
        \brief The NumberDataItem class
    */
    class NumberDataValidator : public ObjectDataValidator {
            T _upper =  0; // upper range
            T _lower =  0; // lower range
            bool _clip = false; // if true the value is clipped to the range rather than rejected
        public:
            NumberDataValidator(T u = 0, T l = 0)
                : _upper(u), _lower(l) {
            }
            /*!
                \brief vaildate
                \param v
                \return
            */
            bool vaildate(const Variant &v) {
                try {
                    T &a  = boost::get<T>(v);
                    if (_upper > _lower) { // valid range
                        if ((a >= _lower) && (a <= _upper)) return true; // in range
                        if (_clip) {
                            if (a < _lower) {
                                v = _lower;
                            }
                            else {
                                v = _upper;
                            }
                        }
                    }
                    else {
                        return true;
                    }
                }
                catch (...) {

                }
                return false;
            }
    };
    //
    typedef NumberDataValidator<int> IntDataValidator; //!< integer
    typedef NumberDataValidator<double> DoubleDataValidator; //!<
}


#endif // OBJECTDATAVALIDATOR_H
