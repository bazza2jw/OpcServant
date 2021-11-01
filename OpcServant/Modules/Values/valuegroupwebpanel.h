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
#ifndef VALUEGROUPWEBPANEL_H
#define VALUEGROUPWEBPANEL_H
#include <MrlLib/mrllib.h>
#include <Common/common.h>
#include <Common/database.h>
#include <Common/displaypropertytree.h>
#include <Common/objectmanager.h>
#include <Common/Web/webdialog.h>
#include <Wt/WText.h>
#include <Wt/WTimer.h>

namespace MRL
{
/*!
 * \brief The ValueGroupWebPanel class
 */
    class ValueGroupWebPanel : public Wt::WContainerWidget
    {

        // message handler - cannot do multiple inhertitance
        /*!
         * \brief The Handler class
         */
        class Handler : public BObject
        {
            ValueGroupWebPanel * _parent = nullptr;
        public:
            Handler(unsigned id, ValueGroupWebPanel * p ) : BObject(id), _parent(p)
            {
                join(MRL::Daq::instance()); // attach to the DAQ manager
                MRL::BObject::setEnabled(true); // enable this object
            }
            bool processQueueItem(const MRL::Message &msg);

        };
        //
        friend class  Handler;
        //
        Handler _handler; // message handler
        Wt::WTable * _table = nullptr ; // current values in a table
        Wt::WTable * _statusLine = nullptr;
        Wt::WTimer *_timer = nullptr;

        unsigned _id = 0;
        //
        /*!
         * \brief setValue
         * \param r
         * \param c
         * \param v
         */
        void setValue(int r, int c, double v)
        {
            char b[32];
            sprintf(b,"%8.1f",v);
            Wt::WText *w = dynamic_cast<Wt::WText *>(_table->elementAt(r,c)->widget(0));
            if(w)
            {
                w->setText(b);
            }
        }
        /*!
         * \brief onTimer
         */
        void onTimer()
        {
            _handler.process();
        }

    public:
        /*!
         * \brief ValueGroupWebPanel
         * \param id
         */
        ValueGroupWebPanel(unsigned id);
        /*!
         * \brief setupTable
         */
        void setupTable();
        /*!
         * \brief updateTable
         */
        void updateTable();
    };
}
#endif // VALUEGROUPWEBPANEL_H
