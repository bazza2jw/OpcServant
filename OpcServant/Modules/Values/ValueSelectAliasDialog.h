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
#ifndef VALUESELECTALIASDIALOG_H
#define VALUESELECTALIASDIALOG_H
#include "ValuesUI.h"
#include <MrlLib/mrllib.h>
/*!
 * \brief The ValueSelectAliasDialog class
 */
class ValueSelectAliasDialog : public ValueSelectAliasDialogBase
{
public:
    std::string _result;
    ValueSelectAliasDialog(wxWindow* parent);
    virtual ~ValueSelectAliasDialog();
protected:
    virtual void onOk(wxCommandEvent& event);
};
#endif // VALUESELECTALIASDIALOG_H
