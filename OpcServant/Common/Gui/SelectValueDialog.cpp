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
#include "SelectValueDialog.h"
#include <Common/common.h>
#include <Common/Daq/daqcommon.h>

/*!
 * \brief SelectValueDialog::SelectValueDialog
 * \param parent
 */
SelectValueDialog::SelectValueDialog(wxWindow* parent)
    : SelectValueDialogBase(parent)
{
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
                for(int i = 0; i < int(l.size()); i++)
                {
                    std::string v = s + TAG_SEPERATOR + l[i];
                    GetListAlias()->AppendString(v); // add the
                }
            }
        }
    }

}

/*!
 * \brief SelectValueDialog::~SelectValueDialog
 */
SelectValueDialog::~SelectValueDialog()
{

}

/*!
 * \brief SelectValueDialog::onOK
 * \param event
 */
void SelectValueDialog::onOK(wxCommandEvent& /*event*/)
{
    EndModal(wxID_OK);
}

/*!
 * \brief SelectValueDialog::onSelect
 */
void SelectValueDialog::onSelect(wxCommandEvent& /*event*/)
{
    selected = GetListAlias()->GetString(GetListAlias()->GetSelection());
}
