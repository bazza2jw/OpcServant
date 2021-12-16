/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file provided .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef WEBLOGINDIALOG_H
#define WEBLOGINDIALOG_H
#include <Common/Web/webdialog.h>
// use local username password for authentication

namespace MRL
{
    class WebLoginDialog :  public Wt::WContainerWidget
    {
        Wt::WLineEdit * _name = nullptr;
        Wt::WLineEdit * _password = nullptr;
        Wt::WToolBar * _toolBar = nullptr;
        bool _result = false;
    public:
        WebLoginDialog();
        void setup();
        void onOk();
        void onCancel();
        bool result() const { return _result;}
    };
}

#endif // WEBLOGINDIALOG_H
