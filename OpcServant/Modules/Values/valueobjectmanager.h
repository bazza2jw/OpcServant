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
#ifndef VALUEOBJECTMANAGER_H
#define VALUEOBJECTMANAGER_H
#include <Common/objectmanager.h>
#include "valuert.h"
#include "ValueGroupConfigurationDialog.h"
#include "valuesetupwebdialog.h"
#include "valuegroupconfigurationwebdialog.h"

namespace MRL
{
/*!
     * \brief The ValueObjectManager class
     */
typedef ObjectManagerRT < ValueRT, ValueGroupConfigurationDialog, MRL::ValueGroupConfigurationWebDialog > ValueObjectManagerBase;
class ValueObjectManager : public ValueObjectManagerBase
{
public:
    ValueObjectManager() :  ValueObjectManagerBase("Value") {}
    wxWindow * createTabWindow(wxWindow *parent, unsigned id);
    void createTabWindow(Wt::WTabWidget * /*tab*/,unsigned /*id*/);
    Wt::WContainerWidget * createWebWindow( Wt::WContainerWidget */*root*/, unsigned /*id*/);

};
}


#endif // VALUEOBJECTMANAGER_H
