
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
#include "I2cConfigurePanel.h"

/*!
 * \brief I2cConfigurePanel::I2cConfigurePanel
 * \param parent
 */
I2cConfigurePanel::I2cConfigurePanel(wxWindow* parent)
    : I2cConfigurePanelBase(parent)
{
    wxDir d("/dev");
    if(d.IsOpened())
    {
        wxString f;
        if(d.GetFirst (&f, "*i2c*", wxDIR_FILES ))
        {
            do
            {
                wxString s = "/dev/" + f;
                GetI2cDevice()->AppendString(s);
            } while(d.GetNext(&f));
        }
    }

}

/*!
 * \brief I2cConfigurePanel::~I2cConfigurePanel
 */
I2cConfigurePanel::~I2cConfigurePanel()
{
}

