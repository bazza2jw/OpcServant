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
#include "valuegroupwebpanel.h"
#include "valuert.h"
#include <Wt/WVBoxLayout.h>
#include <Wt/WTable.h>

#define ICON_RED "/resources/objectIcons/ledred.png"
#define ICON_YELLOW "/resources/objectIcons/ledyellow.png"
#define ICON_GREEN "/resources/objectIcons/ledgreen.png"
#define ICON_BLUE "/resources/objectIcons/ledblue.png"

/*!
 * \brief MRL::ValueGroupWebPanel::ValueGroupWebPanel
 * \param id
 */
MRL::ValueGroupWebPanel::ValueGroupWebPanel(unsigned id) : _handler(id,this), _id(id)
{
    //
    _timer = addChild(std::make_unique<Wt::WTimer>());
    _timer->timeout().connect(this, &ValueGroupWebPanel::onTimer);
    _timer->setInterval(std::chrono::seconds(1));
    _timer->start();
    //
    //
    auto boxLayout = std::make_unique<Wt::WVBoxLayout>();


    Wt::WCssDecorationStyle css;
    Wt::WFont ft(Wt::FontFamily::SansSerif );
    ft.setSize(Wt::FontSize::Large);
    ft.setWeight(Wt::FontWeight::Bold);
    css.setFont(ft);
    css.setBackgroundColor(Wt::StandardColor::LightGray);
    //

    _table = boxLayout->addWidget(std::make_unique<Wt::WTable>(),2);
    _table->setHeaderCount(1);
    _table->setDecorationStyle(css);
    _table->setWidth(Wt::WLength("100%"));
    _table->addStyleClass("table form-inline");
    _table->elementAt(0, 0)->addNew<Wt::WText>(_TR("Value"));
    _table->elementAt(0, 1)->addNew<Wt::WText>(_TR("Current"));
    _table->elementAt(0, 2)->addNew<Wt::WText>(_TR("Mean"));
    _table->elementAt(0, 3)->addNew<Wt::WText>(_TR("Minimum"));
    _table->elementAt(0, 4)->addNew<Wt::WText>(_TR("Maximum"));
    _table->elementAt(0, 5)->addNew<Wt::WText>(_TR("State"));
    //
    setupTable();
    //
    _table->toggleStyleClass("table-bordered", true);
    _table->toggleStyleClass("table-condensed", true);

    _statusLine = boxLayout->addWidget(std::make_unique<Wt::WTable>());
    _statusLine->setWidth(Wt::WLength("100%"));
    _statusLine->elementAt(0, 0)->addNew<Wt::WText>(_TR("STATUS"));
    _statusLine->elementAt(0, 1)->addNew<Wt::WText>(_TR("Waiting")); // default P & D
    _statusLine->elementAt(0, 2)->addNew<Wt::WImage>(ICON_BLUE);
    setLayout(std::move(boxLayout));

}

/*!
 * \brief MRL::ValueGroupWebPanel::setupTable
 */
void MRL::ValueGroupWebPanel::setupTable() // set up the grid - called on start
{
    MRL::ValueRT *v =  MRL::ValueRT::find(_id);
    if(v)
    {
        if(v->values().size() > 0)
        {
            int j = 1;
            for(auto i = v->values().begin(); i != v->values().end(); i++, j++)
            {
                _table->elementAt(j, 0)->addNew<Wt::WText>(Wt::WString(i->first));
                _table->elementAt(j, 1)->addNew<Wt::WText>(Wt::WString("0.0"));
                _table->elementAt(j, 2)->addNew<Wt::WText>(Wt::WString("0.0"));
                _table->elementAt(j, 3)->addNew<Wt::WText>(Wt::WString("0.0"));
                _table->elementAt(j, 4)->addNew<Wt::WText>(Wt::WString("0.0"));
                _table->elementAt(j, 5)->addNew<Wt::WText>(Wt::WString("OK"));
                _table->elementAt(j, 6)->addNew<Wt::WImage>(ICON_YELLOW);
            }
        }
    }
}


/*!
 * \brief MRL::ValueGroupWebPanel::updateTable
 */
