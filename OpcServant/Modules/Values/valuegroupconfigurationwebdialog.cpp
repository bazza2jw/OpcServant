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
#include "valuegroupconfigurationwebdialog.h"
#include "valuesetupwebdialog.h"
#include <Common/Web/webaliasconfigurationdialog.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WSpinBox.h>
#include <Wt/WLabel.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WCheckBox.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WMessageBox.h>
/*!
 * \brief MRL::ValueGroupConfigurationWebDialog::ValueGroupConfigurationWebDialog
 */
MRL::ValueGroupConfigurationWebDialog::ValueGroupConfigurationWebDialog(const PropertyPath &path) : _path(path)
{

}

/*!
 * \brief MRL::ValueGroupConfigurationWebDialog::setup
 */
void MRL::ValueGroupConfigurationWebDialog::setup()
{
    try
    {
        setWindowTitle("Configure Value Group");

        auto boxLayout = contents()->setLayout(std::make_unique<Wt::WVBoxLayout>());
        _toolBar = boxLayout->addWidget(std::make_unique<Wt::WToolBar>()); // add the tool bar

        // Add the buttons
        _add = createColorButton("btn-primary", _TR("Add"));
        _edit = createColorButton("btn-primary", _TR("Edit"));
        _remove = createColorButton("btn-primary", _TR("Remove"));
        _alias = createColorButton("btn-primary", _TR("Alias ..."));
        //
        _add->clicked().connect(this, &MRL::ValueGroupConfigurationWebDialog::onAdd);
        _edit->clicked().connect(this, &MRL::ValueGroupConfigurationWebDialog::onEdit);
        _remove->clicked().connect(this, &MRL::ValueGroupConfigurationWebDialog::onRemove);
        _alias->clicked().connect(this, &MRL::ValueGroupConfigurationWebDialog::onAlias);
        _itemList = boxLayout->addWidget(std::make_unique<Wt::WSelectionBox>(),2);
        // the widgets at the bottom
        auto layout = boxLayout->addLayout(std::make_unique<Wt::WGridLayout>());
        layout->addWidget(std::make_unique<Wt::WLabel>("Measure Interval(s)"),0,0);
        _measureInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(),0,1);
        //
        layout->addWidget(std::make_unique<Wt::WLabel>("Publish Interval(s)"),1,0);
        _publishInterval = layout->addWidget(std::make_unique<Wt::WSpinBox>(),1,1);
        //
        _enabled = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enabled"),2,0);
        //
        _enableTabView = layout->addWidget(std::make_unique<Wt::WCheckBox>("Enable Tab View"),3,0);
        //
        _measureInterval->setMaximum(60);
        _measureInterval->setMinimum(1);
        _measureInterval->setValue(10);
        //
        _publishInterval->setMaximum(300);
        _publishInterval->setMinimum(1);
        _publishInterval->setValue(60);
        //
        Json::Value v;
        if(MRL::Common::configuration().getData(_path,v))
        {
            _configuration.fromJson(v); // load configuration

            MRL::StringList l ;
            _configuration.listChildren("Values",l); // load up the selected aliases in the group
            // Populate the list box
            for(auto i = l.begin(); i != l.end(); i++)
            {
                _itemList->addItem(*i);
            }

            if(l.size() > 0)
            {
                _itemList->setCurrentIndex(0);
            }
            _measureInterval->setValue(_configuration.getInt("/MeasureInterval"));
            _publishInterval->setValue(_configuration.getInt("/PublishInterval"));
            _enabled->setChecked(_configuration.getBool("/Enabled"));
            _enableTabView->setChecked(_configuration.getBool("/EnableTabView"));
        }


        WebDialogBase::setup();
    }
    catch (...) {
        std::cerr << "Exception" << __FUNCTION__ << std::endl;
    }



}
/*!
 * \brief MRL::ValueGroupConfigurationWebDialog::onOk
 */
void MRL::ValueGroupConfigurationWebDialog::onOk()
{
    _configuration.setInt("/MeasureInterval",_measureInterval->value());
    _configuration.setInt("/PublishInterval",_publishInterval->value());
    _configuration.setBool("/Enabled",_enabled->isChecked());
    _configuration.setBool("/EnableTabView",_enableTabView->isChecked());


    Json::Value v;
    _configuration.toJson(v);
    MRL::Common::configuration().updateData(_path,v);

}

/*!
 * \brief MRL::ValueGroupConfigurationWebDialog::onAdd
 */
void MRL::ValueGroupConfigurationWebDialog::onAdd()
{
    // get the name
    Wt::WString s;
    if(MRL::WebGetInput::getInput(this , "New Value Name", s))
    {
        if(_itemList->findText(s) < 0)
        {
            // add to list
            _itemList->addItem(s);
            _itemList->setCurrentIndex(_itemList->findText(s));
            // add to config
            MRL::PropertyPath p;
            p.push_back("Values");
            p.push_back(s.toUTF8());
            _configuration.setValue("HiHiEnabled",false);
            _configuration.setValue("HiLoEnabled",false);
            _configuration.setValue("LoHiEnabled",false);
            _configuration.setValue("LoLoEnabled",false);
            _configuration.setValue("HiHi",0.0);
            _configuration.setValue("HiLo",0.0);
            _configuration.setValue("LoHi",0.0);
            _configuration.setValue("LoLo",0.0);
            _configuration.setValue("Scale",1.0);
            _configuration.setValue("Offset",0.0);
            _configuration.setValue("Object",std::string("None"));

            // open the editor
            onEdit();
        }
        else
        {
            Wt::WMessageBox::show("Duplicate Value","Duplicate Value", Wt::StandardButton::Ok );
        }
    }

}
/*!
 * \brief MRL::ValueGroupConfigurationWebDialog::onEdit
 */
void MRL::ValueGroupConfigurationWebDialog::onEdit()
{
    if(_itemList->count() > 0)
    {
        // get current name
        std::string name = _itemList->currentText().toUTF8();
        MRL::PropertyPath p;
        p.push_back("Values");
        p.push_back(name);
        MRL::ValueSetupWebDialog dlg(p,_configuration);
        dlg.doExec();
    }
}
/*!
 * \brief MRL::ValueGroupConfigurationWebDialog::onRemove
 */
void MRL::ValueGroupConfigurationWebDialog::onRemove()
{
    if(_itemList->count() > 0)
    {
        std::string name = _itemList->currentText().toUTF8();
        MRL::PropertyPath p;
        p.push_back("Values");
        p.push_back(name);
        _itemList->removeItem(_itemList->currentIndex());
        _configuration.remove(p);
        if(_itemList->count() > 0) _itemList->setCurrentIndex(0);
    }

}
/*!
 * \brief MRL::ValueGroupConfigurationWebDialog::onAlias
 */
void MRL::ValueGroupConfigurationWebDialog::onAlias()
{
    WebAliasConfigurationDialog d;
    d.doExec();
}

