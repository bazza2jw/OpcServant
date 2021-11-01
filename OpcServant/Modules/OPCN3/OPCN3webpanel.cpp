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
#include "OPCN3webpanel.h"
#include <Wt/WVBoxLayout.h>
#include <Wt/WTable.h>
#include "OPCN3rt.h"

/*!
 * \brief MRL::OPCN3WebPanel::OPCN3WebPanel
 */
MRL::OPCN3WebPanel::OPCN3WebPanel(unsigned id): _handler(id,this),_id(id)
{
    //
    Wt::WCssDecorationStyle css;
    Wt::WFont ft(Wt::FontFamily::SansSerif );
    ft.setSize(Wt::FontSize::Large);
    ft.setWeight(Wt::FontWeight::Bold);
    css.setFont(ft);
    setDecorationStyle(css);
    //
    _timer = addChild(std::make_unique<Wt::WTimer>());
    _timer->timeout().connect(this, &OPCN3WebPanel::onTimer);
    _timer->setInterval(std::chrono::seconds(1));
    _timer->start();
    //
   auto layout = std::make_unique<Wt::WGridLayout>(); // a bag grid layout
   layout->setColumnStretch(0,1);
   layout->setColumnStretch(1,1);
   layout->setHorizontalSpacing(10);
   int row = 0;
   layout->addWidget(std::make_unique<Wt::WLabel>(_TR("PM1")), row, 0);
   _pm1 = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("0.0")),row++, 1);
   layout->addWidget(std::make_unique<Wt::WLabel>(_TR("PM2.5")), row, 0);
   _pm2_5 = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("0.0")),row++, 1);
   layout->addWidget(std::make_unique<Wt::WLabel>(_TR("PM10")), row, 0);
   _pm10 = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("0.0")),row++, 1);
   layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Status")), row, 0);
   _status = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("OK")),row , 1);
   //

   setLayout(std::move(layout));
}

void MRL::OPCN3WebPanel::onTimer()
{
    _handler.process();
}


/*!
 * \brief MRL::OPCN3WebPanel::update
 */
void MRL::OPCN3WebPanel::update()
{
    MRL::OPCN3RT *p =  MRL::OPCN3RT::find(_id); // thread safe access
    if(p)
    {
        double p0 = p->runtime().getDouble("/PM1/Value");
        double p1 = p->runtime().getDouble("/PM2.5/Value");
        double p2 = p->runtime().getDouble("/PM10/Value");

        _pm1->setText( wxString::Format("%6.1f",p0).ToStdString());
        _pm2_5->setText( wxString::Format("%6.1f",p1).ToStdString());
        _pm10->setText( wxString::Format("%6.1f",p2).ToStdString());
        _status->setText(p->runtime().getString("/StatusDesc"));
    }
}

/*!
 * \brief MRL::OPCN3WebPanel::Handler::processQueueItem
 * \param msg
 * \return
 */
bool MRL::OPCN3WebPanel::Handler::processQueueItem(const MRL::Message &msg)
{
    if (!MRL::BObject::processQueueItem(msg)) {
        // notifications from DAQ thread
        switch (msg.id()) {
        //
        case MESSAGEID::Started: // object has started
        {
            if(msg.source() == dbId())
            {
                _parent->status()->setText(_TR("STARTED"));
            }
        }
        break;

        case MESSAGEID::Stopped: // object has started
        {
            if(msg.source() == dbId())
            {
                _parent->status()->setText(_TR("STOPPED"));
            }
        }
        break;

        case MESSAGEID::IdleTimeout:

            break;

        case MESSAGEID::Update_Object:
        {
            if(msg.source() == dbId())
            {
                _parent->update();
            }
        }
        break;

        default:
            return false;
        }
    }
    return true;

}