void MRL::ValueGroupWebPanel::updateTable() // update the display
{
    MRL::ValueRT *v =  MRL::ValueRT::find(_id);
    if(v)
    {
        for(auto i = v->values().begin(); i != v->values().end(); i++)
        {
            MRL::ValueRT::ValueItem &a = i->second;
            //
            // find the row
            //
             //
            Wt::WString n(i->first);
            //
            for(int j = 1; j < _table->rowCount(); j++)
            {
                {
                    Wt::WText * w = dynamic_cast<Wt::WText *>(_table->elementAt(j,0)->widget(0));
                    Wt::WImage *i = dynamic_cast<Wt::WImage *>(_table->elementAt(j,6)->widget(0));
                    Wt::WText  *s = dynamic_cast<Wt::WText *>(_table->elementAt(j,5)->widget(0));
                    Wt::WCssDecorationStyle &css= w->decorationStyle();  // item name
                    Wt::WCssDecorationStyle &scss= s->decorationStyle(); // status

                    if(w->text() == n)
                    {
                        //
                        setValue(j,1,a.stats().statistics().getLastValue());
                        setValue(j,2,a.stats().statistics().getMean());
                        setValue(j,3,a.stats().statistics().getMinimum());
                        setValue(j,4,a.stats().statistics().getMaximum());
                        //
                        switch(a.state())
                        {
                        case STATES::States::StateOk:
                        {
                            css.setForegroundColor(Wt::StandardColor::Black);
                            css.setBackgroundColor(Wt::StandardColor::Green);
                            scss.setForegroundColor(Wt::StandardColor::Black);
                            scss.setBackgroundColor(Wt::StandardColor::Green);
                            s->setText(_TR("OK"));
                            i->setImageLink(ICON_GREEN);
                        }
                            break;
                        case STATES::States::StateAction:
                            css.setForegroundColor(Wt::StandardColor::White);
                            css.setBackgroundColor(Wt::StandardColor::Red);
                            scss.setForegroundColor(Wt::StandardColor::White);
                            scss.setBackgroundColor(Wt::StandardColor::Red);
                            s->setText(_TR("ACTION"));
                            i->setImageLink(ICON_RED);
                            break;
                        case STATES::States::StateAlert:
                            css.setForegroundColor(Wt::StandardColor::Black);
                            css.setBackgroundColor(Wt::StandardColor::Yellow);
                            scss.setForegroundColor(Wt::StandardColor::Black);
                            scss.setBackgroundColor(Wt::StandardColor::Yellow);
                            s->setText(_TR("ALERT"));
                            i->setImageLink(ICON_YELLOW);
                            break;
                        default:
                            css.setForegroundColor(Wt::StandardColor::White);
                            css.setBackgroundColor(Wt::StandardColor::Blue);
                            scss.setForegroundColor(Wt::StandardColor::White);
                            scss.setBackgroundColor(Wt::StandardColor::Blue);
                            s->setText(_TR("IDLE"));
                            i->setImageLink(ICON_BLUE);
                            break;
                        }
                        break;
                    }
                }
            }
        }

// Update status line
        {

            Wt::WText * w = dynamic_cast<Wt::WText *>(_statusLine->elementAt(0,1)->widget(0));
            Wt::WImage *i =  dynamic_cast<Wt::WImage *>(_statusLine->elementAt(0,2)->widget(0));
            switch(v->runtime().getInt("/groupState"))
            {
            case STATES::States::StateOk:
                w->setText(_TR("OK"));
                i->setImageLink(ICON_GREEN);
                break;
            case STATES::States::StateAction:
                w->setText(_TR("ACTION"));
                i->setImageLink(ICON_RED);
                break;
            case STATES::States::StateAlert:
                w->setText(_TR("ALERT"));
                i->setImageLink(ICON_YELLOW);
                break;
            default:
                w->setText("");
                i->setImageLink(ICON_BLUE);
                break;
            }

        }
    }
    else
    {
//        GetStatus()->SetLabelText(_("MISSING"));
    }
}


/*!
 * \brief MRL::ValueGroupWebPanel::Handler::processQueueItem
 * \param msg
 * \return true on success
 */
bool MRL::ValueGroupWebPanel::Handler::processQueueItem(const MRL::Message &msg)
{
    if (!MRL::BObject::processQueueItem(msg)) {
        // notifications from DAQ thread
        switch (msg.id()) {
        //
        case MESSAGEID::Started: // object has started
        {
            if(msg.source() == dbId())
            {
                Wt::WText *w = dynamic_cast<Wt::WText *>(_parent->_statusLine->elementAt(0,1)->widget(0));
                Wt::WImage *i =  dynamic_cast<Wt::WImage *>(_parent->_statusLine->elementAt(0,2)->widget(0));
                w->setText(_TR("STARTED"));
                i->setImageLink(ICON_GREEN);
            }
        }
        break;

        case MESSAGEID::Stopped: // object has started
        {
            if(msg.source() == dbId())
            {
                Wt::WText * w = dynamic_cast<Wt::WText *>(_parent->_statusLine->elementAt(0,1)->widget(0));
                Wt::WImage *i =  dynamic_cast<Wt::WImage *>(_parent->_statusLine->elementAt(0,2)->widget(0));
                w->setText(_TR("STOPPED"));
                i->setImageLink(ICON_BLUE);
            }
        }
        break;

        case MESSAGEID::IdleTimeout:

            break;

        case MESSAGEID::Update_Object:
        {
            if(msg.source() == dbId())
            {
                _parent->updateTable();
            }
        }
        break;

        default:
            return false;
        }
    }
    return true;

}
