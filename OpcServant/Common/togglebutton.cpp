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
#include "togglebutton.h"
#include "VirtualKeypadUI.h"

static ButtonImages *_images = nullptr;
/*!
 * \brief ToggleButton::ToggleButton
 * \param parent
 * \param id
 * \param label
 * \param pos
 * \param size
 * \param style
 * \param val
 * \param name
 */
ToggleButton::ToggleButton(wxWindow *parent, wxWindowID id)
{
        if(!_images) _images = new ButtonImages();
        Create(parent, id, _images->Bitmap("m_off"));
        SetValue(false);
        Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(ToggleButton::toggle), NULL, this);
}

/*!
 * \brief ToggleButton::SetValue
 * \param f
 */
void ToggleButton::SetValue(bool f)
{
    wxBitmapToggleButton::SetValue(f);
    if(f)
    {
        SetBitmap(_images->Bitmap("m_on"));
    }
    else
    {
        SetBitmap(_images->Bitmap("m_off"));
    }
}
/*!
 * \brief ToggleButton::toggle
 * \param e
 */
void ToggleButton::toggle(wxCommandEvent& e)
{
    if(e.GetInt())
    {
        SetBitmap(_images->Bitmap("m_on"));
    }
    else
    {
        SetBitmap(_images->Bitmap("m_off"));
    }
}


