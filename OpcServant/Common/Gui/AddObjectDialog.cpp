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
#include "AddObjectDialog.h"
#include <Common/database.h>
#include <Common/objectmanager.h>

/*!
 * \brief AddObjectDialog::AddObjectDialog
 * \param parent
 * \param path
 */
AddObjectDialog::AddObjectDialog(wxWindow* parent, const MRL::PropertyPath &path)
    : AddObjectDialogBase(parent),_path(path)
{
    std::list<std::string> l; //!< list of children
    MRL::Common::configuration().tree().listChildren(path,l);
    //
    unsigned ptype = MRL::Common::configuration().typeId(path);
    auto pobj = MRL::ObjectManager::find(ptype);
    //
    // add the string list to the combo box
    for(auto i = l.begin(); i != l.end(); i++)
    {
        GetObjectName()->Append(*i);
    }
    //
    // Now populate the type list
    //
    MRL::ObjectManager::listNames(l);
    for(auto i = l.begin(); i != l.end(); i++)
    {
        auto *o = MRL::ObjectManager::find(*i);
        if(o && o->canBeChildOf(pobj->name()))
        {
            GetType()->AppendString(*i);
        }
    }
    GetType()->SetSelection(0);
    m_buttonOk->Enable(false);
}

/*!
 * \brief AddObjectDialog::~AddObjectDialog
 */
AddObjectDialog::~AddObjectDialog()
{

}

/*!
 * \brief AddObjectDialog::OnOk
 */
void AddObjectDialog::OnOk(wxCommandEvent& /*event*/)
{
    EndModal(wxID_OK);
}

/*!
 * \brief AddObjectDialog::OnEditChanged
 */
void AddObjectDialog::OnEditChanged(wxCommandEvent& /*event*/)
{
    wxString s = GetObjectName()->GetValue().Trim(true).Trim(false);
    // enable Ok button only when name is unique
    m_buttonOk->Enable(!(s.IsEmpty() || (GetObjectName()->FindString(s) != wxNOT_FOUND)));
}
