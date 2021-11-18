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
#include "ValueSelectAliasDialog.h"
#include <Common/database.h>
#include <Common/common.h>

ValueSelectAliasDialog::ValueSelectAliasDialog(wxWindow* parent)
    : ValueSelectAliasDialogBase(parent)
{
    MRL::StringMap &a = MRL::Common::aliasMap();
    if(a.size() > 0)
    {
        for(auto i = a.begin(); i != a.end(); i++)
        {
            // a text key , text value pair
            wxString s(i->first);
            GetAliasList()->Append(s); // add to list
        }
        GetAliasList()->SetSelection(0);
    }

}

ValueSelectAliasDialog::~ValueSelectAliasDialog()
{
}

void ValueSelectAliasDialog::onOk(wxCommandEvent& event)
{
    if(GetAliasList()->GetCount() > 0)
    {
        _result = GetAliasList()->GetValue().ToStdString();
    }
}
