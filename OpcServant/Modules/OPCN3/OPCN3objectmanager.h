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
#ifndef OPCN3OBJECTMANAGER_H
#define OPCN3OBJECTMANAGER_H

#include <Common/objectmanager.h>
#include "OPCN3rt.h"
#include "OPCN3ConfigurationDialog.h"
#include "OPCN3webconfiguration.h"

namespace MRL
{
/*!
 * \brief The OPCN3ObjectManager class
 */
typedef ObjectManagerRT<OPCN3RT,OPCN3ConfigurationDialog,MRL::OPCN3WebConfiguration> OPCN3ObjectManagerBase;
class OPCN3ObjectManager: public OPCN3ObjectManagerBase
{
public:
    OPCN3ObjectManager() : OPCN3ObjectManagerBase("OPCN3"){}
    //
    wxWindow * createTabWindow(wxWindow *parent, unsigned id);
    void createTabWindow(Wt::WTabWidget * /*tab*/,unsigned /*id*/);
    Wt::WContainerWidget * createWebWindow( Wt::WContainerWidget */*root*/, unsigned /*id*/);


};
}
#endif // OPCN3OBJECTMANAGER_H
