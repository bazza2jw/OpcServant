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
#ifndef INTERPRETERRT_H
#define INTERPRETERRT_H
#include <Common/Daq/selene.h>
#include <Common/Daq/rtobject.h>
#include <MrlLib/stats.hpp>
#include <MrlLib/rollingbuffer.hpp>

namespace MRL {
    class InterpreterRT : public RTObject {
            PropertyPath _path; // where in the models the object data goes
            std::unique_ptr<sel::State> _state; // the LUA interpreter

            // flags for method presence of methods
            bool _haveStart = false;
            bool _haveStop = false;
            bool _havePublish = false;
            bool _haveMeasure = false;
            bool _haveProcess = false;
            //
        public:
            InterpreterRT(int id) : RTObject(id) {

            }
            /*!
                \brief start
            */
            virtual void start();
            /*!
                \brief stop
            */
            virtual void stop();
            /*!
                \brief publish
            */
            virtual void publish();
            /*!
                \brief measure
            */
            virtual void measure();
            /*!
                \brief updateOpc
                \param server
                \param objectNode
            */
            virtual void updateOpc(Open62541::Server &server, Open62541::NodeId &objectNode);
            /*!
                \brief initialiseOpc
                \param server
                \param objectNode
            */
            virtual void initialiseOpc(Open62541::Server &server, Open62541::NodeId &objectNode);
            /*!
                \brief publishHtmlPage
            */
            virtual void publishHtmlPage(CTML::Node &s); // publish the current runtime status as HTML text

            /*!
                \brief measureHtmlPage
            */
            virtual void measureHtmlPage(CTML::Node &s);

            /*!
             * \brief process
             */
            virtual void process();
            /*!
                \brief state
                \return
            */
            sel::State &state() {
                return *_state;
            }

            /*!
             * \brief trace
             * \param s
             */
            void trace(const char *s)
            {
                wxLogDebug("InterpreterRT Id %d %s",dbId(),s);
            }

            /*!
             * \brief load
             * \param file
             * \return  true on success
             */
            bool load(const std::string &file);

            /*!
             * \brief setupApi
             */
            virtual void setupApi();

            /*!
             * \brief create
             */
            bool create();

    };
}
#endif // INTERPRETERRT_H
