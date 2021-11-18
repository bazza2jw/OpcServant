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
#include "AliasConfigurationDialog.h"
#include <Common/database.h>
#include <Common/common.h>
#include "AddAliasDialog.h"
/*!
 * \brief AliasConfigurationDialog::AliasConfigurationDialog
 * \param parent
 */
AliasConfigurationDialog::AliasConfigurationDialog(wxWindow* parent)
    : AliasConfigurationDialogBase(parent)
{
    // populate the list
    MRL::StringMap &a = MRL::Common::aliasMap();
    for(auto i = a.begin(); i != a.end(); i++)
    {
        // a text key , text value pair
        wxVector<wxVariant> data;
        data.push_back(wxVariant(wxString(i->first)));
        data.push_back(wxVariant(wxString(i->second)));
        GetAliasList()->AppendItem(data); // add to list
    }
}

/*!
 * \brief AliasConfigurationDialog::~AliasConfigurationDialog
 */
AliasConfigurationDialog::~AliasConfigurationDialog()
{
}



/*!
 * \brief AliasConfigurationDialog::onAdd
 * \param event
 */
void AliasConfigurationDialog::onAdd(wxCommandEvent& /*event*/)
{
    // add an item
    AddAliasDialog dlg(this);
    if(dlg.ShowModal() == wxID_OK)
    {
        wxVector<wxVariant> data;
        wxString k = dlg.GetAlias()->GetValue();
        //
        std::string s;
        if(!MRL::Common::configuration().getAlias(k.ToStdString(), s)) // unique
        {
            // does the key exists?
            wxString v = dlg.GetValueString()->GetValue();
            data.push_back(wxVariant(k));
            data.push_back(wxVariant(v));
            GetAliasList()->AppendItem(data); // add to list
            // now add to database
            if(!MRL::Common::configuration().addAlias(k.ToStdString(),v.ToStdString()))
            {
                wxLogDebug("addAlias Fails");
            }
            else
            {
                MRL::Common::aliasMap()[k.ToStdString()] = v.ToStdString();
                MRL::Common::reverseAliasMap()[ v.ToStdString()] = k.ToStdString();
            }
        }
        else
        {
            wxMessageBox("Alias exists already","Add Alias");
        }
    }
}
/*!
 * \brief AliasConfigurationDialog::onDelete
 * \param event
 */
void AliasConfigurationDialog::onDelete(wxCommandEvent& /*event*/)
{
    int i = GetAliasList()->GetSelectedRow();
    if(i >= 0)
    {
        std::string k = GetAliasList()->GetTextValue(i,0).ToStdString();
        std::string v = GetAliasList()->GetTextValue(i,1).ToStdString();
        //
        GetAliasList()->DeleteItem(i);
        MRL::Common::configuration().deleteAlias(k);
        MRL::Common::aliasMap().erase(k);
        //
        if(MRL::Common::reverseAliasMap()[v] == k)
        {
            MRL::Common::reverseAliasMap().erase(v);
        }
    }
}
/*!
 * \brief AliasConfigurationDialog::onEdit
 * \param event
 */
void AliasConfigurationDialog::onEdit(wxCommandEvent& event)
{
    int i = GetAliasList()->GetSelectedRow();
    if(i >= 0)
    {
        wxString k = GetAliasList()->GetTextValue(i,0);
        wxString v = GetAliasList()->GetTextValue(i,1);
        AddAliasDialog dlg(this);
        dlg.GetAlias()->SetValue(k);
        dlg.GetAlias()->Enable(false);
        dlg.GetValueString()->SetValue(v);
        if(dlg.ShowModal() == wxID_OK)
        {
            wxString vn = dlg.GetValueString()->GetValue();
            GetAliasList()->SetTextValue(vn,i,1);
            //
            std::string a = k.ToStdString();
            std::string vo = v.ToStdString();
            std::string va = vn.ToStdString();
            //
            MRL::Common::configuration().updateAlias(a,va);
            MRL::Common::aliasMap()[a] = va;
            if(MRL::Common::reverseAliasMap()[vo] == a)
            {
                MRL::Common::reverseAliasMap().erase(vo);
            }
            MRL::Common::reverseAliasMap()[va] = a; // This may not be one to one

        }
    }
}
/*!
 * \brief AliasConfigurationDialog::onOK
 * \param event
 */
void AliasConfigurationDialog::onOK(wxCommandEvent& /*event*/)
{
    EndModal(wxID_OK);
}
