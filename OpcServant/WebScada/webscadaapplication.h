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
#ifndef WEBSCADAAPPLICATION_H
#define WEBSCADAAPPLICATION_H
#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WEnvironment.h>
namespace Wt
{
    class WContainerWidget;
    class WNavigationBar;
    class WTreeTable;
    class WMenuItem;
    class WTreeView;
}
namespace MRL {
    class WebScadaApplication : public Wt::WApplication
    {
        Wt::WContainerWidget * _topContainer = nullptr;
        std::shared_ptr<Wt::WBootstrapTheme> _theme;

    public:
        WebScadaApplication(const Wt::WEnvironment& env);
    };
}
#endif // WEBSCADAAPPLICATION_H
