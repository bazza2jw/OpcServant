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
#ifndef SBCSUPPORTOBJECTMANAGER_H
#define SBCSUPPORTOBJECTMANAGER_H


#include <Common/objectmanager.h>
#include "sbcsupport_global.h"

namespace MRL
{
/*!
     * \brief The SbcSupportObjectManager class
     */
    class SbcSupportObjectManager : public ObjectManager
    {
    public:
        SbcSupportObjectManager();
        //
        virtual bool createRT(const PropertyPath &path);
        // Configuration
        virtual bool create(const PropertyPath &path);
        //
        // Variants for GUI and Web
        virtual bool edit(wxWindow *w, const PropertyPath &s);
        virtual bool edit(Wt::WWidget *w, const PropertyPath &s);
        //
        virtual bool view(wxWindow *parent, const PropertyPath &s);
        virtual bool view(Wt::WWidget *parent, const PropertyPath &s);
        //
        virtual bool update(const PropertyPath &s);  // updated as an object so includes all children
        virtual bool canBeChildOf(const std::string &type) {
            return (type == "Site");
        }
        virtual bool createRtObject(unsigned, RtObjectRef &);
        virtual bool canRun() {
            return true; //!< is the object an active runtime object ?
        }
        //
        wxWindow * createTabWindow(wxWindow *parent, unsigned id);
     };
}

#endif // SBCSUPPORTOBJECTMANAGER_H
