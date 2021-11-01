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
#ifndef SERVEROBJECT_H
#define SERVEROBJECT_H

#include <Wt/WException.h>
#include <MrlLib/mrllib.h>
#include <wx/thread.h>
#include <wx/event.h>

namespace MRL
{
    /*!
     * \brief The ServerObject class
     * Non GUI functions running on the main thread
     * Console interface and thread sync
     */
/*!
     * \brief The ServerObject class
     */
    class ServerObject : wxTimer
    {
        static wxMutex _mutex; //!< sync mutex
        static wxCondition _condition;//!< wait condition

    public:
        ServerObject()
        {
            Start(100);
        }

        void Notify();
        /*!
         * \brief mutex
         * \return sync mutex
         */
        static wxMutex & mutex() { return _mutex;}
        /*!
         * \brief condition
         * \return wait condition
         */
        static wxCondition & condition() { return _condition;}
        //

    };
}
#endif // SERVEROBJECT_H
