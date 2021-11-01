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
#ifndef OPCN3WEBPANEL_H
#define OPCN3WEBPANEL_H

#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>
#include <Wt/WTimer.h>


namespace MRL
{
/*!
 * \brief The OPCN3WebPanel class
 */
class OPCN3WebPanel : public Wt::WContainerWidget
{
    class Handler : public BObject
    {
        OPCN3WebPanel * _parent = nullptr;
    public:
        Handler(unsigned id, OPCN3WebPanel * p ) : BObject(id), _parent(p)
        {
            join(MRL::Daq::instance()); // attach to the DAQ manager
            MRL::BObject::setEnabled(true); // enable this object
        }
        bool processQueueItem(const MRL::Message &msg);

    } _handler;

    Wt::WTimer *_timer = nullptr;
    Wt::WLabel * _pm1 = nullptr;
    Wt::WLabel * _pm2_5 = nullptr;
    Wt::WLabel * _pm10 = nullptr;
    Wt::WLabel * _status = nullptr;

    void onTimer();
    unsigned _id = 0;

public:
    OPCN3WebPanel(unsigned id);
    void update();
    Wt::WLabel * status() const { return _status;}
};
}
#endif // OPCN3WEBPANEL_H
