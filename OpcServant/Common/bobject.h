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
#ifndef BOBJECT_H
#define BOBJECT_H
#include "common_global.h"
#include <MrlLib/mrlsignals.h>
#include <queue>
#include <Wt/WObject.h>
#include <Common/messages.h>
#include "parameterids.h"
#include <MrlLib/mrlmutex.h>

namespace MRL {
    class BObject;

    /*!
        \brief The BObject class
        Objects with thread safe signal / slot message passing
        Messages are processed in the context of the owning thread
    */
    class BObject : public Wt::WObject {
            bool _enabled = false; //!< object enabled - capable of being started
            MessageQueue _queue; //!< message queue
            unsigned _dbId = 0; //!< object id (in database, if non-zero)
            bool _started = false; //!< object is active
        // Signals
            Wt::Signal<const Message &> _message;
        protected:
            mutable ReadWriteMutex _mutex; //!< mutex for read/write access
        // Signal Accessors
        public:
            Wt::Signal<const Message &> & message() { return _message;}
        public:
            /*!
                \brief BObject
            */
            BObject(unsigned i = 0) : _dbId(i) {}

            /*!
             * \brief loadConfiguration
             */
            virtual void loadConfiguration() {}

            ReadWriteMutex & mutex() { return _mutex;}
            /*!
             * \brief started
             * \return
             */
            bool started() const {
                return _started;
            }
            /*!
                \brief id
                \return
            */
            unsigned dbId() const {
                return _dbId;
            }

            /*!
             * \brief setDbId
             * \param i
             */
            void setDbId(unsigned i) {
                _dbId = i;
            }

            /*!
                \brief enabled
                \return
            */
            virtual bool enabled() const {
                return _enabled;
            }
            /*!
                \brief setEnabled
                \param f
            */
            virtual void setEnabled(bool f) {
                WriteLock l(_mutex);
                _enabled = f;
            }
            /*!
                \brief start
            */
            virtual void start() {
                _started = enabled();
            }
            /*!
                \brief stop
            */
            virtual void stop() {
                _started = false;
            }

            /*!
                \brief join
                Connect the object message signal from src to this object
                \param src
            */
            void join(BObject *src) {
                wxASSERT(src != nullptr);
                if(src) src->message().connect(this,&BObject::queueItem);
            }


            /*!
             * \brief postMessage
             * \param dest
             * \param o
             */
            void postMessage(BObject *dest, Message &o)
            {
                if(dest)
                {
                    o.setSource(dbId()); // add the source and destination
                    o.setDestination(dest->dbId());
                    sendMessage(dest, o);
                }
            }

            /*!
                \brief sendMessage
                \param dest
                \param o
            */

            static void sendMessage(BObject *dest, const Message &o) {
                if (dest) {
                    dest->queueItem(o);
                }
            }

            /*!
             * \brief sendMessage
             * \param dest
             * \param id
             */
            static void sendMessage(BObject *dest, unsigned id) {
                if (dest) {
                    Message o(id);
                    dest->queueItem(o);
                }
            }

            /*!
                \brief queueItem
                \param o
            */
            void queueItem(const Message &o) {
                WriteLock l(_mutex);
                if (_enabled) {
                    MessageRef r = std::make_unique<Message>(o);
                    _queue.add(r);
                }
            }
            /*!
                \brief processQueue
            */
            virtual void processQueue() {
                try
                {
                    Message *m;
                    while ((m = _queue.next())) {
                        if (enabled()) processQueueItem(*m);
                        _queue.remove();
                    }
                }
                CATCH_DEF
            }
            /*!
                \brief processQueueItem
            */
            virtual bool processQueueItem(const Message &);

            /*!
                \brief process
                process the data
            */
            virtual void process() {
                processQueue();
            }
    };
    typedef std::unique_ptr<BObject> BObjectRef; //!< reference object
}
#endif // BOBJECT_H
