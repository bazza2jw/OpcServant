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
#ifndef DAQ_H
#define DAQ_H
#include <Common/common.h>
#include <Common/bobject.h>
#include <Common/messages.h>
#include <Common/Daq/localdb.h>

//
//
namespace MRL {
    /*!
           \brief The Daq class
           Data collection main object - drives all data collection
    */
    constexpr const char *LOCAL_DB_NAME = "LOCAL.db";
    class Daq : public BObject {
            static Daq *_instance;   //!< singleton for runtime
            time_t _now = 0;  // cache for loop
            //
            // Signals for timing events
            //
            SIGNAL1(oneSecondTimer, time_t)
            SIGNAL1(oneMinuteTimer, time_t)
            SIGNAL1(processTimer, time_t)
            SIGNAL1(oneHourTimer, time_t)
            SIGNAL1(oneDayTimer, time_t)

            // Signal for object publishing event
            SIGNAL1(publishMessage,const Message &)
            //
            wxStopWatch sw;
            std::unique_ptr<LocalDb> _localDb; // the local database
            //
            bool _autoPurgeDatabase = false; // is the 90 day purge enabled
            //
        public:
            //
            Daq();
            ~Daq();
            //
            void start(); //!< start the data acquisition
            void stop();  //!< stop the data acquisition
            void process(); //!< drive the data acquisiiton process objects
            bool processQueueItem(const Message &); //!< process an item in the queue
            //
            void addObject(unsigned id); // add an object to the data aquisition object
            void removeObject(unsigned id); // remove an object from the data acquisition object
            //
            /*!
             * \brief autoPurgeDatabase
             * \return
             */
            bool autoPurgeDatabase() const { return _autoPurgeDatabase;}
            /*!
             * \brief SetAutoPurgeDatabase
             * \param f
             */
            void setAutoPurgeDatabase(bool f)
            {
                _autoPurgeDatabase = f;
            }
            // Accessors
            //
            static Daq *instance() {
                wxASSERT(_instance);
                return _instance;
            }
            /*!
             * \brief now
             * \return current (timeslice) time
             */
            time_t now() {
                return _now;
            }



            /*!
             * \brief post - simple notifications
             * \param msgId
             */
            void post(int msgId)
            {
                Message m(msgId);
                queueItem(m);
            }

            /*!
             * \brief localDb
             * \return
             */
            std::unique_ptr<LocalDb> &localDb()
            {
                return  _localDb;
            }

    };
    constexpr time_t secondsPerDay = 3600 * 24;
    constexpr time_t secondsPerHour = 3600;
    constexpr time_t secondsPerMinute = 60;
}
#endif // DAQ_H
