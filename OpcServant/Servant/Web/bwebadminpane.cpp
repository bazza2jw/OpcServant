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
#include "bwebadminpane.h"
#include <Wt/WTextArea.h>
#include <Wt/WFitLayout.h>
MRL::BWebAdminPane::BWebAdminPane()
{
    auto layout = std::make_unique<Wt::WFitLayout>();
    _tabWidget = layout->addWidget(std::make_unique<Wt::WTabWidget>());
    _tabWidget->addTab(std::make_unique<Wt::WTextArea>("ADMIN This is the contents of the first tab."),"First", Wt::ContentLoading::Eager);
    _tabWidget->addTab(std::make_unique<Wt::WTextArea>("ADMIN  This is the contents of the second tab."),"Second", Wt::ContentLoading::Eager);
    _tabWidget->addTab(std::make_unique<Wt::WTextArea>("ADMIN This is the contents of the third tab."),"Third", Wt::ContentLoading::Eager);
    _tabWidget->setStyleClass("tabwidget");


}
