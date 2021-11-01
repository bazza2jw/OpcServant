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
#ifndef EDITOBJECTDIALOG_H
#define EDITOBJECTDIALOG_H
#include "BourbonCommonUI.h"
#include "Gui/bourboncommon.h"
#include "Gui/propertysheet.h"
#include "Gui/Gui/guipropertysheet.h"
#include "Gui/objectmanager.h"

class EditObjectDialog : public EditObjectDialogBase
{
    MRL::PropertyPath _path; // path to object being edited
    MRL::GuiPropertySheet  _common; // property sheet interface
    MRL::GuiPropertySheet  _recipe;
    std::string _currentRecipe;

public:
    EditObjectDialog(wxWindow* parent, const MRL::PropertyPath &path);
    virtual ~EditObjectDialog();
    void setCommonPropertySheet();
    void setRecipePropertySheet();

protected:
    virtual void OnRecipeSelectionChanged(wxCommandEvent& event);
    virtual void OnOk(wxCommandEvent& event);
};
#endif // EDITOBJECTDIALOG_H
