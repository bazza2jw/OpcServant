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
#include "VirtualKeypad.h"

VirtualKeypad *VirtualKeypad::_keypad;

VirtualKeypad::VirtualKeypad(wxWindow *parent)
    : VirtualKeypadBase(parent) {
    SetSize(10, 10, 500, 400);

}

VirtualKeypad::~VirtualKeypad() {

}

void VirtualKeypad::onBack(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = WXK_DELETE;
    GetTextEntry()->SetInsertionPoint(0);
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_0(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '0';
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_1(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '1';
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_2(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '2';
    GetTextEntry()->EmulateKeyPress(e);

}
void VirtualKeypad::on_3(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '3';
    GetTextEntry()->EmulateKeyPress(e);

}
void VirtualKeypad::on_4(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '4';
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_5(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '5';
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_6(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '6';
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_7(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '7';
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_8(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '8';
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_9(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '9';
    GetTextEntry()->EmulateKeyPress(e);
}
void VirtualKeypad::on_dp(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '.';
    GetTextEntry()->EmulateKeyPress(e);
}
/*!
    \brief VirtualKeypad::on_minus
*/
void VirtualKeypad::on_minus(wxCommandEvent & /*event*/) {
    wxKeyEvent e(wxEVT_KEY_DOWN);
    e.m_keyCode = '-';
    GetTextEntry()->EmulateKeyPress(e);
}
/*!
    \brief VirtualKeypad::onCancel
*/
void VirtualKeypad::onCancel(wxCommandEvent & /*event*/) {
    if (IsModal()) {
        EndModal(wxID_CANCEL);
    }
    else {
        close();
    }
}
/*!
    \brief VirtualKeypad::onOK
*/
void VirtualKeypad::onOK(wxCommandEvent & /*event*/) {
    if (IsModal()) {
        EndModal(wxID_OK);
    }
    else {
        setText();
        close();
    }
}
