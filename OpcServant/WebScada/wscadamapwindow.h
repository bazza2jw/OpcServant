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
#ifndef WSCADAMAPWINDOW_H
#define WSCADAMAPWINDOW_H
#include <Wt/WPaintedWidget.h>
#include <Wt/WPaintDevice.h>
namespace MRL
{
    /*!
     * \brief The WScadaMapWindow class
     */
    class WScadaMapWindow : public Wt::WPaintedWidget
    {
    public:
        WScadaMapWindow();
        virtual void 	paintEvent (Wt::WPaintDevice *paintDevice);
    };
}
#endif // WSCADAMAPWINDOW_H
