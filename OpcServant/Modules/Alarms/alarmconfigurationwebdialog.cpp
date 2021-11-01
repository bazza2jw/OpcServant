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
#include "alarmconfigurationwebdialog.h"
#include <Wt/WGridLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WComboBox.h>
#include <Wt/WSpinBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WCheckBox.h>
#include <set>
#include <Common/Daq/daqcommon.h>
/*!
    \brief SimulatorConfigurationWebForm
    \param path
    \return
*/
void MRL::AlarmConfigurationWebDialog::setup() {

    try {
        Wt::WGridLayout *layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>());  // a bag grid layout
        setWindowTitle("Configure Alarm");
        layout->setColumnResizable(0, true, Wt::WLength(30, Wt::LengthUnit::Percentage));
        // add the items to the grid
        //



        //
        layout->addWidget(std::make_unique<Wt::WLabel>("Measure Interval(s)"), 1, 0);
        _measureInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 1, 1);
        layout->addWidget(std::make_unique<Wt::WLabel>("Publish Interval(s)"), 2, 0);
        _publishInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(), 2, 1);
        //
        //
        _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enabled"), 3, 1);
        //
        _groupList = layout->addWidget(std::make_unique<Wt::WSelectionBox>(), 4, 1);
        _groupList->setSelectionMode(Wt::SelectionMode::Extended); // allow multiple selections
        //
        layout->addWidget(std::make_unique<Wt::WLabel>("Action"), 5, 0);
        _actionObject = layout->addWidget(std::make_unique<Wt::WComboBox>(), 5, 1);
        _actionOutput = layout->addWidget(std::make_unique<Wt::WComboBox>(), 5, 2);
        layout->addWidget(std::make_unique<Wt::WLabel>("Alert"), 6, 0);
        _alertObject = layout->addWidget(std::make_unique<Wt::WComboBox>(), 6, 1);
        _alertOutput = layout->addWidget(std::make_unique<Wt::WComboBox>(), 6, 2);
        layout->addWidget(std::make_unique<Wt::WLabel>("Ok"), 7, 0);
        _okObject = layout->addWidget(std::make_unique<Wt::WComboBox>(), 7, 1);
        _okOutput = layout->addWidget(std::make_unique<Wt::WComboBox>(), 7, 2);
        //
        _measureInterval->setMaximum(60);
        _measureInterval->setMinimum(1);
        _measureInterval->setValue(10);
        _publishInterval->setMaximum(600);
        _publishInterval->setMinimum(0);
        _publishInterval->setValue(0);
        //
        _actionObject->setCurrentIndex(0);
        _alertObject->setCurrentIndex(0);
        _okObject->setCurrentIndex(0);
        _actionOutput->setCurrentIndex(0);
        _alertOutput->setCurrentIndex(0);
        _okOutput->setCurrentIndex(0);
        //
        for (auto i = MRL::Common::daq().objects().begin(); i != MRL::Common::daq().objects().end(); i++) {
            MRL::RtObjectRef &r = i->second;
            if (r) {
                if (r->hasOutputs()) {
                    std::string s;
                    r->path().toString(s);
                    _actionObject->addItem(s); // add the
                    _alertObject->addItem(s); // add the
                    _okObject->addItem(s); // add the
                }
            }
        }

        MRL::StringList valueList; // list of Value objects - TO DO other value types - maybe a isValue() flag is needed
        auto o = MRL::ObjectManager::find("Value");
        if (o) {
            //
            for (auto i = MRL::Common::daq().objects().begin(); i != MRL::Common::daq().objects().end(); i++) {
                MRL::RtObjectRef &r = i->second;
                if (r) {
                    std::string s;
                    r->path().toString(s);
                    if (MRL::Common::configuration().typeId(r->path()) == o->type()) {
                        valueList.push_back(s);
                    }
                }
            }
        }
        //
        Json::Value v;
        if (MRL::Common::configuration().getData(_path, v)) {

            //
            // get the selection list as object ids
            std::string sa = v["Selection"].asString();
            wxArrayString sl =  wxStringTokenize(sa, ",");
            std::vector<unsigned> si(sl.Count() + 1);
            for (size_t i = 0; i < sl.Count(); i++) {
                unsigned id = 0;
                sl[i].ToULong((unsigned long *)&id);
                si[i] = id;
            }
            //

            int n = 0;
            std::set< int > selects;
            for (auto k = valueList.begin(); k != valueList.end(); k++, n++) {
                MRL::PropertyPath p;
                p.toList(*k);
                _groupList->addItem(*k);
                unsigned id = MRL::Common::configuration().find(p);
                for (size_t j = 0; j < sl.Count(); j++) { // is it in the checked list
                    if (id == si[j]) {
                        selects.insert(n); // add the section to the set
                        break;
                    }
                }
            }
            // set the selections
            _groupList->setSelectedIndexes(selects);

            // do not always have to convert to tree - esp if simple property list
            Json::Value &mi = v["MeasureInterval"];
            Json::Value &ei = v["Enabled"];
            //
            _measureInterval->setValue(mi.asInt());
            _enabled->setCheckState(ei.asBool() ? Wt::CheckState::Checked : Wt::CheckState::Unchecked);
            //
            size_t j = _actionObject->findText(v["ActionObject"].asString());
            _actionObject->setCurrentIndex(j);
            j = _alertObject->findText(v["AlertObject"].asString());
            _alertObject->setCurrentIndex(j);
            j = _okObject->findText(v["OkObject"].asString());
            _okObject->setCurrentIndex(j);
            //
        }
        else {
            _actionObject->setCurrentIndex(0);
            _alertObject->setCurrentIndex(0);
            _okObject->setCurrentIndex(0);
        }
        //
        setOutputs(_actionObject->currentText().toUTF8(), _actionOutput);
        setOutputs(_alertObject->currentText().toUTF8(), _alertOutput);
        setOutputs(_okObject->currentText().toUTF8(), _okOutput);
        //
        size_t j = _actionOutput->findText(v["ActionOutput"].asString());
        _actionOutput->setCurrentIndex(j);
        j = _alertOutput->findText(v["AlertOutput"].asString());
        _alertOutput->setCurrentIndex(j);
        j = _okOutput->findText(v["OkObject"].asString());
        _okOutput->setCurrentIndex(j);
        //
        // connect signals
        _actionObject->activated().connect(this, &MRL::AlarmConfigurationWebDialog::OnActionObject);
        _alertObject->activated().connect(this, &MRL::AlarmConfigurationWebDialog::OnAlertObject);
        _okObject->activated().connect(this, &MRL::AlarmConfigurationWebDialog::OnOkObject);
        //
        WebDialogBase::setup();
    }
    catch (...) {
        std::cerr << "Exception" << __FUNCTION__ << std::endl;
    }
}

