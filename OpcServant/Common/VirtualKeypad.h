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
#ifndef VIRTUALKEYPAD_H
#define VIRTUALKEYPAD_H
#include "VirtualKeypadUI.h"

class VirtualKeypad : public VirtualKeypadBase {
        wxTextCtrl *_current = nullptr;
    public:
        VirtualKeypad(wxWindow *parent);
        virtual ~VirtualKeypad();
        void setTextCtrl(wxTextCtrl *t) {
            _current = t;
            GetTextEntry()->Clear();
            GetTextEntry()->SetValue(t->GetValue());
            GetTextEntry()->SetInsertionPoint(0);
            if (_current->GetValidator()) {
                GetTextEntry()->SetValidator(*_current->GetValidator());
            }
        }
        void setText() {
            if (_current) {
                _current->SetValue(GetTextEntry()->GetValue());
            }
        }

        static VirtualKeypad *_keypad;
        static void setKeypad(wxWindow *w)
        {
            _keypad = new VirtualKeypad(w);
        }
        static VirtualKeypad *keypad() {
            return _keypad;
        }
        static void open(wxTextCtrl *t) {
            if (_keypad) {
                _keypad->Show();
                _keypad->Raise();
                _keypad->setTextCtrl(t);
            }
        }
        static void close() {
            // set focus to the the text control's parent - so do not get recurssion
            if (_keypad) {
                if (_keypad->_current) {
                    _keypad->_current->SetFocus();
                    _keypad->_current = nullptr;
                }
                _keypad->Hide();
            }
        }

    protected:
        virtual void onCancel(wxCommandEvent &event);
        virtual void onOK(wxCommandEvent &event);
        virtual void onBack(wxCommandEvent &event);
        virtual void on_0(wxCommandEvent &event);
        virtual void on_1(wxCommandEvent &event);
        virtual void on_2(wxCommandEvent &event);
        virtual void on_3(wxCommandEvent &event);
        virtual void on_4(wxCommandEvent &event);
        virtual void on_5(wxCommandEvent &event);
        virtual void on_6(wxCommandEvent &event);
        virtual void on_7(wxCommandEvent &event);
        virtual void on_8(wxCommandEvent &event);
        virtual void on_9(wxCommandEvent &event);
        virtual void on_dp(wxCommandEvent &event);
        virtual void on_minus(wxCommandEvent &event);
};
#endif // VIRTUALKEYPAD_H
