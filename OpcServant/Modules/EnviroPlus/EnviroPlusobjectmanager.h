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
#ifndef EnviroPlusOBJECTMANAGER_H
#define EnviroPlusOBJECTMANAGER_H
#include <Common/objectmanager.h>

namespace MRL
{
/*!
     * \brief The EnviroPlusObjectManager class
     */
    class EnviroPlusObjectManager : public ObjectManager
    {
    public:
        EnviroPlusObjectManager();
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
            return (type == "Equipment") || (type == "Folder");
        }
        virtual bool createRtObject(unsigned, RtObjectRef &);
        virtual bool canRun() {
            return true; //!< is the object an active runtime object ?
        }
        size_t opcHistoryCount() const { return 1;}
        wxWindow * createTabWindow(wxWindow *parent, unsigned id);
        void createTabWindow(Wt::WTabWidget * /*tab*/,unsigned /*id*/);
        Wt::WContainerWidget * createWebWindow( Wt::WContainerWidget */*root*/, unsigned /*id*/);

     };
}
#endif // EnviroPlusOBJECTMANAGER_H
