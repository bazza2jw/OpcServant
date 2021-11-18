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
#include "webaddobjectdialog.h"


/*!
    \brief MRL::WebAddObjectDialog::setup
    \param path parent object path
*/
void MRL::WebAddObjectForm::setup() {
    auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
    // add the items to the grid
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Name")), 0, 0);
    _name = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Type")), 1, 0);
    _type = layout->addWidget(std::make_unique<Wt::WComboBox>(), 1, 1);
    // Set up the name validation handler

    unsigned t = Common::configuration().typeId(_path);
    auto pobj = ObjectManager::find(t);
    if (pobj) {
        //
        // Set names of existing objects at path
        _currentNames.clear();
        MRL::Common::configuration().tree().listChildren(_path, _currentNames);
        _type->clear();
        // populate list
        std::list<std::string> l;
        ObjectManager::listNames(l);
        for (auto i = l.begin(); i != l.end(); i++) {
            auto *o = MRL::ObjectManager::find(*i);
            if (o && o->canBeChildOf(pobj->name())) {
                _type->addItem(*i);
            }
        }
    }
    WebDialogBase::setup();
}
