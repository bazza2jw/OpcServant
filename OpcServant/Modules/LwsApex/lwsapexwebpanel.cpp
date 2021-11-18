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
#include "lwsapexwebpanel.h"
#include <Wt/WVBoxLayout.h>
#include <Wt/WTable.h>
#include "lwsapexrt.h"

/*!
 * \brief MRL::LwsApexWebPanel::LwsApexWebPanel
 */
MRL::LwsApexWebPanel::LwsApexWebPanel(unsigned id): _handler(id,this)
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
    _timer->timeout().connect(this, &LwsApexWebPanel::onTimer);
    _timer->setInterval(std::chrono::seconds(1));
    _timer->start();


   auto layout = std::make_unique<Wt::WGridLayout>(); // a bag grid layout
   layout->setColumnStretch(0,1);
   layout->setColumnStretch(1,1);
   layout->setHorizontalSpacing(10);
   int row = 0;
   layout->addWidget(std::make_unique<Wt::WLabel>(_TR("0.5um (N/cuft)")), row, 0);
   _particles_0_5 = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("0.0")),row++, 1);
   layout->addWidget(std::make_unique<Wt::WLabel>(_TR("5.0um (N/cuft)")), row, 0);
   _particles_5_0 = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("0.0")),row++, 1);
   layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Status")), row, 0);
   _status = layout->addWidget(std::make_unique<Wt::WLabel>(_TR("OK")),row , 1);
   //

   setLayout(std::move(layout));
}

/*!
 * \brief MRL::LwsApexWebPanel::update
 */
void MRL::LwsApexWebPanel::update()
{
    MRL::LwsApexRT *p =  MRL::LwsApexRT::find(_id); // thread safe access
    if(p)
    {
        double p0 = p->runtime().getDouble("/0.5/Value");
        double p1 = p->runtime().getDouble("/5.0/Value");
        _particles_0_5->setText( wxString::Format("%6.1f",p0).ToStdString());
        _particles_5_0->setText( wxString::Format("%6.1f",p1).ToStdString());
        _status->setText(p->runtime().getString("/StatusDesc"));
    }
}

/*!
 * \brief MRL::LwsApexWebPanel::Handler::processQueueItem
 * \param msg
 * \return
 */
bool MRL::LwsApexWebPanel::Handler::processQueueItem(const MRL::Message &msg)
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
