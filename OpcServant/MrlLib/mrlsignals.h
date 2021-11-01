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
#ifndef MRLSIGNALS_H
#define MRLSIGNALS_H

#include <vector>
#include <map>
#include <string>
//
#include <Wt/WSignal.h>
#include "variant.h"
//
// Convience macros to declare signals
//
#define SIGNAL0(n) \
    private: Wt::Signal<void> _##n;\
    public: Wt::Signal<void> & n() { return _##n;}


#define SIGNAL1(n,t) \
    private: Wt::Signal<t> _##n;\
    public: Wt::Signal<t> & n() { return _##n;}

#define SIGNALT(n,t) \
    private: T _##n;\
    public: T & n() { return _##n;}



#endif // MRLSIGNALS_H
