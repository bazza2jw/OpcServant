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
#include "webaliasconfigurationdialog.h"
#include <Common/common.h>
#include <Common/Daq/daqcommon.h>
#include <Wt/WMessageBox.h>


/*!
 * \brief MRL::WebAliasConfigurationDialog::WebAddAliasDialog::onOk
 */
void MRL::WebAliasConfigurationDialog::WebAddAliasDialog::onOk()
{
    alias = _alias->text().toUTF8();
    value = _valueList->currentText().toUTF8();
}

/*!
 * \brief MRL::WebAliasConfigurationDialog::WebAddAliasDialog::setup
 */
void MRL::WebAliasConfigurationDialog::WebAddAliasDialog::setup()
{
    auto layout = std::make_unique<Wt::WGridLayout>();

    layout->addWidget(std::make_unique<Wt::WLabel>("Alias"),0,0);
    _alias  = layout->addWidget(std::make_unique<Wt::WLineEdit>(alias),0,1);
    layout->addWidget(std::make_unique<Wt::WLabel>("Value"),1,0);
    _valueList  = layout->addWidget(std::make_unique<Wt::WComboBox>(),1,1);
    if(!value.empty())
    {
       _valueList->setValueText(value);
    }

    for(auto i = MRL::Common::daq().objects().begin(); i != MRL::Common::daq().objects().end(); i++)
    {
        MRL::RtObjectRef &r = i->second;
        if(r)
        {
            if(r->hasInputs())
            {
                std::string s;
                r->path().toString(s);
                MRL::StringVector &l = r->inputs();
                for(int i = 0; i < l.size(); i++)
                {
                    std::string v = s + TAG_SEPERATOR + l[i];
                    _valueList->addItem(v);
                }
            }
        }
    }


    contents()->setLayout(std::move(layout));

}


/*!
 * \brief MRL::WebAliasConfigurationDialog::WebAliasConfigurationDialog
 */
MRL::WebAliasConfigurationDialog::WebAliasConfigurationDialog()
{

}


/*!
 * \brief MRL::WebAliasConfigurationDialog::setup
 */
void MRL::WebAliasConfigurationDialog::setup()
{
    MRL::WebDialogBase::setup();
    auto boxLayout = std::make_unique<Wt::WVBoxLayout>();
    _toolBar = boxLayout->addWidget(std::make_unique<Wt::WToolBar>()); // add the tool bar
    _model = std::make_shared<Wt::WStandardItemModel>(MRL::Common::aliasMap().size(),2);
    //
    // load up the aliases
    int j = 0;
    for(auto i = MRL::Common::aliasMap().begin(); i != MRL::Common::aliasMap().end(); i++, j++)
    {
        _model->setData(j,0,Wt::WString(i->first));
        _model->setData(j,1,Wt::WString(i->second));
    }
    // add the buttons to the tool bar
    //
    (_add = createColorButton(_toolBar, "btn-primary", "Add"))->clicked().connect(this,&MRL::WebAliasConfigurationDialog::add);
    (_edit = createColorButton(_toolBar, "btn-primary", "Edit"))->clicked().connect(this,&MRL::WebAliasConfigurationDialog::edit);
    (_remove = createColorButton(_toolBar, "btn-primary", "Remove"))->clicked().connect(this,&MRL::WebAliasConfigurationDialog::remove);
    //
    // Add the list of aliases
    _aliasList = boxLayout->addWidget(std::make_unique<Wt::WSelectionBox>());
    _aliasList->setModel(_model);
    _aliasList->setModelColumn(0);
    _aliasList->setSelectionMode(Wt::SelectionMode::Single);
    _aliasList->setCurrentIndex(0);
    contents()->setLayout(std::move(boxLayout));

}

/*!
 * \brief MRL::WebAliasConfigurationDialog::add
 */
void MRL::WebAliasConfigurationDialog::add()
{
    MRL::WebAliasConfigurationDialog::WebAddAliasDialog dlg;
    if(dlg.doExec())
    {
        //
        std::string s;
        if(!MRL::Common::configuration().getAlias(dlg.alias, s)) // unique
        {
            // does the key exists?
            // now add to database
            if(!MRL::Common::configuration().addAlias(dlg.alias,dlg.value))
            {
                wxLogDebug("addAlias Fails");
            }
            else
            {
                MRL::Common::aliasMap()[dlg.alias] = dlg.value;
                MRL::Common::reverseAliasMap()[dlg.value] = dlg.alias; // This may not be one to one so last defintion holds
                std::vector< std::unique_ptr<Wt::WStandardItem> > items;
                items.push_back(std::make_unique<Wt::WStandardItem>(dlg.alias));
                items.push_back(std::make_unique<Wt::WStandardItem>(dlg.value));
                _model->insertRow(_model->rowCount(), std::move(items));
            }
        }
        else
        {
            Wt::WMessageBox::show("Alias exists already","Add Alias",  Wt::StandardButton::Ok | Wt::StandardButton::Cancel);
        }
    }

}
/*!
 * \brief MRL::WebAliasConfigurationDialog::edit
 */
void MRL::WebAliasConfigurationDialog::edit()
{
    int i = _aliasList->currentIndex();
    if(i >= 0)
    {
        MRL::WebAliasConfigurationDialog::WebAddAliasDialog dlg;
        dlg.alias = _aliasList->currentText().toUTF8();
        Wt::WStandardItem *it = _model->item(i,1);
        std::string v = it->text().toUTF8();
        dlg.value = v;
        if(dlg.doExec())
        {
            MRL::Common::configuration().updateAlias(dlg.alias,dlg.value);
            MRL::Common::aliasMap()[dlg.alias] = dlg.value;
            if(MRL::Common::reverseAliasMap()[v] == dlg.alias)
            {
                MRL::Common::reverseAliasMap().erase(v);
            }
            MRL::Common::reverseAliasMap()[dlg.value] = dlg.alias; // This may not be one to one
        }
    }
}
/*!
 * \brief MRL::WebAliasConfigurationDialog::remove
 */
void MRL::WebAliasConfigurationDialog::remove()
{
    int i = _aliasList->currentIndex();
    if(i >= 0)
    {
        //
        std::string a = _aliasList->currentText().toUTF8();
        Wt::WStandardItem *it = _model->item(i,1);
        std::string v = it->text().toUTF8();
        //
        MRL::Common::configuration().deleteAlias(a);
        MRL::Common::aliasMap().erase(a);
        if(MRL::Common::reverseAliasMap()[v] == a)
        {
            MRL::Common::reverseAliasMap().erase(v);
        }
    }
}


