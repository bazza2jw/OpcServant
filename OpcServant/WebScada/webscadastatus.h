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
#ifndef WEBSCADASTATUS_H
#define WEBSCADASTATUS_H
#include "webscadamain.h"
#include "webscadastatustable.h"
namespace MRL {
    /*!
           \brief The WebScadaStatus class
           A status table
    */
    typedef std::unique_ptr<Wt::WStandardItemModel> StandardItemModelRef;
    typedef std::map<std::string, StandardItemModelRef> StandardItemModelMap;


    class WebScadaStatus : public Wt::WContainerWidget {
            static StandardItemModelMap _model; // set of models - there may be multiple users each with a different view or charing views
            std::string _currentView = "default"; // the view
            Wt::WNavigationBar *_navigationBar = nullptr; // menu bar
            WebScadaStatusTable *_table = nullptr;  // the table

        public:
            WebScadaStatus();
            void setModel(const std::string &s);
            //
            static StandardItemModelRef &model(const std::string &s) {
                return _model[s];
            }
            //
            //
            static bool loadView(const std::string &name); // load a view
            static bool saveView(const std::string &name); // save a view
            static bool newView(Wt::WWidget *parent, std::string &name); // create a new view
            static void update(); // update the status tables
            //

    };
}
#endif // WEBSCADASTATUS_H