/*!
    \brief MRL::SimulatorConfigurationWebForm::OnOk
*/
void MRL::AlarmConfigurationWebDialog::onOk() {
    Json::Value v;
    v["MeasureInterval"] = _measureInterval->value();
    v["PublishInterval"] = _publishInterval->value();
    v["Enabled"] = _enabled->isChecked();
    MRL::Common::configuration().updateData(_path, v);
    //
    v["ActionObject"] = _actionObject->currentText().toUTF8();
    v["ActionOutput"] = _actionOutput->currentText().toUTF8();
    v["AlertObject"] = _alertObject->currentText().toUTF8();
    v["AlertOutput"] = _alertOutput->currentText().toUTF8();
    v["OkObject"] = _okObject->currentText().toUTF8();
    v["OkOutput"] = _okOutput->currentText().toUTF8();
    //
    // get the selected items
    const std::set<int> &sel = _groupList->selectedIndexes();
    //
    if (sel.size() > 0) {
        std::stringstream ss;
        // set the selection array
        for (auto  i = sel.begin(); i != sel.end(); i++) {
            std::string s = _groupList->itemText(*i).toUTF8(); // selected path
            // find the object id
            MRL::PropertyPath p;
            p.toList(s);
            unsigned id = MRL::Common::configuration().find(p);
            if (id > 0) {
                ss << id << ",";
            }
        }
        v["Selection"] = ss.str();
    }
    else {
        v["Selection"] = std::string("");
    }
    MRL::Common::configuration().updateData(_path, v);
}

void MRL::AlarmConfigurationWebDialog::setOutputs(const std::string &s, Wt::WComboBox *c) {
    MRL::PropertyPath p;
    p.toList(s); // convert to rpath
    // find object from path
    unsigned id = MRL::Common::configuration().find(p); // find the object id
    // locate the object
    MRL::RtObjectRef &r = MRL::Common::daq().objects()[id];
    // subtle consideration - an RT objects input names are not necessaraly fixed to object type
    c->clear();
    if (r) {
        MRL::StringVector &in = r->outputs();
        for (size_t i = 0; i < in.size(); i++) {
            c->addItem(in[i]);
        }
        c->setCurrentIndex(0);
    }
}


/*!
    \brief AlarmConfigurationDialog::OnActionObject
    \param event
*/
void MRL::AlarmConfigurationWebDialog::OnActionObject(int i) {
    setOutputs(_actionObject->itemText(i).toUTF8(), _actionOutput);
}
/*!
    \brief AlarmConfigurationDialog::OnAlertObject
    \param event
*/
void MRL::AlarmConfigurationWebDialog::OnAlertObject(int i) {
    setOutputs(_alertObject->itemText(i).toUTF8(), _alertOutput);
}
/*!
    \brief AlarmConfigurationDialog::OnOkObject
    \param event
*/
void MRL::AlarmConfigurationWebDialog::OnOkObject(int i) {
    setOutputs(_okObject->itemText(i).toUTF8(), _okOutput);
}



